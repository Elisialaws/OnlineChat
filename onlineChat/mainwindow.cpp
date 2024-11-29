#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"logindialog.h"
#include<QProcess>
#include<QStandardItemModel>
#include<QTableView>
#include"tcpmgr.h"
#define TIMEOUT 1*1000
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _login_dlg=new LoginDialog(this);//dialog对话框作为成员变量，不用每次都new,节省资源
     _login_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_login_dlg);
    //_login_dlg->show();
    //create and register message link
    connect(_login_dlg,&LoginDialog::switchRegister,this,&MainWindow::SlotSwitchReg);
    //连接登录界面忘记密码信号
    connect(_login_dlg, &LoginDialog::switchReset, this, &MainWindow::SlotSwitchReset);
    //连接创建聊天界面信号
    connect(TcpMgr::GetInstance().get(),&TcpMgr::sig_swich_chatdlg,this,&MainWindow::SlotSwitchChat);
    emit TcpMgr::GetInstance()->sig_swich_chatdlg();
}

MainWindow::~MainWindow()
{
    delete ui;
    if(_login_dlg){
        delete _login_dlg;
        _login_dlg=nullptr;
    }
    if(_reg_dlg){
        delete _reg_dlg;
        _reg_dlg=nullptr;
    }

}

void MainWindow::SlotSwitchReg()
{
    _reg_dlg=new RegisterDialog(this);
    //connect连接信号和槽,sender,signal,receiver,槽函数名字
    this->setWindowTitle("HonkaiChat");
    _reg_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
     connect(_reg_dlg,&RegisterDialog::sigSwitchLogin,this,&MainWindow::SlotSwitchLogin);
    setCentralWidget(_reg_dlg);
    _login_dlg->hide();
    _reg_dlg->show();
}

void MainWindow::SlotSwitchLogin()
{
    //create a CentralWidget,设置为中心部件
    _login_dlg=new LoginDialog(this);
    _login_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
       setCentralWidget(_login_dlg);
      _reg_dlg->hide();
       _login_dlg->show();
       //连接登录界面注册信号
       connect(_login_dlg, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);
       //连接登录界面忘记密码信号
      connect(_login_dlg, &LoginDialog::switchReset, this, &MainWindow::SlotSwitchReset);
}

void MainWindow::SlotSwitchReset()
{
    _reset_dlg=new ResetDialog(this);
    _reset_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
      setCentralWidget(_reset_dlg);//中心界面设置为重置界面
     _login_dlg->hide();//系统会自动回收资源
      _reset_dlg->show();
      //注册返回登录信号和槽函数
      connect(_reset_dlg, &ResetDialog::switchLogin, this, &MainWindow::SlotSwitchLogin2);
}
/**
 * @brief MainWindow::SlotSwitchLogin2
 * 重置密码界面->登录界面
 */
void MainWindow::SlotSwitchLogin2()
{
    _login_dlg=new LoginDialog(this);
    _login_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_login_dlg);
    _reset_dlg->hide();
    _login_dlg->show();
    //连接登录界面忘记密码信号
    connect(_login_dlg,&LoginDialog::switchReset,this,&MainWindow::SlotSwitchReset);
    //连接登录界面注册信号
    connect(_login_dlg, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);
}

void MainWindow::SlotSwitchChat()
{
       _chat_dlg = new ChatDialog();
       _chat_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
       setCentralWidget(_chat_dlg);
       _chat_dlg->show();
       _login_dlg->hide();
       this->setMinimumSize(QSize(1050,900));
       this->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
}



