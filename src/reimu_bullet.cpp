#include "reimu_bullet.h"

Reimu_bullet::Reimu_bullet(int x, int y, QWidget* father)
    : Bullet(x, y, 16, 73, QPointF(0, -50), ":/res/player/bullet_with_tail.png",
             father) {}

Reimu_bullet::~Reimu_bullet() {}
