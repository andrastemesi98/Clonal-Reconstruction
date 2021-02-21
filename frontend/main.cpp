#include "mainwindow.h"
#include <QScreen>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.move(QGuiApplication::screens().first()->geometry().center() - w.rect().center());
    return a.exec();
}
