#pragma once
#include <vector>
#include <boost/asio.hpp>
#include "Singleton.h"
class AsioIOServicePool:public Singleton<AsioIOServicePool>
{
	friend Singleton<AsioIOServicePool>;
public:
	using IOService = boost::asio::io_context;
	using Work = boost::asio::io_context::work;//work���캯������һ��io_context
	using WorkPtr = std::unique_ptr<Work>;
	~AsioIOServicePool();
	AsioIOServicePool(const AsioIOServicePool&) = delete;
	AsioIOServicePool& operator=(const AsioIOServicePool&) = delete;
	// ʹ�� round-robin �ķ�ʽ����һ�� io_service
	boost::asio::io_context& GetIOService();
	void Stop();//������Դ��֪ͨ������̻߳���
private:
	AsioIOServicePool(std::size_t size = 2/*std::thread::hardware_concurrency()*/);
	std::vector<IOService> _ioServices;//actually, it is io_context
	std::vector<WorkPtr> _works;
	std::vector<std::thread> _threads;//һ��io_context��Ӧһ���߳�
	std::size_t   _nextIOService;//��һ��io_context����
};

