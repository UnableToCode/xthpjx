#ifndef ENEMY_BULLET_H
#define ENEMY_BULLET_H

#include "bullet.h"

#define ENEMY_BULLET_L 81
#define ENEMY_BULLET_S 30
#define DEFAULT_BULLET_L_SCORE 4
#define DEFAULT_BULLET_S_SCORE 2
#define BULLET_L_R 30
#define BULLET_S_R 11

class Enemy_bullet : public Bullet {
 public:
  Enemy_bullet(int x, int y, int w, int h, QPointF drc, const QString& pngFile,
               QWidget* father);
  ~Enemy_bullet();

 public:
  virtual unsigned get_score() = 0;   //返回bomb消弹得分
  virtual unsigned get_radius() = 0;  //返回碰撞半径
};

class Enemy_bullet_L : public Enemy_bullet {
 public:
  Enemy_bullet_L(int x, int y, QPointF drc, QWidget* father);
  ~Enemy_bullet_L();
  unsigned get_score();
  unsigned get_radius();

  static unsigned score;  //消弹得分
};

class Enemy_bullet_S : public Enemy_bullet {
 public:
  Enemy_bullet_S(int x, int y, QPointF drc, QWidget* father);
  ~Enemy_bullet_S();
  unsigned get_score();
  unsigned get_radius();

  static unsigned score;
};

#endif  // ENEMY_BULLET_H
