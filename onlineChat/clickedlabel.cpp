#include "clickedlabel.h"
#include<QMouseEvent>
ClickedLabel::ClickedLabel(QWidget *parent):QLabel (parent),_curstate(ClickLbState::Normal)
{
  this->setCursor(Qt::PointingHandCursor);
}

ClickedLabel::~ClickedLabel()
{

}

void ClickedLabel::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        if(_curstate==ClickLbState::Normal){
            qDebug()<<"clicked , change to selected hover: "<< _selected_hover;
             _curstate = ClickLbState::Selected;
            setProperty("state",_selected_hover);//根据property改变css样式
             repolish(this);
             update();
        }else{
            qDebug()<<"clicked , change to normal hover: "<< _normal_hover;
             _curstate = ClickLbState::Normal;
             setProperty("state",_normal_hover);
             repolish(this);
             update();
        }
        return; //在抬起事件里发送信号
    }
    // 其他事件调用基类的mousePressEvent以保证正常的事件处理
       QLabel::mousePressEvent(event);
}

void ClickedLabel::enterEvent(QEvent *event)
{
    if(_curstate==ClickLbState::Normal){
        qDebug()<<"enter , change to normal hover: "<< _normal_hover;
        setProperty("state",_normal_hover);
        repolish(this);
        update();

    }else{
        qDebug()<<"enter , change to selected hover: "<< _selected_hover;
         setProperty("state",_selected_hover);
         repolish(this);
         update();
    }
    QLabel::enterEvent(event);
}

void ClickedLabel::leaveEvent(QEvent *event)
{
    // 在这里处理鼠标悬停离开的逻辑
       if(_curstate == ClickLbState::Normal){
            qDebug()<<"leave , change to normal : "<< _normal;
           setProperty("state",_normal);
           repolish(this);
           update();

       }else{
            qDebug()<<"leave , change to normal hover: "<< _selected;
           setProperty("state",_selected);
           repolish(this);
           update();
       }
       QLabel::leaveEvent(event);
}

void ClickedLabel::SetState(QString normal, QString hover, QString press, QString select, QString select_hover, QString select_press)
{
    _normal = normal;
       _normal_hover = hover;
       _normal_press = press;

       _selected = select;
       _selected_hover = select_hover;
       _selected_press = select_press;

       setProperty("state",normal);
       repolish(this);
}

bool ClickedLabel::SetCurState(ClickLbState state)
{
    _curstate=state;
    if(_curstate==ClickLbState::Normal){
        setProperty("state",_normal);
        repolish(this);
    }else if(_curstate==ClickLbState::Selected){
        setProperty("state",_selected);
        repolish(this);
    }
    return true;
}

ClickLbState ClickedLabel::GetCurState()
{
    return _curstate;
}

void ClickedLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        if(_curstate==ClickLbState::Normal){
            setProperty("state",_normal_hover);
              repolish(this);
              update();
        }else{
            setProperty("state",_selected_hover);
            repolish(this);
            update();
        }
        emit clicked(this->text(),_curstate);
        return;
    }
    QLabel::mouseReleaseEvent(event);//其他事件调用基类的事件
}
void ClickedLabel::ResetNormalState()
{
    _curstate = ClickLbState::Normal;
    setProperty("state", _normal);
    repolish(this);
}
