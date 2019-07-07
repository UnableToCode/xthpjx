#include "enemy_bullet.h"

Enemy_bullet::Enemy_bullet(int x, int y, int w, int h, QPointF drc,
                           const QString& pngFile, QWidget* father)
    : Bullet(x, y, w, h, drc, pngFile, father) {}

Enemy_bullet::~Enemy_bullet() {}

Enemy_bullet_L::Enemy_bullet_L(int x, int y, QPointF drc, QWidget* father)
    : Enemy_bullet(x, y, ENEMY_BULLET_L, ENEMY_BULLET_L, drc,
                   ":/res/enemy/bullet_L.png", father) {}

Enemy_bullet_L::~Enemy_bullet_L() {}

unsigned Enemy_bullet_L::get_score() { return this->score; }

unsigned Enemy_bullet_L::get_radius() { return BULLET_L_R; }

Enemy_bullet_S::Enemy_bullet_S(int x, int y, QPointF drc, QWidget* father)
    : Enemy_bullet(x, y, ENEMY_BULLET_S, ENEMY_BULLET_S, drc,
                   ":/res/enemy/bullet_S.png", father) {}

Enemy_bullet_S::~Enemy_bullet_S() {}

unsigned Enemy_bullet_S::get_score() { return this->score; }

unsigned Enemy_bullet_S::get_radius() { return BULLET_S_R; }

unsigned Enemy_bullet_L::score = DEFAULT_BULLET_L_SCORE;
unsigned Enemy_bullet_S::score = DEFAULT_BULLET_S_SCORE;
