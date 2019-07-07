#include "enemy.h"
#include "reimu.h"
#include "reimu_bullet.h"

Enemy::Enemy(int x, int y, int w, int h, unsigned life, QPointF drc,
             const QString &gifFile, QWidget *father)
    : Plane(x, y, w, h, gifFile, father) {
  this->life = life;
  this->direct = drc;
  this->bullet_type = STRAIGHT;
}

Enemy::~Enemy() {}

void Enemy::fly(QPointF newPos) {}

bool Enemy::fly() {
  int newX = this->x() + direct.x() * move_speed;
  int newY = this->y() + direct.y() * move_speed;
  //敌机出界返回false
  if (newX < AIR_L || newX > AIR_R - this->width() || newY < AIR_T ||
      newY > AIR_D - this->height()) {
    return false;
  }

  //更新位置，返回true
  // qDebug() << newX << " " << newY;
  this->move(newX, newY);

  return true;
}

bool Enemy::be_hit(list<Reimu_bullet *> &bullet) {
  //只要自机子弹矩形框和敌机矩形框产生图形相交就算作击中
  //击中后生命值-1，删除击中的子弹。
  for (auto i = bullet.begin(); i != bullet.end();) {
    if ((*i)->x() > (this->x() - (*i)->width()) &&
        (*i)->x() < (this->x() + this->width()) && (*i)->y() > this->y() &&
        (*i)->y() < (this->y() + this->height())) {
      // QSound::play(":/sound/damage.wav");
      hit_sound->play();
      if (life > 0) life--;
      delete (*i);
      i = bullet.erase(i);
    } else {
      i++;
    }
  }
  //生命值为0时返回true，否则返回false
  if (this->life == 0) return true;
  return false;
}

QPointF Enemy::get_center() {
  return QPointF(this->x() + this->width() / 2, this->y() + this->height() / 2);
}

void Enemy::set_move_speed(double n) { move_speed = n; }

void Enemy::delete_sound() { delete hit_sound; }

Enemy_L::Enemy_L(int x, int y, QWidget *father)
    : Enemy(x, y, ENEMY_L_W, ENEMY_L_H, ENEMY_L_LIFE, QPointF(0, 4),
            ":/res/enemy/L_butterfly.gif", father) {
  shoot_remain = 0;
  shoot_timer = this->shoot_timeval;
}

Enemy_L::~Enemy_L() {}

void Enemy_L::shoot(list<Enemy_bullet *> &bullet, Reimu *reimu) {
  //如果无剩余射击次数，每一帧计时器--，归零时设置剩余射击次数，随机射击子弹类型并重置计时器
  if (shoot_remain == 0) {
    if (shoot_timer == 0) {
      shoot_remain = this->shoot_times;
      shoot_timer = this->shoot_timeval;
      bullet_type = (Bullet_type)random(BULLET_TYPE_NUM);
      // qDebug() << "bulle type is " << bullet_type;
    } else {
      shoot_timer--;
    }
  } else {
    // int n = random(2);
    QPointF direct;
    //根据射击子弹类型设置方向
    switch (this->bullet_type) {
      case STRAIGHT:
        direct.setX(0);
        direct.setY(7);
        break;
      case SNIPE:
        //自机狙需要利用自机位置计算方向
        direct = reimu->get_center() - this->get_center();
        double length = sqrt(direct.x() * direct.x() + direct.y() * direct.y());
        direct.setX(direct.x() * 7 / length);
        direct.setY(direct.y() * 7 / length);
        break;
    }
    shoot_sound->play();
    // QSound::play(":/sound/enemy_shoot_1.wav");
    //生成新弹幕，加入链表，剩余射击次数--
    Enemy_bullet *new_bullet = new Enemy_bullet_L(
        this->x() + 10, this->y() + 40, direct, this->parentWidget());
    bullet.push_back(new_bullet);
    shoot_remain--;
  }
}

unsigned Enemy_L::get_score() { return this->score; }

unsigned Enemy_L::get_radius() { return ENEMY_L_R; }

void Enemy_L::delete_sound() { delete shoot_sound; }

Enemy_S::Enemy_S(int x, int y, QWidget *father)
    : Enemy(x, y, ENEMY_S_W, ENEMY_S_H, ENEMY_S_LIFE,
            QPointF(-6 + random(13), 6), ":/res/enemy/S_butterfly.gif",
            father) {
  shoot_remain = 0;
  shoot_timer = this->shoot_timeval;
}

Enemy_S::~Enemy_S() {}

void Enemy_S::shoot(list<Enemy_bullet *> &bullet, Reimu *reimu) {
  if (shoot_remain == 0) {
    if (shoot_timer == 0) {
      shoot_remain = this->shoot_times;
      shoot_timer = this->shoot_timeval;
      bullet_type = (Bullet_type)random(BULLET_TYPE_NUM);
      // qDebug() << "bulle type is " << bullet_type;
    } else {
      shoot_timer--;
    }
  } else {
    // int n = random(2);
    QPointF direct;
    switch (this->bullet_type) {
      case STRAIGHT:
        direct.setX(0);
        direct.setY(7);
        break;
      case SNIPE:
        direct = reimu->get_center() - this->get_center();
        double length = sqrt(direct.x() * direct.x() + direct.y() * direct.y());
        direct.setX(direct.x() * 7 / length);
        direct.setY(direct.y() * 7 / length);
        break;
    }
    //    if (n == 0) {
    //      direct.setX(0);
    //      direct.setY(7);
    //    } else {
    //      direct = reimu->get_center() - this->get_center();
    //      direct = reimu->get_center() - this->get_center();
    //      double length = sqrt(direct.x() * direct.x() + direct.y() *
    //      direct.y()); direct.setX(direct.x() * 7 / length);
    //      direct.setY(direct.y() * 7 / length);
    //    }
    shoot_sound->play();
    // QSound::play(":/sound/enemy_shoot_0.wav");
    Enemy_bullet *new_bullet = new Enemy_bullet_S(this->x() + 9, this->y() + 24,
                                                  direct, this->parentWidget());

    bullet.push_back(new_bullet);
    shoot_remain--;
  }
}

unsigned Enemy_S::get_score() { return this->score; }

unsigned Enemy_S::get_radius() { return ENEMY_S_R; }

void Enemy_S::set_shoot_times(unsigned n) { shoot_times = n; }

void Enemy_S::delete_sound() { delete shoot_sound; }

double Enemy::move_speed = 1;
QSound *Enemy::hit_sound = new QSound(":/sound/damage.wav");
unsigned Enemy_L::generatetime = DEFAULT_ENEMY_L_GERNE;
unsigned Enemy_L::shoot_times = 1;
unsigned Enemy_L::shoot_timeval = DEFAULT_ENEMY_L_SHOOT;
unsigned Enemy_L::score = DEFAULT_ENEMY_L_SCORE;
QSound *Enemy_L::shoot_sound = new QSound(":/sound/enemy_shoot_0.wav");
unsigned Enemy_S::generatetime = DEFAULT_ENEMY_S_GERNE;
unsigned Enemy_S::shoot_times = 1;
unsigned Enemy_S::shoot_timeval = DEFAULT_ENEMY_S_SHOOT;
unsigned Enemy_S::score = DEFAULT_ENEMY_S_SCORE;
QSound *Enemy_S::shoot_sound = new QSound(":/sound/enemy_shoot_1.wav");
