#include "ui/mainwindow.h"

#include <QApplication>
#include <QScreen>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  a.setWindowIcon(QIcon(":/icons/app.png"));

  MainWindow w;
  const QScreen * screen = qApp->primaryScreen();
  w.setGeometry(QRect(QPoint(0,0), screen->geometry().size()));
  w.show();

  return a.exec();
}
