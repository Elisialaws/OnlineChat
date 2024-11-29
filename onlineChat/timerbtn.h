#ifndef TIMERBTN_H
#define TIMERBTN_H
#include<QPushButton>
#include<QTimer>

class TimerBtn:public QPushButton
{
public:
    TimerBtn(QWidget *parent=nullptr);
    ~TimerBtn();
    void mouseReleaseEvent(QMouseEvent *e)override;
private:
    QTimer *_timer;
    int _counter;//计数器
};

#endif // TIMERBTN_H
