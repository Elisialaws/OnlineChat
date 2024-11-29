#pragma once
#include"const.h"
class HttpConnection;
typedef std::function<void(std::shared_ptr<HttpConnection>)>HttpHandler;
class LogicSystem:public Singleton<LogicSystem>
{
	friend class Singleton<LogicSystem>;
public:
	~LogicSystem();
	bool HandleGet(std::string,std::shared_ptr<HttpConnection>);//handle get«Î«Û
	bool HandlePost(std::string, std::shared_ptr<HttpConnection>);//handle post«Î«Û
	void RegGet(std::string,HttpHandler);//register get request
	void RegPost(std::string url,HttpHandler handler);
private:
	LogicSystem();
	std::map<std::string, HttpHandler> _post_handlers;
	std::map<std::string, HttpHandler> _get_handlers;

};

