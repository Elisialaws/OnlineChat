#ifndef STATEWIDGET_H
#define STATEWIDGET_H
#include<QWidget>
#include"global.h"
#include<QLabel>
#include<QEvent>
/**
 * @brief The StateWidget class
 * 实现侧边栏按钮功能，希望点击一个按钮，清空其他按钮的选中状态。
 * 按钮上面能在有新的通知的时候出现红点的图标
 */
class StateWidget:public QWidget
{
    Q_OBJECT
public:
    explicit StateWidget(QWidget *parent=nullptr);
    void SetState(QString normal="", QString hover="", QString press="",
                   QString select="", QString select_hover="", QString select_press="");

     ClickLbState GetCurState();
     void ClearState();

     void SetSelected(bool bselected);
     void AddRedPoint();
     void ShowRedPoint(bool show=true);

 protected:
     void paintEvent(QPaintEvent* event);
     virtual void mousePressEvent(QMouseEvent *ev) override;
     virtual void mouseReleaseEvent(QMouseEvent *ev) override;
     virtual void enterEvent(QEvent* event) override;
     virtual void leaveEvent(QEvent* event) override;

 private:

     QString _normal;
     QString _normal_hover;
     QString _normal_press;

     QString _selected;
     QString _selected_hover;
     QString _selected_press;

     ClickLbState _curstate;
     QLabel * _red_point;

 signals:
     void clicked(void);

 signals:

 public slots:
 };

#endif // STATEWIDGET_H
