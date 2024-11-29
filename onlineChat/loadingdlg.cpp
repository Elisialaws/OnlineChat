#include "loadingdlg.h"
#include "ui_loadingdlg.h"
#include <QLabel>
#include <QMovie>
#include <QGraphicsOpacityEffect>
#include <QGuiApplication>
#include <QScreen>
/**
 * @brief LoadingDlg::LoadingDlg
 * @param parent
 * 加载聊天用户的界面
 */
LoadingDlg::LoadingDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadingDlg)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint);
       setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明
       // 获取屏幕尺寸
       setFixedSize(parent->size()); // 设置对话框为全屏尺寸

       QMovie *movie = new QMovie(":/res/loading.gif"); // 加载动画的资源文件
       ui->loading_lb->setMovie(movie);
      movie->start();//play animation
}

LoadingDlg::~LoadingDlg()
{
    delete ui;
}
