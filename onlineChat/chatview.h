#ifndef CHATVIEW_H
#define CHATVIEW_H
#include<QWidget>
#include<QHBoxLayout>
#include<QScrollArea>
/**
 * @brief The ChatView class
 * 每条聊天记录的视图
 */
class ChatView:public QWidget
{
    Q_OBJECT
public:
    ChatView(QWidget *parent=nullptr);
    void appendChatItem(QWidget *item);//添加聊天记录,尾插
    void prependChatItem(QWidget *item);                //头插
    void insertChatItem(QWidget *before, QWidget *item);//中间插
    void removeAllItem();
protected:
     bool eventFilter(QObject *o, QEvent *e) override;
     void paintEvent(QPaintEvent *event) override;
private slots:
     void onVScrollBarMoved(int min, int max);//滑动聊天记录
private:
     void initStyleSheet();
private:
       //QWidget *m_pCenterWidget;
       QVBoxLayout *m_pVl;
       QScrollArea *m_pScrollArea;
       bool isAppended;//是否正在添加数据
};

#endif // CHATVIEW_H
