#ifndef GROUPTIPITEM_H
#define GROUPTIPITEM_H

#include"listitembase.h"
#include"userdata.h"
namespace Ui {
class GroupTipItem;
}
//分组提示条目
class GroupTipItem : public ListItemBase
{
    Q_OBJECT

public:
    explicit GroupTipItem(QWidget *parent = nullptr);
    ~GroupTipItem();
    QSize sizeHint() const override;
   void SetGroupTip(QString str);
private:
    Ui::GroupTipItem *ui;
    QString _tip;
};

#endif // GROUPTIPITEM_H