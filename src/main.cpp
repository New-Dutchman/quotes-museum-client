#include "mainwindow.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Quotes Museum");

    w.show();
    return a.exec();
}

