#include "mainwindow.h"
#include <Qtwidgets/QApplication>
#include <QTextCodec>

void SetCodec(QApplication &app)
{
    QTextCodec *codec=QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
