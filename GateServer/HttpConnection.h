#pragma once
#include"const.h"
/**
_buffer ������������

_request ������������

_response ������Ӧ�ͻ���

_deadline ��������ʱ���ж������Ƿ�ʱ
*/

class HttpConnection :public std::enable_shared_from_this<HttpConnection>
{
public:
	friend class LogicSystem;
	HttpConnection(boost::asio::io_context& ioc);//��Ϊ�̳߳�ʵ�ֺ�ʹ��io_context���������socket
	void Start();
	tcp::socket& GetSocket();
private:
	void CheckDeadLine();
	void WriteResponse();
	void HandleReq();
	void PreParseGetParam();
	tcp::socket _socket;
	beast::flat_buffer _buffer{ 8192 };//buffer of the received data
    http::request<http::dynamic_body>_request;//dynamic_body,���ܸ�����������
	http::response<http::dynamic_body> _response;
	std::string _get_url;
	std::unordered_map<std::string, std::string> _get_params;
	net::steady_timer _deadline{ //�¼����ȺͶ�ʱ���ȶ��ŵ��ײ��ѭ�����Ҫ�󶨵�����
		_socket.get_executor(),std::chrono::seconds(60)
	};
};

