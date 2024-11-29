#include "timerbtn.h"
#include<QDebug>
#include<QMouseEvent>
TimerBtn::TimerBtn(QWidget *parent):QPushButton(parent),_counter(10)
{
    _timer=new QTimer(this);//父容器回收，timer也回收
    connect(_timer,&QTimer::timeout,[this](){
        _counter--;
        if(_counter<=0){
            _timer  ->stop();
            _counter=10;
            this->setText("获取");
            this->setEnabled(true);
            return  ;
        }
        this->setText(QString::number(_counter));//设置计数器文本
    });
}

TimerBtn::~TimerBtn()
{
    _timer->stop();//随父容器一起析构
}

void TimerBtn::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
            // 在这里处理鼠标左键释放事件
            qDebug() << "MyButton was released!";
            this->setEnabled(false); //设置不可用
             this->setText(QString::number(_counter));
            _timer->start(1000);
            emit clicked();
        }
    QPushButton::mouseReleaseEvent(e);//调用基类，确保正常事件处理
}
