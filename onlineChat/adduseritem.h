#ifndef ADDUSERITEM_H
#define ADDUSERITEM_H

#include <QWidget>
#include"listitembase.h"
namespace Ui {
class AddUserItem;
}

class AddUserItem : public ListItemBase
{
    Q_OBJECT

public:
    explicit AddUserItem(QWidget *parent = nullptr);
    ~AddUserItem();
    QSize sizeHint()const override;//提供了一个组件（widget）的推荐尺寸。
private:
    Ui::AddUserItem *ui;
};

#endif // ADDUSERITEM_H
