#ifndef VIEW_H
#define VIEW_H

#include <QEvent>
#include <QFont>
#include <QIcon>
#include <QKeyEvent>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QStackedWidget>
#include <QThread>
#include <QTimer>
#include <QWidget>

#include "commom.h"
#include "enemy.h"
#include "reimu.h"

// Menu options number
#define MENU_NUM 3
#define PAUSE_MENU_NUM 3
#define GAMEOVER_MENU_NUM 2
// Rank options number
#define RANK_NUM 4
// number of high score players
#define MAX_HIGH_SCORE 3

//每帧计时
#define FRAME_TIMER 20

//当前界面（用于键盘事件）
enum Stage { LOADING = 0, MENU, RANK, DATA, GAMING, PAUSE, OVER, INPUT_SCORE };
//主菜单选项
enum Menu_pos { GAME_START = 0, PLAYER_DATA, QUIT };
//暂停/疮痍菜单选项
enum Pause_menu_pos { CONTINUE = 0, RESTART, TITLE };

// high score player
struct PlayerScore {
  string name;
  unsigned score;
  PlayerScore(string &n, unsigned s) {
    name = n;
    score = s;
  }
  PlayerScore() {
    name = '\0';
    score = 0;
  }
};

namespace Ui {
class View;
}

class View : public QStackedWidget {
  Q_OBJECT

 public:
  explicit View(QWidget *parent = nullptr);
  ~View();

 protected:
  //重写的计时事件和键盘事件响应
  void timerEvent(QTimerEvent *event);
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);

 private:
  Ui::View *ui;

  // 用于键盘事件响应判断
  Stage nowWindow;

  // loading movie
  QMovie *movie;

  // bgm
  QMediaPlayer bgm;
  QMediaPlaylist bgmlist;
  QSound *over_bgm;

  // 用于各种QLabel的显示隐藏
  vector<QLabel *> menu_list;
  vector<QLabel *> rank_menu_list;
  vector<QLabel *> rank_game_list;
  vector<QLabel *> pause_menu_list;
  vector<QLabel *> life_icon;
  vector<QLabel *> bomb_icon;

  // player high score
  vector<PlayerScore> high_score;

  // for menu choose
  unsigned menu_pos;

  // game rank
  unsigned rank;

  // player info
  unsigned score;
  int life;
  int bomb;

  //自机和自机移动方向
  Reimu *reimu;
  QPointF reimu_diret;

  // Timer ID
  int event_Timer_Id;
  //  QTimer *reimu_move_timer;
  //  QTimer *reimu_shoot_timer;

  // enemy generate timer
  unsigned L_butterfly_generate_timer;
  unsigned S_butterfly_generate_timer;

  //子弹和敌机链表
  list<Reimu_bullet *> reimu_bullet;
  list<Enemy *> enemys;
  list<Enemy_bullet *> enemy_bullets;

  // key press
  bool mKeyShift;
  bool mKeyUp;
  bool mKeyDown;
  bool mKeyLeft;
  bool mKeyRight;
  bool mKeyZ;

  // hide and show label
  void menu_lsit_hide();
  void rank_hide();
  void playdata_hide();
  void menu_lsit_show();
  void rank_show();
  void playdata_show();
  void show_rank();
  void pause_hide();
  void pause_show();
  void over_hide();
  void over_show();
  void input_hide();
  void input_show();

  // load and save high score from file
  void load_score();
  void save_score();

  // for gaming
  void use_bomb();
  unsigned clear_enemy();
  void rank_set();
  void start_game();
  void pause_game();
  void continue_game();
  void ret2title();
  void gameover();
  void reset();
  void quit();

  //每一帧需要调用的方法
  void reimu_move();
  void reimu_shoot();
  void enemy_move();
  void enemy_genrerate();
  void enemy_shoot();
  bool crash();

 private slots:
  void move2menu();
  void move2game();
  void on_yes_clicked();
};

#endif  // VIEW_H
