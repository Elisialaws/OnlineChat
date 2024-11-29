#ifndef HTTPMGR_H
#define HTTPMGR_H
#include"singleton.h"
#include<QString>
#include<QUrl>
#include<QObject>
#include<QNetworkAccessManager>
#include<QJsonObject>
#include<QJsonDocument>

/**
 * @brief The HttpMgr class
 * QTcpSocket：用于 TCP 客户端连接。
QTcpServer：用于 TCP 服务器。
QUdpSocket：用于 UDP 通信。
QNetworkAccessManager：用于高层的 HTTP 请求（GET、POST 等）。
QNetworkReply：用于处理网络请求的应答。
 */
class HttpMgr:public QObject, public Singleton<HttpMgr>,
        public std::enable_shared_from_this<HttpMgr>//使用shared_from_this需要继承std::enable_shared_from_this
{
    Q_OBJECT
public:
    ~HttpMgr();
        void PostHttpReq(QUrl url,QJsonObject json,ReqId req_id,Modules mod);//采用异步方式通信，有回调函数，需要知道是哪个回调函数
private:
    friend class Singleton<HttpMgr>;//父类访问子类私有构造函数
    HttpMgr();
    QNetworkAccessManager _manager;//基础的网络管理
private slots:
    void slot_http_finish(ReqId id,QString res,ErrorCodes err,Modules mod);
signals:
    void sig_http_finish(ReqId id,QString res,ErrorCodes err,Modules mod);
    void sig_reg_mod_finish(ReqId id,QString res,ErrorCodes err);
    void sig_reset_mod_finish(ReqId id,QString res,ErrorCodes err);
    void sig_login_mod_finish(ReqId id,QString res,ErrorCodes err);
};

#endif // HTTPMGR_H
