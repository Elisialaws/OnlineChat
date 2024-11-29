#include "AsyncServer.h"
#include<iostream>
#include <iomanip>
#include <asyncinfo.h>
using namespace boost;
using namespace std;
void Session::Start() //��start�������һ�����¼�
{
	memset(_data, 0, max_length);//��ʼ��Ϊ0
	_socket.async_read_some(boost::asio::buffer(_data, max_length),bind(&Session::handle_read,this,
		placeholders::_1,placeholders::_2,shared_from_this())); //�ж��¼������ͻᴥ��handle_read
	//socket�ײ㻺�����ɿձ��������,shared_from_this����һ������ָ�룬����������ָ��ͬ������һ���ڴ�
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
	}//�����������û�̬������tcp���û�������,�첽��������£�tcp������û�����ܷ��ͣ�����д�ص� 
	//ÿ��bind��session,���ü���+1
	else {
		_server->ClearSession(_uuid);
		//delete this; //error>0,�д���
	}
}

void Session::handle_write(const boost::system::error_code& error, std::shared_ptr<Session>_self_shared)
{
	if (!error) {
		memset(_data, 0, max_length);//���ͳɹ��ˣ���֮ǰ���������,�Ѷ��¼����ݣ�����
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
        //�Ѿ��ƶ����ַ���
        int copy_len = 0;
        while (bytes_transferred > 0) {
            if (!_b_head_parse) { //��ʼͷ��û�н���
                //�յ������ݲ���ͷ����С
                if (bytes_transferred + _recv_head_node->_cur_len < HEAD_LENGTH) {
                    memcpy(_recv_head_node->_data + _recv_head_node->_cur_len, _data + copy_len, bytes_transferred);
                    //��ǰδ�����ȫ������
                    _recv_head_node->_cur_len += bytes_transferred;
                    ::memset(_data, 0, MAX_LENGTH);//ʹ��ȫ���������memset
                    _socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH),
                        std::bind(&Session::HandleRead, this, std::placeholders::_1, std::placeholders::_2, shared_self));
                    return;
                }
                //�յ������ݱ�ͷ����
                //ͷ��ʣ��δ���Ƶĳ���
                int head_remain = HEAD_LENGTH - _recv_head_node->_cur_len;
                memcpy(_recv_head_node->_data + _recv_head_node->_cur_len, _data + copy_len, head_remain);
                //�����Ѵ����data���Ⱥ�ʣ��δ����ĳ���
                copy_len += head_remain; //�Ѿ����µ�
                bytes_transferred -= head_remain;//δ�����
                //��ȡͷ������
                short data_len = 0;
                memcpy(&data_len, _recv_head_node->_data, HEAD_LENGTH);
                cout << "data_len is " << data_len << endl;
                //���ȷǷ�
                if (data_len > MAX_LENGTH) {
                    std::cout << "invalid data length is " << data_len << endl;
                    _server->ClearSession(_uuid);
                    return;
                }
                _recv_msg_node = make_shared<MsgNode>(data_len);
                //��Ϣ�ĳ���С��ͷ���涨�ĳ��ȣ�˵������δ��ȫ�����Ƚ�������Ϣ�ŵ����սڵ���
                if (bytes_transferred < data_len) {
                    memcpy(_recv_msg_node->_data + _recv_msg_node->_cur_len, _data + copy_len, bytes_transferred);
                    //�����Ѿ����յ����ݵĳ���
                    _recv_msg_node->_cur_len += bytes_transferred;
                    ::memset(_data, 0, MAX_LENGTH);
                    _socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH),
                        std::bind(&Session::HandleRead, this, std::placeholders::_1, std::placeholders::_2, shared_self));
                    //ͷ���������
                    _b_head_parse = true;//�´δ���HandleRead������뵽����
                    return;
                }
                memcpy(_recv_msg_node->_data + _recv_msg_node->_cur_len, _data + copy_len, data_len);
                _recv_msg_node->_cur_len += data_len;
                copy_len += data_len;
                bytes_transferred -= data_len;
                _recv_msg_node->_data[_recv_msg_node->_total_len] = '\0';
                cout << "receive data is " << _recv_msg_node->_data << endl;
                //�˴����Ե���Send���Ͳ���
                Send(_recv_msg_node->_data, _recv_msg_node->_total_len);
                //������ѯʣ��δ��������
                _b_head_parse = false;
                _recv_head_node->Clear();
                if (bytes_transferred <= 0) { //û��ճ����ֱ�ӷ���
                    ::memset(_data, 0, MAX_LENGTH);
                    _socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH),
                        std::bind(&Session::HandleRead, this, std::placeholders::_1, std::placeholders::_2, shared_self));
                    return;
                }
                continue;//ճ���ˣ���������,bytes_transfered>0��˵����������ݴ���һ����
            }
            //�Ѿ�������ͷ���������ϴ�δ���������Ϣ����
            //���յ������Բ���ʣ��δ�����
            int remain_msg = _recv_msg_node->_total_len - _recv_msg_node->_cur_len;
            if (bytes_transferred < remain_msg) { //��û������
                memcpy(_recv_msg_node->_data + _recv_msg_node->_cur_len, _data + copy_len, bytes_transferred);
                _recv_msg_node->_cur_len += bytes_transferred;
                ::memset(_data, 0, MAX_LENGTH);
                _socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH),
                    std::bind(&Session::HandleRead, this, std::placeholders::_1, std::placeholders::_2, shared_self));
                return;
            }
            memcpy(_recv_msg_node->_data + _recv_msg_node->_cur_len, _data + copy_len, remain_msg);
           //��Ϣ���������
            _recv_msg_node->_cur_len += remain_msg;
            bytes_transferred -= remain_msg;
            copy_len += remain_msg;
            _recv_msg_node->_data[_recv_msg_node->_total_len] = '\0';
            cout << "receive data is " << _recv_msg_node->_data << endl;
            //�˴����Ե���Send���Ͳ���
            Send(_recv_msg_node->_data, _recv_msg_node->_total_len);
            //������ѯʣ��δ��������
            _b_head_parse = false; //��һ�ִ������ı���
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
		_send_que.pop();//������ɣ������ﵯ������
		if (!_send_que.empty()) {
			auto& msgnode = _send_que.front();//��һ�η���
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
	std::lock_guard<std::mutex>lock(_send_lock);//lock_guard������������������,������ֱ������
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
		new_session,std::placeholders::_1));//ר��Ϊ�µ����Ӵ���,placeholderռλ�����շ��ص�error_code����

}

void Server::handle_accept(std::shared_ptr<Session>new_session, const boost::system::error_code& error)
{
	if (!error) {
		new_session->Start();
		_sessions.insert(make_pair(new_session->GetUuid(), new_session));
	}
	else {//���ִ���
		//delete new_session;���ڼ��������󣬺�������û���������ڣ�new_session���ü���-1
	}
	start_accept();//�����µĻỰ
}
int main() {
    try {
        //���������ķ���,io_context�൱��asio�������
        boost::asio::io_context ioc;//io_context.run(),�ײ����һ����ѭ������⵽�¼��ͻᴥ�����ǵĻص�����
        //����endpoint,������async_write,read�첽��д������������io_contextע���д�¼�
        //ע���¼�ʵ���ϸ�ע��ģ��ע�ᣬwindows��iocpģ�ͣ�Linux��epollģ�ͣ��ŵ����������asio�ײ�ͻ᲻�ϵ���
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