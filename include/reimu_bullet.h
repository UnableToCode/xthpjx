#ifndef REIMU_BULLET_H
#define REIMU_BULLET_H

#include "bullet.h"

class Reimu_bullet : public Bullet {
 public:
  Reimu_bullet(int x, int y, QWidget* father);
  ~Reimu_bullet();
};

#endif  // REIMU_BULLET_H
