#ifndef CLICKEDLABEL_H
#define CLICKEDLABEL_H
#include <QLabel>
#include"global.h"
/**
 * @brief The ClickedLabel class
 * 点击标签，鼠标滑动到标签事件
 */
class ClickedLabel : public QLabel
{
    Q_OBJECT
public:
    ClickedLabel(QWidget *parent=nullptr);
    ~ClickedLabel();
    virtual void mousePressEvent(QMouseEvent *ev) override;
    virtual void enterEvent(QEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
    void SetState(QString normal="", QString hover="", QString press="",
                    QString select="", QString select_hover="", QString select_press="");
    bool SetCurState(ClickLbState state);
    void ResetNormalState();
   ClickLbState GetCurState();
   virtual void mouseReleaseEvent(QMouseEvent *ev)override;
private:
   QString _normal;
     QString _normal_hover;
     QString _normal_press;

     QString _selected;
     QString _selected_hover;
     QString _selected_press;

     ClickLbState _curstate;
signals:
     void clicked(QString, ClickLbState);

};

#endif // CLICKEDLABEL_H
