#ifndef CHATITEMBASE_H
#define CHATITEMBASE_H
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include "global.h"
/**
 * @brief The ChatItemBase class
 * 每条聊天信息的类，由spacer,nameLabel,IconLabel,message等组成
 */
class ChatItemBase:public QWidget
{
    Q_OBJECT
public:
    explicit ChatItemBase(ChatRole role,QWidget *parent=nullptr);
    void setUserName(const QString &name);
    void setUserIcon(const QPixmap &icon);
    void setWidget(QWidget *w);
private:
    ChatRole m_role;
    QLabel *m_pNameLabel;
    QLabel *m_pIconLabel;
    QWidget *m_pBubble;
};

#endif // CHATITEMBASE_H
