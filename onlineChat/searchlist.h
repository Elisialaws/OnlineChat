#ifndef SEARCHLIST_H
#define SEARCHLIST_H
#include <QListWidget>
#include <QWheelEvent>
#include <QEvent>
#include <QScrollBar>
#include <QDebug>
#include <QDialog>
#include <memory>
#include "userdata.h"
#include "loadingdlg.h"
#include"findfaildlg.h"
/**
 * @brief The SearchList class
 * 搜索列表页面
 */
class SearchList: public QListWidget
{
    Q_OBJECT
public:
    SearchList(QWidget *parent = nullptr);
    void CloseFindDlg();
    void SetSearchEdit(QWidget* edit);
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
private:
    void waitPending(bool pending = true);
    void addTipItem();
    bool _send_pending;//为true阻塞状态，无法接受服务器回包
    std::shared_ptr<QDialog> _find_dlg;
    QWidget* _search_edit;
    LoadingDlg * _loadingDialog;
private slots:
    void slot_item_clicked(QListWidgetItem *item);
    void slot_user_search(std::shared_ptr<SearchInfo> si);
signals:
    void sig_jump_chat_item(std::shared_ptr<SearchInfo> si);
};

#endif // SEARCHLIST_H
