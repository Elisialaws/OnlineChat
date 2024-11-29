#ifndef LISTITEMBASE_H
#define LISTITEMBASE_H
/**
 * 所有itemWidget类的基类
 * */
#include <QWidget>
#include "global.h"

class ListItemBase : public QWidget
{
    Q_OBJECT
public:
    explicit ListItemBase(QWidget *parent = nullptr);
    void SetItemType(ListItemType itemType);

    ListItemType GetItemType();
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    ListItemType _itemType;

public slots:

signals:
};

#endif // LISTITEMBASE_H
