//***************************************************//
//版本：v1.1
//作者：史前小虫
//主业：http://www.slanl.com
//修改时间：2015年6月14日 14:55:26
//***************************************************//




#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
