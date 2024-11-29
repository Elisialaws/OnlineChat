#ifndef CLICKEDONCELABEL_H
#define CLICKEDONCELABEL_H

#include <QLabel>
#include<QMouseEvent>
/**
 * @brief The ClickedOnceLabel class
 * 支持点击一次的label功能。
 */
class ClickedOnceLabel : public QLabel
{
    Q_OBJECT
public:
    ClickedOnceLabel(QWidget *parent=nullptr);
    virtual void mouseReleaseEvent(QMouseEvent *event)override;
signals:
    void clicked(QString);
};

#endif // CLICKEDONCELABEL_H
