#ifndef BULLET_H
#define BULLET_H

#include "element.h"

class Bullet : public Element {
 public:
  Bullet(int x, int y, int w, int h, QPointF drc, const QString& pngFile,
         QWidget* father);
  virtual ~Bullet();
  void fly(QPointF newPos);  // 不用
  bool fly();                //根据direct移动
  QPointF get_center();      //返回中心坐标

 private:
  QPointF direct;  //移动方向
};

#endif  // BULLET_H
