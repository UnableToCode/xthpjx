#ifndef ENEMY_H
#define ENEMY_H

#include "enemy_bullet.h"
#include "plane.h"

#define ENEMY_L_W 100
#define ENEMY_L_H 80
#define ENEMY_S_W 52
#define ENEMY_S_H 47
#define ENEMY_S_LIFE 5
#define ENEMY_L_LIFE 25
#define DEFAULT_ENEMY_L_GERNE 90
#define DEFAULT_ENEMY_S_GERNE 30
#define DEFAULT_ENEMY_L_SHOOT 50
#define DEFAULT_ENEMY_S_SHOOT 20
#define DEFAULT_ENEMY_L_SCORE 20
#define DEFAULT_ENEMY_S_SCORE 10
#define ENEMY_S_R 25
#define ENEMY_L_R 30

#define BULLET_TYPE_NUM 2
//子弹类型 STRAIGHT 直线弹， SNIPE自机狙
enum Bullet_type { STRAIGHT = 0, SNIPE };

class Reimu_bullet;
class Reimu;

class Enemy : public Plane {
 public:
  Enemy(int x, int y, int w, int h, unsigned life, QPointF drc,
        const QString& gifFile, QWidget* father);
  virtual ~Enemy();

  void fly(QPointF newPos);                              //不用
  bool fly();                                            //移动
  bool be_hit(list<Reimu_bullet*>&);                     //判断是否被击中
  QPointF get_center();                                  //返回中心坐标
  virtual void shoot(list<Enemy_bullet*>&, Reimu*) = 0;  //射击
  virtual unsigned get_score() = 0;                      //返回击破得分
  virtual unsigned get_radius() = 0;                     //返回碰撞半径

  static void set_move_speed(double n);  //设置移动速度
  static void delete_sound();  //删除中弹声音，退出时调用释放内存

 protected:
  unsigned shoot_remain;    //单次射击剩余子弹
  unsigned shoot_timer;     //射击计时
  Bullet_type bullet_type;  //子弹类型

 private:
  unsigned life;   //剩余生命值
  QPointF direct;  //移动方向

  static QSound* hit_sound;  //击中音效
  static double move_speed;  //移动速度
};

class Enemy_L : public Enemy {
 public:
  Enemy_L(int x, int y, QWidget* father);
  virtual ~Enemy_L();

  void shoot(list<Enemy_bullet*>&, Reimu*);
  unsigned get_score();
  unsigned get_radius();

  static void delete_sound();

  static unsigned generatetime;   //生成计时
  static unsigned shoot_timeval;  //设计间隔
  static unsigned score;          //击破得分

 private:
  static QSound* shoot_sound;   //射击音效
  static unsigned shoot_times;  //射击次数
};

class Enemy_S : public Enemy {
 public:
  Enemy_S(int x, int y, QWidget* father);
  virtual ~Enemy_S();

  void shoot(list<Enemy_bullet*>&, Reimu*);
  unsigned get_score();
  unsigned get_radius();

  static void set_shoot_times(unsigned n);
  static void delete_sound();

  static unsigned generatetime;
  static unsigned shoot_timeval;
  static unsigned score;

 private:
  static QSound* shoot_sound;
  static unsigned shoot_times;
};

#endif  // ENEMY_H
