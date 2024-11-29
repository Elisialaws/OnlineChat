#pragma once
#include<grpcpp/grpcpp.h>
#include"message.grpc.pb.h"
#include"const.h"
#include"Singleton.h"
using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

using message::GetVarifyReq;
using message::GetVarifyRsp;
using message::VarifyService;
class VerifyGrpcClient :public Singleton<VerifyGrpcClient>
{
	friend class Singleton<VerifyGrpcClient>;
public:
	~VerifyGrpcClient() {
	}
    GetVarifyRsp GetVarifyCode(std::string email);
private:
	VerifyGrpcClient();

	std::unique_ptr<RPConPool> pool_;
};
//RPC,远程过程调用通信协议，它允许一个程序（客户端）
//调用位于另一台计算机（服务器）上运行的程序的函数或过程，就像调用本地函数一样。用于分布式
class RPConPool {
public:
    RPConPool(size_t poolsize,std::string host,std::string port);
    ~RPConPool();
    void Close();
    std::unique_ptr<VarifyService::Stub>getConnection();
    void returnConnection(std::unique_ptr<VarifyService::Stub> context);
private:
    std::atomic<bool> b_stop_;//是否回收
    size_t poolSize_;
    std::string host_;
    std::string port_;
    std::queue<std::unique_ptr<VarifyService::Stub>> connections_;//std::queue非线程安全，用锁确保安全
    std::mutex mutex_;
    std::condition_variable cond_;//条件变量
};

 