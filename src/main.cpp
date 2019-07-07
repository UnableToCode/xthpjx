#include <QApplication>
#include <QDesktopWidget>
#include <QSplashScreen>

#include "view.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  //显示加载界面
  QPixmap welcome(":/res/loading/loading.png");
  QSplashScreen splash(welcome);
  splash.resize(welcome.size());
  splash.show();

  a.processEvents();

  // QDesktopWidget *desk = QApplication::desktop();
  // int wd = desk->width();
  // int ht = desk->height();

  // QTime t;

  // QMovie *movie;
  // movie = new QMovie("/res/loading/loading.gif");
  // QLabel *label = new QLabel("", 0);
  // label->resize(206, 116);
  // label->move((wd - label->width()) / 2, (ht - label->height()) / 2);
  // label->setMovie(movie);
  // label->setScaledContents(true);
  // movie->start();
  // label->show();
  // t.start();
  // while (t.elapsed() < 5000) {
  //     QApplication::processEvents();
  // }
  // label->close();

  a.setWindowIcon(QIcon(":/res/icon/icon.ico"));

  //显示游戏界面
  View game;
  QThread::sleep(3);
  splash.finish(&game);
  game.show();
  // delete desk;
  // delete label;

  return a.exec();
}
