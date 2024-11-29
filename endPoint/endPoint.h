#pragma once
#include<memory>
#include"boost/asio.hpp"
#include<iostream>
#include<queue>
extern int client_end_point();
extern int server_end_point();
extern int create_tcp_socket();
extern int create_acceptor_socket();
extern int bind_acceptor_socket();
extern int connect_to_end();
extern int accept_new_connection();
extern int acceptor_new_connection();
extern void use_const_buffer();
extern void use_buffer_str();
extern void use_buffer_array();
class MsgNode;
class Session {
public:
    Session(std::shared_ptr<asio::ip::tcp::socket> socket);
    void Connect(const asio::ip::tcp::endpoint& ep);
    void WriteCallBackErr(const boost::system::error_code& ec, std::size_t bytes_transfered, std::shared_ptr<MsgNode> msg_node);//回调函数两个参数
    void WriteToSocketErr(const std::string buf);
    void WriteCallBack(const boost::system::error_code& ec, std::size_t bytes_transfered);//回调函数两个参数
    void WriteToSocket(const std::string buf);
    void WriteAllCallBack(const boost::system::error_code& ec, std::size_t bytes_transfered);//回调函数两个参数
    void WriteAllToSocket(const std::string buf);
    void ReadFromSocket();
    void ReadCallBack(const boost::system::error_code& ec, std::size_t bytes_transferred);
    void ReadAllFromSocket(const std::string& buf);
    void ReadAllCallBack(const boost::system::error_code& ec, std::size_t bytes_transferred);
private:
    std::shared_ptr<asio::ip::tcp::socket> _socket;
    std::shared_ptr<MsgNode> _send_node;
    std::queue<std::shared_ptr<MsgNode>>_send_queue;
    std::shared_ptr<MsgNode> _recv_node;
    bool _send_pending;
    bool _recv_pending;
};
const int RECVSIZE = 1024;
/*封装一个Node结构，用来管理要发送和接收的数据
*/
class MsgNode {
public:
    MsgNode(const char* msg, int total_len):_total_len(total_len),_cur_len(0) {
        _msg = new char[_total_len];
        memcpy(_msg, _msg, _total_len);
    }
    MsgNode(int total_len):_total_len(total_len), _cur_len(0) {
        _msg = new char[_total_len];
    }
    ~MsgNode() {
        delete[] _msg;
    }
    int _total_len;
    int _cur_len;
    char* _msg;
};