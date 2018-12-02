#include "ui/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icons/app.png"));

    MainWindow w;
    w.show();
    w.setFixedSize(w.sizeHint());

    return a.exec();
}
