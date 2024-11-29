#include "VerifyGrpcClient.h"
#include"ConfigMgr.h"
#include"const.h"
GetVarifyRsp VerifyGrpcClient::GetVarifyCode(std::string email)
{
    ClientContext context;//上下文
    GetVarifyRsp reply;//回包
    GetVarifyReq request;
    request.set_email(email);
    auto stub = pool_->getConnection();

    Status status = stub->GetVarifyCode(&context, request, &reply);

    if (status.ok()) {
        pool_->returnConnection(std::move(stub));//释放资源 
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
     std::lock_guard<std::mutex>lock(mutex_);//构造后不允许手动解锁
     Close();
     while (!connections_.empty()) {
         connections_.pop();//回收资源需要通知正在运行的线程
     }
 }

 void RPConPool::Close()
 {
     b_stop_ = true;
     cond_.notify_all();
 }

 std::unique_ptr<VarifyService::Stub> RPConPool::getConnection()
 {
     std::unique_lock<std::mutex>lock(mutex_);//随时可以unlock解锁，可以与条件变量一起使用
     cond_.wait(lock, [this]() {
         if (b_stop_) {
             return true;
         }
         return !connections_.empty();//返回true就挂起，释放锁，等待Notify,return false就继续执行
         });
     if (b_stop_) {
         return nullptr;
    }
    auto context= std::move(connections_.front());//没停止且
    connections_.pop();
    return context;
 }

 void RPConPool::returnConnection(std::unique_ptr<VarifyService::Stub> context)
 {
     std::lock_guard<std::mutex>lock(mutex_);
     if (b_stop_) {
         return; //没必要释放连接
     }
     connections_.push(std::move(context));
     cond_.notify_one();
 }
