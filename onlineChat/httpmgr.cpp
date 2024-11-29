#include "httpmgr.h"

HttpMgr::~HttpMgr()
{

}

HttpMgr::HttpMgr()
{
    connect(this,&HttpMgr::sig_http_finish,this,&HttpMgr::slot_http_finish);
    //连接http请求和完成信号，保证队列消费
}

void HttpMgr::PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod)
{
    QByteArray data=QJsonDocument(json).toJson();//接收数据流json对象序列化为比特流
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader,QByteArray::number(data.length()));
    auto self=shared_from_this();//允许一个对象安全地获得指向自身的 std::shared_ptr 智能指针
    //shared_from_this生成的智能指针和外面的智能指针共享计数，对引用计数+1，防止触发回调事件时对象消失
    QNetworkReply *reply=_manager.post(request,data);//send post request,返回一个server发回来的reply
    //处理错误情况,发送请求
    QObject::connect(reply,&QNetworkReply::finished,[self,reply,req_id,mod](){
        if(reply->error()!=QNetworkReply::NoError){ //发生错误
            qDebug()<<reply->errorString();
            //发信号通知完成
            emit self->sig_http_finish(req_id,"",ErrorCodes::ERR_NETWORK,mod);
            reply->deleteLater();
            return;
        }
    //等待信号，触发回调事件
    QString res=reply->readAll();
    emit self->sig_http_finish(req_id,res,ErrorCodes::SUCCESS,mod);
    reply->deleteLater();
    });
}

void HttpMgr::slot_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod)
{
    if(mod==Modules::REGISTERMOD){
        //发信号通知指定模块http响应结束
        emit sig_reg_mod_finish(id,res,err);
    }
    if(mod==Modules::RESETMOD){
        emit sig_reset_mod_finish(id,res,err);
    }
    if(mod==Modules::LOGINMOD){
        emit sig_login_mod_finish(id,res,err);
    }
}
