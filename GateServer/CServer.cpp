#include "CServer.h"
#include"HttpConnection.h"
#include"AsioIOServicePool.h"
CServer::CServer(boost::asio::io_context& ioc, unsigned short& port):
	_ioc(ioc),_acceptor(ioc,tcp::endpoint(tcp::v4(),port)) {

}

void CServer::Start()
{
	auto self = shared_from_this();//shared_from_this������ֱ�ӹ�������ָ�룬��ֹ������������ָ�����һƬ����
	auto& io_context = AsioIOServicePool::GetInstance()->GetIOService();
	std::shared_ptr<HttpConnection> new_connection = std::make_shared<HttpConnection>(io_context);
	_acceptor.async_accept(new_connection->GetSocket(), [self,new_connection](beast::error_code ec) {
		try {
			//if the link is wrong,listen for other links
			if (ec) {
				self->Start();
				return;
			}
			//create a new link��create HttpConnection class to manage the link
			//ʹ���̳߳غ󣬲���Ҫ������ֱ��ʹ��
			new_connection->Start();
			self->Start();
		}
		catch (std::exception &exp) {

		}
		});//���۳ɹ�ʧ�ܶ���һ���ص�����,capture the self,�ӳ�����ָ�����ʱ��

}
