#ifndef TCPMGR_H
#define TCPMGR_H
#include <QTcpSocket>
#include "singleton.h"
#include "global.h"
#include<functional>

#include <QTcpSocket>
#include "singleton.h"
#include "global.h"
#include <functional>
#include <QObject>
#include "userdata.h"
#include <QJsonArray>

class TcpMgr:public QObject, public Singleton<TcpMgr>,
        public std::enable_shared_from_this<TcpMgr>
{
    Q_OBJECT
public:
   ~ TcpMgr();
private:
    friend class Singleton<TcpMgr>;
    TcpMgr();
    void initHandlers();
    void handleMsg(ReqId id, int len, QByteArray data);
    QTcpSocket _socket;
    QString _host;
    uint16_t _port;
    QByteArray _buffer;
    bool _b_recv_pending;//接收挂起,没有接收完全
    quint16 _message_id;//接收时先看id和长度，判断长度和后面数据的长度是否匹配，是否收完了
    quint16 _message_len;
    QMap<ReqId, std::function<void(ReqId id, int len, QByteArray data)>> _handlers;//处理回调函数,id，长度，字节流数组
public slots:
    void slot_tcp_connect(ServerInfo);//连接成功的回调
    void slot_send_data(ReqId reqId, QByteArray data);//发送数据的回调
signals:
    void sig_con_success(bool bsuccess);//连接成功信号，告诉别的界面
    void sig_send_data(ReqId reqId, QByteArray data);//发送数据
    void sig_swich_chatdlg(); //切换到聊天对话框
    void sig_load_apply_list(QJsonArray json_array);
    void sig_login_failed(int);
    void sig_user_search(std::shared_ptr<SearchInfo>);
    void sig_friend_apply(std::shared_ptr<AddFriendApply>);//对方申请的好友验证
    void sig_add_auth_friend(std::shared_ptr<AuthInfo>);//对方同意添加自己为好友
    void sig_auth_rsp(std::shared_ptr<AuthRsp>);//我同意对方加好友
    void sig_text_chat_msg(std::shared_ptr<TextChatMsg> msg);
};
#endif // TCPMGR_H
