#ifndef REIMU_H
#define REIMU_H

#include "enemy.h"
#include "plane.h"
#include "reimu_bullet.h"

#define REIMU_X 340
#define REIMU_Y 620
#define REIMU_W 50
#define REIMU_HE 70
#define REIMU_MOVE_SPEED 10

using std::list;

class Reimu : public Plane {
 public:
  using Element::fly;
  Reimu(QWidget* father);
  ~Reimu();
  void fly(QPointF newPos);  //根据移动向量移动，移动向量又上层键盘事件改变提供
  bool fly();                         //不用
  void shoot(list<Reimu_bullet*>&);   // 射击
  void set_slow();                    //低速
  void set_fast();                    //高速
  void reset();                       //重置
  QPointF get_center();               //返回红点中心坐标
  bool be_hit(list<Enemy_bullet*>&);  //被击中
  bool be_crash(list<Enemy*>&);       //被体术

  bool is_shoot;  //是否在射击

 private:
  QSound* shoot_sound;  //射击音效
  QLabel* center;       //中心红点（判定点）
  bool is_slow;         //是否低速
};

#endif  // REIMU_H
