#pragma once
#include <vector>
#include <boost/asio.hpp>
#include "Singleton.h"
class AsioIOServicePool:public Singleton<AsioIOServicePool>
{
	friend Singleton<AsioIOServicePool>;
public:
	using IOService = boost::asio::io_context;
	using Work = boost::asio::io_context::work;//work构造函数接受一个io_context
	using WorkPtr = std::unique_ptr<Work>;
	~AsioIOServicePool();
	AsioIOServicePool(const AsioIOServicePool&) = delete;
	AsioIOServicePool& operator=(const AsioIOServicePool&) = delete;
	// 使用 round-robin 的方式返回一个 io_service
	boost::asio::io_context& GetIOService();
	void Stop();//回收资源，通知挂起的线程唤醒
private:
	AsioIOServicePool(std::size_t size = 2/*std::thread::hardware_concurrency()*/);
	std::vector<IOService> _ioServices;//actually, it is io_context
	std::vector<WorkPtr> _works;
	std::vector<std::thread> _threads;//一个io_context对应一个线程
	std::size_t   _nextIOService;//下一个io_context索引
};

