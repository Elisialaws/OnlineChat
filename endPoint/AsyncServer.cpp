#include "AsyncServer.h"
#include<iostream>
#include <iomanip>
#include <asyncinfo.h>
using namespace boost;
using namespace std;
void Session::Start() //在start处理绑定了一个读事件
{
	memset(_data, 0, max_length);//初始化为0
	_socket.async_read_some(boost::asio::buffer(_data, max_length),bind(&Session::handle_read,this,
		placeholders::_1,placeholders::_2,shared_from_this())); //有读事件触发就会触发handle_read
	//socket底层缓冲区由空变成有数据,shared_from_this返回一个智能指针，和其他智能指针同步管理一块内存
}

std::string Session::GetUuid()
{
	return _uuid;
}

void Session::handle_read(const boost::system::error_code& error, size_t bytes_transfered,
	std::shared_ptr<Session>_self_shared)
{
	if (!error) {
		boost::asio::async_write(_socket, boost::asio::buffer(_data, bytes_transfered),
			bind(&Session::handle_write,this,placeholders::_1,_self_shared));
	}//发送数据由用户态拷贝到tcp的用户缓冲区,异步发送情况下，tcp缓冲区没满就能发送，触发写回调 
	//每次bind绑定session,引用计数+1
	else {
		_server->ClearSession(_uuid);
		//delete this; //error>0,有错误
	}
}

void Session::handle_write(const boost::system::error_code& error, std::shared_ptr<Session>_self_shared)
{
	if (!error) {
		memset(_data, 0, max_length);//发送成功了，把之前的数据清空,把读事件传递，监听
		_socket.async_read_some(boost::asio::buffer(_data, max_length), std::bind(&Session::handle_read,
			this, placeholders::_1, placeholders::_2,_self_shared));
	}
	else {
		_server->ClearSession(_uuid); 
		delete this;
	}
}
void Session::HandleRead(const boost::system::error_code& error, size_t  bytes_transferred, std::shared_ptr<Session> shared_self) {
    if (!error) {
        //已经移动的字符数
        int copy_len = 0;
        while (bytes_transferred > 0) {
            if (!_b_head_parse) { //开始头部没有解析
                //收到的数据不足头部大小
                if (bytes_transferred + _recv_head_node->_cur_len < HEAD_LENGTH) {
                    memcpy(_recv_head_node->_data + _recv_head_node->_cur_len, _data + copy_len, bytes_transferred);
                    //当前未处理的全部拷贝
                    _recv_head_node->_cur_len += bytes_transferred;
                    ::memset(_data, 0, MAX_LENGTH);//使用全局作用域的memset
                    _socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH),
                        std::bind(&Session::HandleRead, this, std::placeholders::_1, std::placeholders::_2, shared_self));
                    return;
                }
                //收到的数据比头部多
                //头部剩余未复制的长度
                int head_remain = HEAD_LENGTH - _recv_head_node->_cur_len;
                memcpy(_recv_head_node->_data + _recv_head_node->_cur_len, _data + copy_len, head_remain);
                //更新已处理的data长度和剩余未处理的长度
                copy_len += head_remain; //已经更新的
                bytes_transferred -= head_remain;//未处理的
                //获取头部数据
                short data_len = 0;
                memcpy(&data_len, _recv_head_node->_data, HEAD_LENGTH);
                cout << "data_len is " << data_len << endl;
                //长度非法
                if (data_len > MAX_LENGTH) {
                    std::cout << "invalid data length is " << data_len << endl;
                    _server->ClearSession(_uuid);
                    return;
                }
                _recv_msg_node = make_shared<MsgNode>(data_len);
                //消息的长度小于头部规定的长度，说明数据未收全，则先将部分消息放到接收节点里
                if (bytes_transferred < data_len) {
                    memcpy(_recv_msg_node->_data + _recv_msg_node->_cur_len, _data + copy_len, bytes_transferred);
                    //加上已经接收的数据的长度
                    _recv_msg_node->_cur_len += bytes_transferred;
                    ::memset(_data, 0, MAX_LENGTH);
                    _socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH),
                        std::bind(&Session::HandleRead, this, std::placeholders::_1, std::placeholders::_2, shared_self));
                    //头部处理完成
                    _b_head_parse = true;//下次触发HandleRead不会进入到这里
                    return;
                }
                memcpy(_recv_msg_node->_data + _recv_msg_node->_cur_len, _data + copy_len, data_len);
                _recv_msg_node->_cur_len += data_len;
                copy_len += data_len;
                bytes_transferred -= data_len;
                _recv_msg_node->_data[_recv_msg_node->_total_len] = '\0';
                cout << "receive data is " << _recv_msg_node->_data << endl;
                //此处可以调用Send发送测试
                Send(_recv_msg_node->_data, _recv_msg_node->_total_len);
                //继续轮询剩余未处理数据
                _b_head_parse = false;
                _recv_head_node->Clear();
                if (bytes_transferred <= 0) { //没有粘包，直接返回
                    ::memset(_data, 0, MAX_LENGTH);
                    _socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH),
                        std::bind(&Session::HandleRead, this, std::placeholders::_1, std::placeholders::_2, shared_self));
                    return;
                }
                continue;//粘包了，继续操作,bytes_transfered>0，说明多个包内容串在一起了
            }
            //已经处理完头部，处理上次未接受完的消息数据
            //接收的数据仍不足剩余未处理的
            int remain_msg = _recv_msg_node->_total_len - _recv_msg_node->_cur_len;
            if (bytes_transferred < remain_msg) { //还没接收完
                memcpy(_recv_msg_node->_data + _recv_msg_node->_cur_len, _data + copy_len, bytes_transferred);
                _recv_msg_node->_cur_len += bytes_transferred;
                ::memset(_data, 0, MAX_LENGTH);
                _socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH),
                    std::bind(&Session::HandleRead, this, std::placeholders::_1, std::placeholders::_2, shared_self));
                return;
            }
            memcpy(_recv_msg_node->_data + _recv_msg_node->_cur_len, _data + copy_len, remain_msg);
           //消息体接收完了
            _recv_msg_node->_cur_len += remain_msg;
            bytes_transferred -= remain_msg;
            copy_len += remain_msg;
            _recv_msg_node->_data[_recv_msg_node->_total_len] = '\0';
            cout << "receive data is " << _recv_msg_node->_data << endl;
            //此处可以调用Send发送测试
            Send(_recv_msg_node->_data, _recv_msg_node->_total_len);
            //继续轮询剩余未处理数据
            _b_head_parse = false; //下一轮处理，更改变量
            _recv_head_node->Clear();
            if (bytes_transferred <= 0) {
                ::memset(_data, 0, MAX_LENGTH);
                _socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH),
                    std::bind(&Session::HandleRead, this, std::placeholders::_1, std::placeholders::_2, shared_self));
                return;
            }
            continue;
        }
    }
    else {
        std::cout << "handle read failed, error is " << error.what() << endl;
        Close();
        _server->ClearSession(_uuid);
    }
}
void Session::PrintRecvData(char* data, int length) {
    stringstream ss;
    string result = "0x";
    for (int i = 0; i < length; i++) {
        string hexstr;
        ss << hex << std::setw(2) << std::setfill('0') << int(data[i]) << endl;
        ss >> hexstr;
        result += hexstr;
    }
    std::cout << "receive raw data is : " << result << endl;;
}
void Session::HandleWrite(const boost::system::error_code& error, std::shared_ptr<Session> _self_shared)
{
	if (!error) {
		std::lock_guard<std::mutex> lock(_send_lock);
		_send_que.pop();//发送完成，队列里弹出数据
		if (!_send_que.empty()) {
			auto& msgnode = _send_que.front();//下一次发送
			boost::asio::async_write(_socket, boost::asio::buffer(msgnode->_data, msgnode->_total_len),
				std::bind(&Session::HandleWrite, this, std::placeholders::_1, _self_shared));
		}
	}
	else {
		std::cout << "handle write failed, error is " << error.what() << endl;
		_server->ClearSession(_uuid);
	}
}

void Session::Send(char* msg, int max_length)
{
	bool pending = false;
	std::lock_guard<std::mutex>lock(_send_lock);//lock_guard管理互斥锁的生命周期,构造完直接上锁
	if (_send_que.size() > 0) {
		pending = true;
	}
	_send_que.push(make_shared<MsgNode>(msg, max_length));
	if (pending) {
		return;
	}
	asio::async_write(_socket, boost::asio::buffer(msg, max_length),
		std::bind(&Session::HandleWrite, this, std::placeholders::_1, shared_from_this()));
}

Server::Server(boost::asio::io_context& ioc, short port):_ioc(ioc),_acceptor(ioc,tcp::endpoint(tcp::v4(),port))
{

}

void Server::ClearSession(std::string uuid)
{
	_sessions.erase(uuid);
}

void Server::start_accept()
{
	std::shared_ptr<Session> new_session=make_shared<Session>(_ioc, this);
	_acceptor.async_accept(new_session->Socket(), std::bind(&Server::handle_accept, this,
		new_session,std::placeholders::_1));//专门为新的连接创建,placeholder占位符接收返回的error_code参数

}

void Server::handle_accept(std::shared_ptr<Session>new_session, const boost::system::error_code& error)
{
	if (!error) {
		new_session->Start();
		_sessions.insert(make_pair(new_session->GetUuid(), new_session));
	}
	else {//出现错误
		//delete new_session;不在监听函数后，函数对象没有生命周期，new_session引用计数-1
	}
	start_accept();//接收新的会话
}
int main() {
    try {
        //创建上下文服务,io_context相当于asio的网络层
        boost::asio::io_context ioc;//io_context.run(),底层进行一个死循环，检测到事件就会触发我们的回调函数
        //构造endpoint,调用了async_write,read异步读写函数，都会向io_context注册读写事件
        //注册事件实际上给注册模型注册，windows是iocp模型，Linux是epoll模型，放到就绪队列里，asio底层就会不断调用
        tcp::endpoint  remote_ep(asio::ip::address::from_string("127.0.0.1"), 10086);
        tcp::socket  sock(ioc);
        boost::system::error_code   error = boost::asio::error::host_not_found; ;
        sock.connect(remote_ep, error);
        if (error) {
            cout << "connect failed, code is " << error.value() << " error msg is " << error.message();
            return 0;
        }
        thread send_thread([&sock] {
            for (;;) {
                this_thread::sleep_for(std::chrono::milliseconds(2));
                const char* request = "hello world!";
                size_t request_length = strlen(request);
                char send_data[MAX_LENGTH] = { 0 };
                memcpy(send_data, &request_length, 2);
                memcpy(send_data + 2, request, request_length);
                boost::asio::write(sock, boost::asio::buffer(send_data, request_length + 2));
            }
            });
        thread recv_thread([&sock] {
            for (;;) {
                this_thread::sleep_for(std::chrono::milliseconds(2));
                cout << "begin to receive..." << endl;
                char reply_head[HEAD_LENGTH];
                size_t reply_length = boost::asio::read(sock, boost::asio::buffer(reply_head, HEAD_LENGTH));
                short msglen = 0;
                memcpy(&msglen, reply_head, HEAD_LENGTH);
                char msg[MAX_LENGTH] = { 0 };
                size_t  msg_length = boost::asio::read(sock, boost::asio::buffer(msg, msglen));
                std::cout << "Reply is: ";
                std::cout.write(msg, msglen) << endl;
                std::cout << "Reply len is " << msglen;
                std::cout << "\n";
            }
            });
        send_thread.join();
        recv_thread.join();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << endl;
    }
    return 0;
}