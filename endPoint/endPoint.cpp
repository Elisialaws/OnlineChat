#include"endPoint.h"
#include<boost/asio.hpp>
#include<iostream>
using namespace boost;
using namespace std;
class Session;
int client_end_point() {
    // Step 1. Assume that the client application has already 
  // obtained the IP-address and the protocol port number.
    std::string raw_ip_address = "127.0.0.1";
    unsigned short port_num = 3333;
    // Used to store information about error that happens
    // while parsing the raw IP-address.
    boost::system::error_code ec;
    // Step 2. Using IP protocol version independent address
    // representation.
    asio::ip::address ip_address =
        asio::ip::address::from_string(raw_ip_address, ec);
    if (ec.value() != 0) {
        // Provided IP address is invalid. Breaking execution.
        std::cout
            << "Failed to parse the IP address. Error code = "
            << ec.value() << ". Message: " << ec.message();
        return ec.value();
    }
    // Step 3.
    asio::ip::tcp::endpoint ep(ip_address, port_num);
    // Step 4. The endpoint is ready and can be used to specify a 
    // particular server in the network the client wants to 
    // communicate with.
    return 0;
}
int server_end_point() {
    unsigned short port_num = 3333;
    asio::ip::address ip_address = asio::ip::address_v6::any();
    asio::ip::tcp::endpoint ep(ip_address, port_num);
}
int create_tcp_socket() {
    // Step 1. An instance of 'io_service' class is required by
            // socket constructor. 
    asio::io_context  ios;
    // Step 2. Creating an object of 'tcp' class representing
    // a TCP protocol with IPv4 as underlying protocol.
    asio::ip::tcp protocol = asio::ip::tcp::v4();
    // Step 3. Instantiating an active TCP socket object.
    asio::ip::tcp::socket sock(ios);
    // Used to store information about error that happens
    // while opening the socket.
    boost::system::error_code ec;
    // Step 4. Opening the socket.
    sock.open(protocol, ec);
    if (ec.value() != 0) {
        // Failed to open the socket.
        std::cout
            << "Failed to open the socket! Error code = "
            << ec.value() << ". Message: " << ec.message();
        return ec.value();
    }
    return 0;
}
int create_acceptor_socket() {
    asio::io_context ioc;
    asio::ip::tcp protocol = asio::ip::tcp::v6();
    asio::ip::tcp::acceptor acceptor(ioc);
    boost::system::error_code ec;
    acceptor.open(protocol, ec);
    if (ec.value() != 0) {
        // Failed to open the socket.
        std::cout
            << "Failed to open the acceptor socket!"
            << "Error code = "
            << ec.value() << ". Message: " << ec.message();
        return ec.value();
    }
    return 0;
}
int bind_acceptor_socket() {
    unsigned short port_num = 3333;
    asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(), port_num);
    asio::io_context ios;
    asio::ip::tcp::acceptor acceptor(ios, ep.protocol());
    boost::system::error_code ec;
    acceptor.bind(ep, ec);
    // Handling errors if any.
    if (ec.value() != 0) {
        // Failed to bind the acceptor socket. Breaking
        // execution.
        std::cout << "Failed to bind the acceptor socket."
            << "Error code = " << ec.value() << ". Message: "
            << ec.message();
        return ec.value();
    }
    return 0;
}
int connect_to_end() {
    std::string raw_ip_address = "192.168.1.124";
    unsigned short port_num = 3333;
    try {
        asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);
        asio::io_context ios;
        asio::ip::tcp::socket sock(ios, ep.protocol());
        sock.connect(ep);
    }
    catch (system::system_error& e) {
        std::cout << "Error occured! Error code = " << e.code()
            << ". Message: " << e.what();
        return e.code().value();
    }
}
int accept_new_connection() {
    // The size of the queue containing the pending connection
           // requests.
    const int BACKLOG_SIZE = 30;
    // Step 1. Here we assume that the server application has
    // already obtained the protocol port number.
    unsigned short port_num = 3333;
    // Step 2. Creating a server endpoint.
    asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(),
        port_num);
    asio::io_context  ios;
    try {
        asio::ip::tcp::acceptor acceptor(ios, ep.protocol());//服务端接收客服端连接
        acceptor.bind(ep);
        acceptor.listen(BACKLOG_SIZE);//必须要listen,
        asio::ip::tcp::socket sock(ios);
        acceptor.accept(sock);//accpt会返回新的连接
    }
    catch (system::system_error& e) {
        std::cout << "Error occured! Error code = " << e.code()
            << ". Message: " << e.what();
        return e.code().value();
    }
}
void use_const_buffer() {
    std::string buf = "hello world";
    asio::const_buffer asio_buf(buf.c_str(), buf.length());
    std::vector<asio::const_buffer>buffers_sequence;
    buffers_sequence.push_back(asio_buf);
}
void use_buffer_str() {
    asio::const_buffers_1 output_buf = asio::buffer("hello world");
}
void use_buffer_array() {
    const size_t BUF_SIZE_BYTES = 20;
    std::unique_ptr<char[]>buf(new char[BUF_SIZE_BYTES]);//new出来的数组首地址交给unique_ptr管理
    auto input_buf = asio::buffer(static_cast<void*>(buf.get()), BUF_SIZE_BYTES);//get返回原始指针

}
void write_to_socket(asio::ip::tcp::socket& sock) {
    std::string buf = "hello world!";
    std::size_t total_bytes_written = 0;
    //循环发送,write_some返回每次写入字节数,每次向指定空间写入固定字节数
    while (total_bytes_written != buf.length()) {
        total_bytes_written += sock.write_some(asio::buffer(buf.c_str() + total_bytes_written,
            buf.length() - total_bytes_written));

    }
}
int  send_data_by_write_some() {
    std::string raw_ip_address = "127.0.0.1";
    unsigned short port_num = 3333;
    try {
        asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);
        asio::io_context ioc;
        asio::ip::tcp::socket sock(ioc, ep.protocol());
        sock.connect(ep);
        write_to_socket(sock);
    }
    catch (system::system_error& e) {
        return e.code().value();
        }
}
int send_data_by_send() {
    std::string raw_ip_address = "127.0.0.1";
    unsigned short port_num = 3333;
    try {
        asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);
        asio::io_context ioc;
        asio::ip::tcp::socket sock(ioc, ep.protocol());
        sock.connect(ep);
        std::string buf = "hello world";
        int send_length=sock.send(asio::buffer(buf.c_str(), buf.length()));//没发完，tcp会阻塞，直到发完数据
        if (send_length <= 0) {//说明发送失败
            return 0;
        }
    }
    catch (system::system_error& e) {
        return e.code().value();
    }
}
int send_data_by_write() {
    std::string raw_ip_address = "127.0.0.1";
    unsigned short port_num = 3333;
    try {
        asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);
        asio::io_context ioc;
        asio::ip::tcp::socket sock(ioc, ep.protocol());
        sock.connect(ep);
        std::string buf = "hello world";
        int send_length = asio::write(sock, asio::buffer(buf.c_str(), buf.length()));//同send,会阻塞tcp直到完成
        if (send_length <= 0) {//说明发送失败
            return 0;
        }
    }
    catch (system::system_error& e) {
        return e.code().value();
    }
}
std::string read_from_socket(asio::ip::tcp::socket &sock) {
    const unsigned char MESSAGE_SIZE = 7;
    char buf[MESSAGE_SIZE];
    std::size_t total_bytes_read = 0;
    while (total_bytes_read != MESSAGE_SIZE) {
        total_bytes_read += sock.read_some(asio::buffer(buf + total_bytes_read,
            MESSAGE_SIZE - total_bytes_read));
    }
    return std::string(buf, total_bytes_read);
}
int read_data_by_read_some() {
    std::string raw_ip_address = "127.0.0.1";
    unsigned short port_num = 3333;

    try {
        asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);
        asio::io_context ioc;
        asio::ip::tcp::socket sock(ioc, ep.protocol());
        sock.connect(ep);
        read_from_socket(sock);
    }
    catch (system::system_error& e) {
        return e.code().value();
    }
    return 0;
}
int read_data_by_receive() {
    std::string raw_ip_address = "127.0.0.1";
    unsigned short port_num = 3333;

    try {
        asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);
        asio::io_context ioc;
        asio::ip::tcp::socket sock(ioc, ep.protocol());
        sock.connect(ep);
        const unsigned char BUFF_SIZE = 7;
        char buffer_receive[BUFF_SIZE];
        int receive_length = sock.receive(asio::buffer(buffer_receive, BUFF_SIZE));//return the length of the operation
        boost::system::error_code error = boost::asio::error::host_not_found;
        if (receive_length <= 0) {
            //read failed
        }
    }
    catch (system::system_error& e) {
        return e.code().value();
    }
    return 0;
}
void session(boost::asio::ip::tcp::socket *sock) {
    constexpr int max_length = 1024;
    try {
        for (;;) {
            char data[max_length];
            memset(data, '\0', max_length);
            boost::system::error_code error;
            size_t length = sock->read_some(boost::asio::buffer(data, max_length), error);
            if (error == boost::asio::error::eof) {
                std::cout << "connection closed by peer" << endl;
                break;
            }
            else if (error) {
                throw boost::system::system_error(error);
            }

        }
    }
    catch (std::exception& e) {

    }
}
void server(boost::asio::io_context& io_context, unsigned short port) {
     asio::ip::tcp::acceptor a(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port));//接收客户端请求
    for (;;) {
        asio::ip::tcp::socket *socket(new asio::ip::tcp::socket(io_context));
        a.accept(*socket);
        auto t = std::make_shared<std::thread>(session, socket);//在独立的线程里处理
    }
}
Session::Session(std::shared_ptr<asio::ip::tcp::socket> socket): _socket(socket), 
_send_pending(false),_recv_pending(false) {

}
void Session::Connect(const asio::ip::tcp::endpoint& ep) {
    _socket->connect(ep);
}
//为了调用async_write_some函数也要传入一个符合WriteToken定义的函数，
//就是我们声明的WriteCallBackErr函数，
void Session::WriteCallBackErr(const boost::system::error_code& ec,
    std::size_t bytes_transferred, std::shared_ptr<MsgNode> msg_node) {
    if (bytes_transferred + msg_node->_cur_len
        < msg_node->_total_len) { //没发完
        _send_node->_cur_len += bytes_transferred; //更新已发送长度
        this->_socket->async_write_some(asio::buffer(_send_node->_msg + _send_node->_cur_len,
            _send_node->_total_len - _send_node->_cur_len),
            std::bind(&Session::WriteCallBackErr,
                this, std::placeholders::_1, std::placeholders::_2, _send_node));
    }
}
void Session::WriteToSocketErr(const std::string buf)
{
    _send_node = make_shared<MsgNode>(buf.c_str(), buf.length());
    //async_write_some是异步写的函数,创建buffer,包含要发送的消息
    //绑定回调函数，placeholders占位符接收回调参数,
    //this指针指向调用该成员函数的实例。bind需要有一个对象实例来调用
    this->_socket->async_write_some(asio::buffer(_send_node->_msg, _send_node->_total_len),
        std::bind(&Session::WriteCallBackErr,this,std::placeholders::_1,std::placeholders::_2,_send_node));
}
void Session::WriteCallBack(const boost::system::error_code& ec, std::size_t bytes_transfered)
{
    //出现错误
    if (ec.value() != 0) {
        return;
    }
    auto& send_data = _send_queue.front();
    send_data->_cur_len += bytes_transfered;
    if (send_data->_cur_len < send_data->_total_len) {//没发完继续发
        this->_socket->async_write_some(asio::buffer(send_data->_msg + send_data->_cur_len,
            send_data->_total_len - send_data->_cur_len), std::bind(&Session::WriteCallBack,
                this, std::placeholders::_1, std::placeholders::_2));
        return;
    }
    _send_queue.pop();//发完了弹出队首元素
    if(_send_queue.empty()) {
        _send_pending = false;
    }
    else {
        auto& send_data = _send_queue.front();
        this->_socket->async_write_some(asio::buffer(send_data->_msg + send_data->_cur_len, send_data->_total_len - send_data->_cur_len),
            std::bind(&Session::WriteCallBack,
                this, std::placeholders::_1, std::placeholders::_2));
    }

}
void Session::WriteToSocket(const std::string buf)
{
    _send_queue.emplace(new MsgNode(buf.c_str(), buf.length()));
    if (_send_pending) {
        return; //有没发完的数据
    }
    this->_socket->async_write_some(asio::buffer(buf), std::bind(&Session::WriteCallBack, this, std::placeholders::_1,
        std::placeholders::_2));//bind two parameters
    _send_pending = true;
}
void Session::WriteAllCallBack(const boost::system::error_code& ec, std::size_t bytes_transfered)
{
    if (ec.value() != 0) {
        std::cout << "Error occured! Error code = "
            << ec.value()
            << ". Message: " << ec.message();
        return;
    }
    //如果发送完，则pop出队首元素
    _send_queue.pop();
    //如果队列为空，则说明所有数据都发送完,将pending设置为false
    if (_send_queue.empty()) {
        _send_pending = false;
    }
    //如果队列不是空，则继续将队首元素发送
    if (!_send_queue.empty()) {
        auto& send_data = _send_queue.front();
        this->_socket->async_send(asio::buffer(send_data->_msg + send_data->_cur_len, send_data->_total_len - send_data->_cur_len),
            std::bind(&Session::WriteAllCallBack,
                this, std::placeholders::_1, std::placeholders::_2));
    }
}
void Session::WriteAllToSocket(const std::string buf)
{
    //插入发送队列
    _send_queue.emplace(new MsgNode(buf.c_str(), buf.length()));
    //pending状态说明上一次有未发送完的数据
    if (_send_pending) {
        return;
    }
    //异步发送数据，因为异步所以不会一下发送完
    this->_socket->async_send(asio::buffer(buf),
        std::bind(&Session::WriteAllCallBack, this,
            std::placeholders::_1, std::placeholders::_2));
    _send_pending = true;
}
void Session::ReadFromSocket()
{
    if (_recv_pending) {
        return;
    }
    _recv_node = std::make_shared<MsgNode>(RECVSIZE);
    _socket->async_read_some(asio::buffer(_recv_node->_msg, _recv_node->_total_len), std::bind(&Session::ReadCallBack, this,
        std::placeholders::_1, std::placeholders::_2));
    _recv_pending = true;
}
void Session::ReadCallBack(const boost::system::error_code& ec, std::size_t bytes_transferred)
{
    _recv_node->_cur_len += bytes_transferred;
    //没读完继续读
    if (_recv_node->_cur_len < _recv_node->_total_len) {
        _socket->async_read_some(asio::buffer(_recv_node->_msg + _recv_node->_cur_len,
            _recv_node->_total_len - _recv_node->_cur_len), std::bind(&Session::ReadCallBack, this,
                std::placeholders::_1, std::placeholders::_2));
        return;
    }
    //将数据投递到队列里交给逻辑线程处理，此处略去
    //如果读完了则将标记置为false
    _recv_pending = false;
    //指针置空
    _recv_node = nullptr;
}
void Session::ReadAllFromSocket(const std::string& buf) {
    if (_recv_pending) {
        return;
    }
    //可以调用构造函数直接构造，但不可用已经构造好的智能指针赋值
    /*auto _recv_nodez = std::make_unique<MsgNode>(RECVSIZE);
    _recv_node = _recv_nodez;*/
    _recv_node = std::make_shared<MsgNode>(RECVSIZE);
    _socket->async_receive(asio::buffer(_recv_node->_msg, _recv_node->_total_len), std::bind(&Session::ReadAllCallBack, this,
        std::placeholders::_1, std::placeholders::_2));
    _recv_pending = true;
}
void Session::ReadAllCallBack(const boost::system::error_code& ec, std::size_t bytes_transferred) {
    _recv_node->_cur_len += bytes_transferred;
    //将数据投递到队列里交给逻辑线程处理，此处略去
    //如果读完了则将标记置为false
    _recv_pending = false;
    //指针置空
    _recv_node = nullptr;
}
