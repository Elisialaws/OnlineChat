#pragma once
#include<boost/asio.hpp>
#include<iostream>
#include<map>
#include<boost/uuid/uuid_generators.hpp>
#include<boost/uuid/uuid_io.hpp>
#include<queue>
#define HEAD_LENGTH 2
#define MAX_LENGTH 1024*2
using namespace std;
using boost::asio::ip::tcp;
class MsgNode
{
    friend class Session;
public:
    //��Ϣ���Ⱥ���Ϣ���ݣ�2�ֽڳ��Ĵ�С��ʾ��Ϣ����
    MsgNode(char* msg, short max_len) :_total_len(max_len + HEAD_LENGTH), _cur_len(0) {
        _data = new char[_total_len + 1]();
        memcpy(_data, &max_len, HEAD_LENGTH);
        memcpy(_data + HEAD_LENGTH, msg, max_len);
        _data[_total_len] = '\0';
    }
    MsgNode(short max_len) :_total_len(max_len), _cur_len(0) {
        _data = new char[_total_len + 1]();
    }
    ~MsgNode() {
        delete[] _data;
    }
    void Clear() {
        ::memset(_data, 0, _total_len);
        _cur_len = 0;
    }
private:
    short _cur_len;
    short _total_len;
    char* _data;
};
class Server;
class Session: public std::enable_shared_from_this<Session>
{
public:
    Session(boost::asio::io_context& ioc,Server *server) :_socket(ioc),_server(server) {
        boost::uuids::uuid a_uuid = boost::uuids::random_generator()();
        _uuid = boost::uuids::to_string(a_uuid);
    }
    ~Session() {
        std::cout << "delete session" << endl;
    }
    tcp::socket& Socket() {
        return _socket;
    }
    void Start();
    std::string GetUuid();
private:
    void handle_read(const boost::system::error_code& error, size_t bytes_transfered,
        std::shared_ptr<Session>_self_shared);//�ͻ������ݵ����Ļص�����
    void handle_write(const boost::system::error_code& error, std::shared_ptr<Session>_self_shared);//д�Ļص�����
    void HandleWrite(const boost::system::error_code& error, std::shared_ptr<Session> _self_shared);
    void HandleRead(const boost::system::error_code& error, size_t  bytes_transferred, std::shared_ptr<Session> _self_shared);
    void PrintRecvData(char* data, int length);
    tcp::socket _socket;
    enum { max_length = 1024 };
    char _data[max_length];
    Server* _server;
    std::string _uuid;
    void Send(char* msg, int max_length);
    std::queue<std::shared_ptr<MsgNode> > _send_que;
    std::mutex _send_lock;
    //�յ�����Ϣ�ṹ
    std::shared_ptr<MsgNode> _recv_msg_node;
    bool _b_head_parse; //ͷ���Ƿ�������
    //�յ���ͷ���ṹ
    std::shared_ptr<MsgNode> _recv_head_node;
};
class Server {
public:
    Server(boost::asio::io_context& ioc, short port);
    void ClearSession(std::string uuid);
private:
    void start_accept();//����acceptor�����Է�����
    void handle_accept(std::shared_ptr<Session>new_session,const boost::system::error_code &error);//������������ʱ�����ص�����
    boost::asio::io_context& _ioc;//��������
    tcp::acceptor _acceptor;//acceptor�������ض˿ںŵ�ַ
    std::map<std::string,std::shared_ptr<Session>> _sessions;// α�հ�ʵ�����ӵİ�ȫ����
    //�ص�����û�н���֮ǰ����Ҫdelete,keyΪsession��uid
};