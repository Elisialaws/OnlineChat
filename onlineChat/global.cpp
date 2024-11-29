#include"global.h"
QString gate_url_prefix="";
std::function<void(QWidget*)>repolish=[](QWidget *w){
    w->style()->unpolish(w);//去除旧的属性
    w->style()->polish(w);//添加样式
};
std::function<QString(QString)>xorString=[](QString input){
    QString result=input;
    int length=input.length()%255;
    for(int i=0;i<length;++i){
        //对每个字符异或,假设 都为ASCII
        result[i]=QChar(static_cast<ushort>(input[i].unicode()^static_cast<ushort>(length)));
    }
    return result;
};
