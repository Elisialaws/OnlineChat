#include "findfaildlg.h"
#include "ui_findfaildlg.h"

FindFailDlg::FindFailDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindFailDlg)
{
    ui->setupUi(this);
    setWindowTitle("添加");
    setWindowFlags(windowFlags()|Qt::FramelessWindowHint);
    this->setObjectName("FindFailDlg");//方便后续Qss美化
    ui->fail_sure_btn->SetState("normal","hover","press");
     // 获取当前应用程序的路径
     this->setModal(true);
}

FindFailDlg::~FindFailDlg()
{
    delete ui;
}

void FindFailDlg::on_fail_sure_btn_clicked()
{
    this->hide();
}

