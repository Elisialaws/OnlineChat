#pragma once
#include"const.h"
/**
_buffer 用来接受数据

_request 用来解析请求

_response 用来回应客户端

_deadline 用来做定时器判断请求是否超时
*/

class HttpConnection :public std::enable_shared_from_this<HttpConnection>
{
public:
	friend class LogicSystem;
	HttpConnection(boost::asio::io_context& ioc);//改为线程池实现后，使用io_context构造而不是socket
	void Start();
	tcp::socket& GetSocket();
private:
	void CheckDeadLine();
	void WriteResponse();
	void HandleReq();
	void PreParseGetParam();
	tcp::socket _socket;
	beast::flat_buffer _buffer{ 8192 };//buffer of the received data
    http::request<http::dynamic_body>_request;//dynamic_body,接受各种数据类型
	http::response<http::dynamic_body> _response;
	std::string _get_url;
	std::unordered_map<std::string, std::string> _get_params;
	net::steady_timer _deadline{ //事件调度和定时调度都放到底层的循环里，需要绑定调度器
		_socket.get_executor(),std::chrono::seconds(60)
	};
};

