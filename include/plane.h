#ifndef PLANE_H
#define PLANE_H

#include "element.h"

class Plane : public Element {
 public:
  Plane(int x, int y, int w, int h, const QString& gifFile, QWidget* father);
  virtual ~Plane();
  void pause_gif();  //暂停gif播放
  void begin_gif();  //继续gif播放

 private:
  QMovie* gif;  //飞机动图载入
};

#endif  // PLANE_H
