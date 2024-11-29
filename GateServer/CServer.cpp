#include "CServer.h"
#include"HttpConnection.h"
#include"AsioIOServicePool.h"
CServer::CServer(boost::asio::io_context& ioc, unsigned short& port):
	_ioc(ioc),_acceptor(ioc,tcp::endpoint(tcp::v4(),port)) {

}

void CServer::Start()
{
	auto self = shared_from_this();//shared_from_this而不是直接构造智能指针，防止出现两个智能指针管理一片区域
	auto& io_context = AsioIOServicePool::GetInstance()->GetIOService();
	std::shared_ptr<HttpConnection> new_connection = std::make_shared<HttpConnection>(io_context);
	_acceptor.async_accept(new_connection->GetSocket(), [self,new_connection](beast::error_code ec) {
		try {
			//if the link is wrong,listen for other links
			if (ec) {
				self->Start();
				return;
			}
			//create a new link，create HttpConnection class to manage the link
			//使用线程池后，不需要创建，直接使用
			new_connection->Start();
			self->Start();
		}
		catch (std::exception &exp) {

		}
		});//无论成功失败都有一个回调函数,capture the self,延长智能指针持续时间

}
