#ifndef CHATUSERWID_H
#define CHATUSERWID_H

#include <QWidget>
#include"listitembase.h"
#include"userdata.h"
namespace Ui {
class ChatUserWid;
}
/**
 * @brief The ChatUserWid class
 * 聊天窗口用户的widget
 */
class ChatUserWid : public ListItemBase
{
    Q_OBJECT

public:
    explicit ChatUserWid(QWidget *parent = nullptr);
    ~ChatUserWid();
    QSize sizeHint() const override;
    void SetInfo(std::shared_ptr<UserInfo> user_info);
    void SetInfo(std::shared_ptr<FriendInfo> friend_info);
    void ShowRedPoint(bool bshow);
    std::shared_ptr<UserInfo> GetUserInfo();
    void updateLastMsg(std::vector<std::shared_ptr<TextChatData>> msgs);//更新消息
private:
    Ui::ChatUserWid *ui;
    std::shared_ptr<UserInfo> _user_info;
};

#endif // CHATUSERWID_H
