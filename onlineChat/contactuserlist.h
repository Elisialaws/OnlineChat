#ifndef CONTACTUSERLIST_H
#define CONTACTUSERLIST_H

#include<QListWidget>
#include"userdata.h"
#include<memory>
#include"conuseritem.h"
#include"grouptipitem.h"
class ContactUserList : public QListWidget
{
    Q_OBJECT
public:
    explicit ContactUserList(QWidget *parent = nullptr);
    ~ContactUserList();
    void showRedPoint(bool bshow=true);
protected:
    bool eventFilter(QObject *watched,QEvent *event)override;
private:
    void addContactUserList();
    ConUserItem* _add_friend_item;
    QListWidgetItem * _groupitem;
    bool _load_pending;
public slots:
     void slot_item_clicked(QListWidgetItem *item);
     void slot_add_auth_firend(std::shared_ptr<AuthInfo>);//收到通知对方同意添加好友
     void slot_auth_rsp(std::shared_ptr<AuthRsp>);
signals:
    void sig_loading_contact_user();
    void sig_switch_apply_friend_page();
    void sig_switch_friend_info_page(std::shared_ptr<UserInfo>user_info);

};

#endif // CONTACTUSERLIST_H
