#include "VerifyGrpcClient.h"
#include"ConfigMgr.h"
#include"const.h"
GetVarifyRsp VerifyGrpcClient::GetVarifyCode(std::string email)
{
    ClientContext context;//������
    GetVarifyRsp reply;//�ذ�
    GetVarifyReq request;
    request.set_email(email);
    auto stub = pool_->getConnection();

    Status status = stub->GetVarifyCode(&context, request, &reply);

    if (status.ok()) {
        pool_->returnConnection(std::move(stub));//�ͷ���Դ 
        return reply;
    }
    else {
        pool_->returnConnection(std::move(stub));
        reply.set_error(ErrorCodes::RPCFailed);
        return reply;
    }
}
VerifyGrpcClient::VerifyGrpcClient(){
     auto& gCfgMgr = ConfigMgr::Inst();
     std::string host = gCfgMgr["VarifyServer"]["Host"];
     std::string port = gCfgMgr["VarifyServer"]["Port"];
     pool_.reset(new RPConPool(5, host, port));
 }

 RPConPool::RPConPool(size_t poolsize, std::string host, std::string port):poolSize_(poolsize),
     host_(host),port_(port),b_stop_(false)
 {
     for (size_t i = 0; i < poolSize_; ++i) {
         std::shared_ptr<Channel> channel = grpc::CreateChannel(host + ":" + port,
             grpc::InsecureChannelCredentials());
         connections_.push(VarifyService::NewStub(channel));
   }
 }

 RPConPool::~RPConPool()
 {
     std::lock_guard<std::mutex>lock(mutex_);//����������ֶ�����
     Close();
     while (!connections_.empty()) {
         connections_.pop();//������Դ��Ҫ֪ͨ�������е��߳�
     }
 }

 void RPConPool::Close()
 {
     b_stop_ = true;
     cond_.notify_all();
 }

 std::unique_ptr<VarifyService::Stub> RPConPool::getConnection()
 {
     std::unique_lock<std::mutex>lock(mutex_);//��ʱ����unlock��������������������һ��ʹ��
     cond_.wait(lock, [this]() {
         if (b_stop_) {
             return true;
         }
         return !connections_.empty();//����true�͹����ͷ������ȴ�Notify,return false�ͼ���ִ��
         });
     if (b_stop_) {
         return nullptr;
    }
    auto context= std::move(connections_.front());//ûֹͣ��
    connections_.pop();
    return context;
 }

 void RPConPool::returnConnection(std::unique_ptr<VarifyService::Stub> context)
 {
     std::lock_guard<std::mutex>lock(mutex_);
     if (b_stop_) {
         return; //û��Ҫ�ͷ�����
     }
     connections_.push(std::move(context));
     cond_.notify_one();
 }