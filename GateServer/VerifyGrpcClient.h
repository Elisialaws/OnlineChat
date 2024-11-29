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
//RPC,Զ�̹��̵���ͨ��Э�飬������һ�����򣨿ͻ��ˣ�
//����λ����һ̨��������������������еĳ���ĺ�������̣�������ñ��غ���һ�������ڷֲ�ʽ
class RPConPool {
public:
    RPConPool(size_t poolsize,std::string host,std::string port);
    ~RPConPool();
    void Close();
    std::unique_ptr<VarifyService::Stub>getConnection();
    void returnConnection(std::unique_ptr<VarifyService::Stub> context);
private:
    std::atomic<bool> b_stop_;//�Ƿ����
    size_t poolSize_;
    std::string host_;
    std::string port_;
    std::queue<std::unique_ptr<VarifyService::Stub>> connections_;//std::queue���̰߳�ȫ������ȷ����ȫ
    std::mutex mutex_;
    std::condition_variable cond_;//��������
};

 