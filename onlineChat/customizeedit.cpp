#include "customizeedit.h"


CustomizeEdit::CustomizeEdit(QWidget *parent):QLineEdit(parent),_max_len(0)
{
    connect(this,&QLineEdit::textChanged,this,&CustomizeEdit::limitTextLength);//每次文本变化就触发
}

void CustomizeEdit::SetMaxLength(int maxLen)
{
    _max_len=maxLen;
}

void CustomizeEdit::focusOutEvent(QFocusEvent *event)
{
    // 执行失去焦点时的处理逻辑
    //qDebug() << "CustomizeEdit focusout";
    // 调用基类的focusOutEvent()方法，保证基类的行为得到执行
    QLineEdit::focusOutEvent(event);
    //发送失去焦点得信号
    emit sig_foucus_out();
}
void CustomizeEdit::limitTextLength(QString text) {
    if(_max_len <= 0){
        return;
    }
    QByteArray byteArray = text.toUtf8();

    if (byteArray.size() > _max_len) { //超过限制截取文字
        byteArray = byteArray.left(_max_len);
        this->setText(QString::fromUtf8(byteArray));
    }
}
