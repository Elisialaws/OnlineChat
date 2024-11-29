#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include<QItemDelegate>
#include"global.h"
#include<QTimer>
namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

private slots:
    void on_get_code_clicked();
    void slot_reg_mod_finish(ReqId id,QString res,ErrorCodes err);
    void on_sure_btn_clicked();

    void on_return_btn_clicked();

    void on_cancel_btn_clicked();

private:
    void showTip(QString str,bool ok);
    void initHttpHandlers();
    bool checkUserValid();
    bool checkEmailValid();
    bool checkPassValid();
    bool checkConfirmValid();
    bool checkVarifyValid();
    void AddTipErr(TipErr te,QString tips);
    void DelTipErr(TipErr te);
    void ChangeTipPage();
    QMap<ReqId,std::function<void(const QJsonObject &)>> _handlers;
    Ui::RegisterDialog *ui;
    QMap<TipErr, QString> _tip_errs;
    QTimer *_countdown_timer;
    int _countdown;
signals:
   void sigSwitchLogin();
};
#endif // REGISTERDIALOG_H
