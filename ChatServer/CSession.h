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
	tcp::socket& GetSocket();//返回与当前会话关联的 TCP 套接字的引用，以便进行网络操作。
	std::string& GetSessionId();//返回会话 ID 的引用，通常用于标识当前的会话。
	void SetUserId(int uid);
	int GetUserId();
	void Start();//启动会话，通常会开始异步读取数据或进行初始化工作。
	void Send(char* msg,  short max_length, short msgid);
	void Send(std::string msg, short msgid);
	void Close();//关闭回话，清理工作
	std::shared_ptr<CSession> SharedSelf();//允许对象安全地在异步操作和回调中保持有效性。
	void AsyncReadBody(int length);//开始异步读取消息体，读取指定长度的消息。
	void AsyncReadHead(int total_len);
private:
	void asyncReadFull(std::size_t maxLength, std::function<void(const boost::system::error_code& , std::size_t)> handler);
	void asyncReadLen(std::size_t  read_len, std::size_t total_len,
		std::function<void(const boost::system::error_code&, std::size_t)> handler);
	
	//当写入操作完成时调用，
	//该方法会检查是否有错误并进行相应的处理，通常用于在写操作完成后更新状态或清理消息队列。
	void HandleWrite(const boost::system::error_code& error, std::shared_ptr<CSession> shared_self);
	tcp::socket _socket;
	std::string _session_id;//标识唯一session
	char _data[MAX_LENGTH];//存储数据
	CServer* _server;
	bool _b_close;//会话是否关闭
	std::queue<shared_ptr<SendNode> > _send_que;//接收到的消息结构的智能指针。
	std::mutex _send_lock;
	//收到的消息结构
	std::shared_ptr<RecvNode> _recv_msg_node;
	bool _b_head_parse;//为了解决粘包问题
	//收到的头部结构
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
