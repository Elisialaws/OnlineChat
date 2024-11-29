#include "mainwindow.h"
#include"global.h"
#include <QApplication>
#include<QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); //将环境变量和参数传递给QApplication对象
    //a.exec()，轮询机制，有消息就处理
    QFile qss(":/style/stylesheet.qss");
    if(qss.open(QFile::ReadOnly)){
        qDebug("open success");
        QString style=QLatin1String(qss.readAll());
        a.setStyleSheet((style));
    }else{
        qDebug("open failed");
    }
    QString fileName="config.ini";
    QString app_path=QCoreApplication::applicationDirPath();//application的 dir path
    QString config_path = QDir::toNativeSeparators(app_path +
                            QDir::separator() + fileName);

    QSettings settings(config_path, QSettings::IniFormat);
    QString gate_host = settings.value("GateServer/host").toString();
    QString gate_port = settings.value("GateServer/port").toString();
    gate_url_prefix = "http://"+gate_host+":"+gate_port;
    MainWindow w;
    w.show();
    return a.exec();
}
