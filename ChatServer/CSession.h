#pragma once
#include <boost/asio.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <queue>
#include <mutex>
#include <memory>
#include "const.h"
#include "MsgNode.h"
using namespace std;
namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>


class CServer;
class LogicSystem;

class CSession: public std::enable_shared_from_this<CSession>
{
public:
	CSession(boost::asio::io_context& io_context, CServer* server);
	~CSession();
	tcp::socket& GetSocket();//�����뵱ǰ�Ự������ TCP �׽��ֵ����ã��Ա�������������
	std::string& GetSessionId();//���ػỰ ID �����ã�ͨ�����ڱ�ʶ��ǰ�ĻỰ��
	void SetUserId(int uid);
	int GetUserId();
	void Start();//�����Ự��ͨ���Ὺʼ�첽��ȡ���ݻ���г�ʼ��������
	void Send(char* msg,  short max_length, short msgid);
	void Send(std::string msg, short msgid);
	void Close();//�رջػ���������
	std::shared_ptr<CSession> SharedSelf();//�������ȫ�����첽�����ͻص��б�����Ч�ԡ�
	void AsyncReadBody(int length);//��ʼ�첽��ȡ��Ϣ�壬��ȡָ�����ȵ���Ϣ��
	void AsyncReadHead(int total_len);
private:
	void asyncReadFull(std::size_t maxLength, std::function<void(const boost::system::error_code& , std::size_t)> handler);
	void asyncReadLen(std::size_t  read_len, std::size_t total_len,
		std::function<void(const boost::system::error_code&, std::size_t)> handler);
	
	//��д��������ʱ���ã�
	//�÷��������Ƿ��д��󲢽�����Ӧ�Ĵ���ͨ��������д������ɺ����״̬��������Ϣ���С�
	void HandleWrite(const boost::system::error_code& error, std::shared_ptr<CSession> shared_self);
	tcp::socket _socket;
	std::string _session_id;//��ʶΨһsession
	char _data[MAX_LENGTH];//�洢����
	CServer* _server;
	bool _b_close;//�Ự�Ƿ�ر�
	std::queue<shared_ptr<SendNode> > _send_que;//���յ�����Ϣ�ṹ������ָ�롣
	std::mutex _send_lock;
	//�յ�����Ϣ�ṹ
	std::shared_ptr<RecvNode> _recv_msg_node;
	bool _b_head_parse;//Ϊ�˽��ճ������
	//�յ���ͷ���ṹ
	std::shared_ptr<MsgNode> _recv_head_node;
	int _user_uid;
};

class LogicNode {
	friend class LogicSystem;
public:
	LogicNode(shared_ptr<CSession>, shared_ptr<RecvNode>);
private:
	shared_ptr<CSession> _session;
	shared_ptr<RecvNode> _recvnode;
};
