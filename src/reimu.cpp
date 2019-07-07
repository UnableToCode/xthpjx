#include "reimu.h"

Reimu::Reimu(QWidget* father)
    : Plane(REIMU_X, REIMU_Y, REIMU_W, REIMU_HE, ":/res/player/reimu.gif",
            father) {
  center = new QLabel(this);
  center->setPixmap(QPixmap(":/res/player/center.png"));
  center->setGeometry(11, 14, 25, 25);
  center->raise();
  center->hide();
  shoot_sound = new QSound(":/sound/player_shoot.wav");
  is_slow = false;
  is_shoot = false;
  update();
}

Reimu::~Reimu() {
  delete center;
  delete shoot_sound;
}

void Reimu::fly(QPointF newPos) {
  int length = REIMU_MOVE_SPEED;
  //低速时候移动距离减半
  if (is_slow) {
    newPos.setX(newPos.x() / 2);
    newPos.setY(newPos.y() / 2);
    length /= 2;
  }
  double x = newPos.x();
  double y = newPos.y();
  //斜对角反向移动时需要重新计算向量保证移动速度不变，保证向量的模为移动距离。
  if (x != 0 && y != 0) {
    newPos.setX(newPos.x() * length / sqrt(pow(x, 2) + pow(y, 2)));
    newPos.setY(newPos.y() * length / sqrt(pow(x, 2) + pow(y, 2)));
    // qDebug() << newPos.x() << " " << newPos.y();
  }

  int newX = this->x() + newPos.x();
  int newY = this->y() + newPos.y();

  //到达边界时防止越界
  newX = newX >= AIR_L ? newX : AIR_L;
  newX = newX <= AIR_R - this->width() ? newX : AIR_R - this->width();
  newY = newY >= AIR_T ? newY : AIR_T;
  newY = newY <= AIR_D - this->height() ? newY : AIR_D - this->height();
  // qDebug() << newX << " " << newY;

  //更新位置
  this->move(newX, newY);
}

bool Reimu::fly() { return false; }

void Reimu::shoot(list<Reimu_bullet*>& reimu_bullet) {
  //如果正在射击，播放射击音效并创建子弹加入链表
  if (is_shoot) {
    shoot_sound->play();
    // QSound::play(":/sound/player_shoot.wav");
    Reimu_bullet* new_bullet =
        new Reimu_bullet(this->x() + 17, this->y(), this->parentWidget());
    reimu_bullet.push_back(new_bullet);
  }
}

void Reimu::set_slow() {
  //设置低速模式，显示判定点
  is_slow = true;
  center->raise();
  center->show();
}

void Reimu::set_fast() {
  //设置高速模式，隐藏判定点
  is_slow = false;
  center->hide();
}

void Reimu::reset() {
  //重置
  this->move(REIMU_X, REIMU_Y);
  set_fast();
  is_shoot = false;
}

QPointF Reimu::get_center() { return QPointF(this->x() + 18, this->y() + 20); }

bool Reimu::be_hit(list<Enemy_bullet*>& bullet) {
  //与每个敌机子弹计算距离，小于等于判定半径时判定为被击中
  for (auto& i : bullet) {
    if (pow(this->get_center().x() - i->get_center().x(), 2) +
            pow(this->get_center().y() - i->get_center().y(), 2) <=
        pow(i->get_radius(), 2)) {
      return true;
    }
  }
  return false;
}

bool Reimu::be_crash(list<Enemy*>& enemys) {
  //与每个敌机计算距离，小于等于判定半径时判定为被体术
  for (auto& i : enemys) {
    if (pow(this->get_center().x() - i->get_center().x(), 2) +
            pow(this->get_center().y() - i->get_center().y(), 2) <=
        pow(i->get_radius(), 2)) {
      return true;
    }
  }
  return false;
}
