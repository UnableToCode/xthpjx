#include "bullet.h"

Bullet::Bullet(int x, int y, int w, int h, QPointF drc, const QString& pngFile,
               QWidget* father)
    : Element(x, y, w, h, father) {
  this->setPixmap(QPixmap(pngFile));
  this->direct = drc;
  this->raise();
  this->show();
}

Bullet::~Bullet() {}

void Bullet::fly(QPointF newPos) {}

bool Bullet::fly() {
  int newX = this->x() + direct.x();
  int newY = this->y() + direct.y();
  //子弹出界返回false
  if (newX < AIR_L || newX > AIR_R - this->width() || newY < AIR_T ||
      newY > AIR_D - this->height()) {
    return false;
  }

  //更新位置，返回false
  // qDebug() << newX << " " << newY;
  this->move(newX, newY);

  return true;
}

QPointF Bullet::get_center() {
  return QPointF(this->x() + this->width() / 2, this->y() + this->height() / 2);
}
