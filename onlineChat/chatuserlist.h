#ifndef CHATUSERLIST_H
#define CHATUSERLIST_H
#include <QListWidget>
#include <QWheelEvent>
#include <QEvent>
#include <QScrollBar>
#include <QDebug>
/**
 * @brief The ChatUserList class
 * 在聊天记录窗口上滚动设置滚动条等，实现事件过滤器
 * 事件过滤器事件过滤器是一种机制，允许你在事件被发送到目标对象之前拦截并处理该事件。
 * 通过事件过滤器，开发者可以在不修改事件目标对象的情况下，监控并改变事件的处理行为。
在Qt中，事件过滤器是通过QObject::installEventFilter()方法来实现的。基本步骤如下：

创建一个自定义的事件过滤器类，继承自QObject，并重写eventFilter()方法。
在想要监控事件的对象上安装事件过滤器
 */
class ChatUserList: public QListWidget
{
    Q_OBJECT
public:
    ChatUserList(QWidget *parent = nullptr);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
private:
    bool _load_pending;
signals:
    void sig_loading_chat_user();
};

#endif // CHATUSERLIST_H
