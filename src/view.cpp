#include "view.h"
#include "ui_view.h"

View::View(QWidget *parent) : QStackedWidget(parent), ui(new Ui::View) {
  ui->setupUi(this);
  //设置窗口大小和标题，禁用大小调整和窗口调整按钮，设置可以鼠标点击获得焦点
  this->setWindowFlags(Qt::WindowFlags() & ~Qt::WindowMaximizeButtonHint);
  this->setFixedSize(this->width(), this->height());
  this->setWindowTitle(tr("（锤） 车万 （锤）"));
  this->setFocusPolicy(Qt::StrongFocus);

  //加载界面右下角动图
  movie = new QMovie(":/res/loading/loading.gif");
  ui->loading_2->setMovie(movie);
  movie->start();
  ui->loading_2->show();

  //设置当前界面为Loading
  nowWindow = LOADING;
  this->setCurrentIndex(LOADING);

  // 9.5s后切换到主菜单
  QTimer::singleShot(9500, this, SLOT(move2menu()));

  //将Label图标添加到链表
  menu_list.push_back(ui->game_start_1);
  menu_list.push_back(ui->player_data_1);
  menu_list.push_back(ui->quit_1);
  rank_menu_list.push_back(ui->easy);
  rank_menu_list.push_back(ui->normal);
  rank_menu_list.push_back(ui->hard);
  rank_menu_list.push_back(ui->lunatic);
  rank_game_list.push_back(ui->rank_e);
  rank_game_list.push_back(ui->rank_n);
  rank_game_list.push_back(ui->rank_h);
  rank_game_list.push_back(ui->rank_l);
  pause_menu_list.push_back(ui->continue_1);
  pause_menu_list.push_back(ui->restart_1);
  pause_menu_list.push_back(ui->return_title_1);
  life_icon.push_back(ui->life_0);
  life_icon.push_back(ui->life_1);
  bomb_icon.push_back(ui->bomb_0);
  bomb_icon.push_back(ui->bomb_1);

  //设置高分画面字体和颜色
  QFont font("STHupo", 25, 50);
  QPalette pa;
  pa.setColor(QPalette::WindowText, QColor(0xff, 0x00, 0xff));

  ui->score_0->setFont(font);
  ui->score_0->setPalette(pa);
  ui->score_1->setFont(font);
  ui->score_1->setPalette(pa);
  ui->score_2->setFont(font);
  ui->score_2->setPalette(pa);

  //初始化相关参数
  menu_pos = 0;
  rank = 0;

  score = 0;
  life = 0;
  bomb = 0;

  mKeyShift = false;
  mKeyUp = false;
  mKeyDown = false;
  mKeyLeft = false;
  mKeyRight = false;
  mKeyZ = false;

  // 加载 bgm
  bgmlist.addMedia(QUrl("qrc:///sound/start.wav"));
  bgmlist.addMedia(QUrl("qrc:///sound/bgm.wav"));
  over_bgm = new QSound("qrc:///sound/gameover.wav");

  bgmlist.setCurrentIndex(0);
  bgmlist.setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
  bgm.setPlaylist(&bgmlist);
  bgm.setVolume(70);
  bgm.play();

  //隐藏相关图标
  menu_lsit_hide();
  playdata_hide();
  rank_hide();
  pause_hide();
  over_hide();
  input_hide();

  //加载最高分
  load_score();
}

View::~View() { delete ui; }

void View::timerEvent(QTimerEvent *event) {
  //采用帧同步事件，每20ms一帧，每帧先判断是否GG，如果疮痍，则gameover,如果没有，则进行自机射击，移动，敌机生成，移动，射击
  if (event->timerId() == event_Timer_Id) {
    if (crash()) {
      gameover();
      return;
    }
    reimu_shoot();
    reimu_move();
    enemy_genrerate();
    enemy_move();
    enemy_shoot();
  }
}

void View::keyPressEvent(QKeyEvent *event) {
  //不同界面下键盘按下响应事件
  if (nowWindow == MENU) {
    //主菜单，上下切换选项，Z键选择
    if (event->key() == Qt::Key_Down && !event->isAutoRepeat()) {
      QSound::play("qrc:///sound/select.wav");
      menu_list[menu_pos]->hide();
      menu_pos = menu_pos == MENU_NUM - 1 ? menu_pos : menu_pos + 1;
      menu_list[menu_pos]->show();
    } else if (event->key() == Qt::Key_Up && !event->isAutoRepeat()) {
      QSound::play("qrc:///sound/select.wav");
      menu_list[menu_pos]->hide();
      menu_pos = menu_pos == 0 ? menu_pos : menu_pos - 1;
      menu_list[menu_pos]->show();
    } else if (event->key() == Qt::Key_X) {
      QSound::play("qrc:///sound/check.wav");
      menu_list[menu_pos]->hide();
      menu_pos = QUIT;
      menu_list[menu_pos]->show();
    } else if (event->key() == Qt::Key_Z) {
      QSound::play("qrc:///sound/check.wav");
      switch (menu_pos) {
        case GAME_START:
          nowWindow = RANK;
          menu_lsit_hide();
          rank = 0;
          rank_show();
          break;
        case PLAYER_DATA:
          nowWindow = DATA;
          menu_lsit_hide();
          playdata_show();
          break;
        case QUIT:
          quit();
          break;
      }
    }
  } else if (nowWindow == RANK) {
    //难度选择见面，左右切换，z选择，x返回菜单界面
    if (event->key() == Qt::Key_Left && !event->isAutoRepeat()) {
      QSound::play("qrc:///sound/select.wav");
      rank_menu_list[rank]->hide();
      rank = rank == 0 ? rank : rank - 1;
      rank_menu_list[rank]->show();
    } else if (event->key() == Qt::Key_Right && !event->isAutoRepeat()) {
      QSound::play("qrc:///sound/select.wav");
      rank_menu_list[rank]->hide();
      rank = rank == RANK_NUM - 1 ? rank : rank + 1;
      rank_menu_list[rank]->show();
    } else if (event->key() == Qt::Key_X) {
      QSound::play("qrc:///sound/check.wav");
      rank_hide();
      menu_lsit_show();
      nowWindow = MENU;
    } else if (event->key() == Qt::Key_Z) {
      QSound::play("qrc:///sound/check.wav");
      rank_set();
      start_game();
    }
  }

  else if (nowWindow == DATA) {
    //玩家数据界面,Z键返回
    if (event->key() == Qt::Key_X && !event->isAutoRepeat()) {
      QSound::play("qrc:///sound/check.wav");
      playdata_hide();
      menu_lsit_show();
      nowWindow = MENU;
    }

  } else if (nowWindow == GAMING) {
    //游戏界面，esc暂停，按下shift自己进入低速模式，按下上下左右键改变自己移动向量控制方向，按下Z开始射击，X使用炸弹
    switch (event->key()) {
      case Qt::Key_Escape:
        QSound::play("qrc:///sound/check.wav");
        pause_game();
        break;
      case Qt::Key_Shift:
        if (!event->isAutoRepeat() && !mKeyShift) {
          mKeyShift = true;
          reimu->set_slow();
        }
        break;
      case Qt::Key_Up:
        if (!event->isAutoRepeat() && !mKeyUp) {
          mKeyUp = true;
          reimu_diret += QPointF(0, -REIMU_MOVE_SPEED);
        }
        break;
      case Qt::Key_Down:
        if (!event->isAutoRepeat() && !mKeyDown) {
          mKeyDown = true;
          reimu_diret += QPointF(0, REIMU_MOVE_SPEED);
        }
        break;
      case Qt::Key_Left:
        if (!event->isAutoRepeat() && !mKeyLeft) {
          mKeyLeft = true;
          reimu_diret += QPointF(-REIMU_MOVE_SPEED, 0);
        }
        break;
      case Qt::Key_Right:
        if (!event->isAutoRepeat() && !mKeyRight) {
          mKeyRight = true;
          reimu_diret += QPointF(REIMU_MOVE_SPEED, 0);
        }
        break;
      case Qt::Key_Z:
        if (!event->isAutoRepeat() && !mKeyZ) {
          mKeyZ = true;
          reimu->is_shoot = true;
        }
        break;
      case Qt::Key_X:
        use_bomb();
        break;
    }
  } else if (nowWindow == PAUSE) {
    //暂停菜单， 上下键切换选项，X返回，Z键选择
    if (event->key() == Qt::Key_Down && !event->isAutoRepeat()) {
      QSound::play("qrc:///sound/select.wav");
      pause_menu_list[menu_pos]->hide();
      menu_pos = menu_pos == MENU_NUM - 1 ? menu_pos : menu_pos + 1;
      pause_menu_list[menu_pos]->show();
    } else if (event->key() == Qt::Key_Up && !event->isAutoRepeat()) {
      QSound::play("qrc:///sound/select.wav");
      pause_menu_list[menu_pos]->hide();
      menu_pos = menu_pos == 0 ? menu_pos : menu_pos - 1;
      pause_menu_list[menu_pos]->show();
    } else if (event->key() == Qt::Key_X) {
      QSound::play("qrc:///sound/check.wav");
      continue_game();
    } else if (event->key() == Qt::Key_Z) {
      QSound::play("qrc:///sound/check.wav");
      switch (menu_pos) {
        case CONTINUE:
          continue_game();
          break;
        case RESTART:
          reset();
          break;
        case TITLE:
          ret2title();
          break;
      }
    }
  } else if (nowWindow == OVER) {
    //暂停菜单， 上下键切换选项，Z键选择
    if (event->key() == Qt::Key_Down && !event->isAutoRepeat()) {
      QSound::play("qrc:///sound/select.wav");
      pause_menu_list[menu_pos]->hide();
      menu_pos = menu_pos == MENU_NUM - 1 ? menu_pos : menu_pos + 1;
      pause_menu_list[menu_pos]->show();
    } else if (event->key() == Qt::Key_Up && !event->isAutoRepeat()) {
      QSound::play("qrc:///sound/select.wav");
      pause_menu_list[menu_pos]->hide();
      menu_pos = menu_pos == 1 ? menu_pos : menu_pos - 1;
      pause_menu_list[menu_pos]->show();
    } else if (event->key() == Qt::Key_Z) {
      QSound::play("qrc:///sound/check.wav");
      over_bgm->stop();
      switch (menu_pos) {
        case RESTART:
          reset();
          break;
        case TITLE:
          ret2title();
          break;
      }
    }
  }
}

void View::keyReleaseEvent(QKeyEvent *event) {
  //键盘松开时间响应，针对游戏时的按键响应
  if (nowWindow == GAMING) {
    //松开上下左右设置方向向量，松开shift进入高速模式，松开z停止射击。
    switch (event->key()) {
      case Qt::Key_Shift:
        if (!event->isAutoRepeat() && mKeyShift) {
          mKeyShift = false;
          reimu->set_fast();
        }
        break;
      case Qt::Key_Up:
        if (!event->isAutoRepeat() && mKeyUp) {
          mKeyUp = false;
          reimu_diret -= QPointF(0, -REIMU_MOVE_SPEED);
        }
        break;
      case Qt::Key_Down:
        if (!event->isAutoRepeat() && mKeyDown) {
          mKeyDown = false;
          reimu_diret -= QPointF(0, REIMU_MOVE_SPEED);
        }
        break;
      case Qt::Key_Left:
        if (!event->isAutoRepeat() && mKeyLeft) {
          mKeyLeft = false;
          reimu_diret -= QPointF(-REIMU_MOVE_SPEED, 0);
        }
        break;
      case Qt::Key_Right:
        if (!event->isAutoRepeat() && mKeyRight) {
          mKeyRight = false;
          reimu_diret -= QPointF(REIMU_MOVE_SPEED, 0);
        }
        break;
      case Qt::Key_Z:
        if (!event->isAutoRepeat() && mKeyZ) {
          mKeyZ = false;
          reimu->is_shoot = false;
        }
        break;
    }
  }
}

void View::rank_set() {
  //难度设置，按难度调整比例设置敌机生成时间射击间隔射击次数和分数
  double rate = 0.75 + 0.25 * rank;
  // qDebug() << "rank = " << rank;
  // Enemy::set_move_speed(rate);
  // qDebug() << "move speed = " << rate;
  Enemy_S::set_shoot_times(1 + rank);
  Enemy_L::generatetime = DEFAULT_ENEMY_L_GERNE / rate;
  Enemy_S::generatetime = DEFAULT_ENEMY_S_GERNE / rate;
  Enemy_L::shoot_timeval = DEFAULT_ENEMY_L_SHOOT / rate;
  Enemy_S::shoot_timeval = DEFAULT_ENEMY_S_SHOOT / rate;

  double score_rate = 0.5 + 0.5 * rank;
  Enemy_L::score = DEFAULT_ENEMY_L_SCORE * score_rate;
  Enemy_S::score = DEFAULT_ENEMY_S_SCORE * score_rate;
  Enemy_bullet_S::score = DEFAULT_BULLET_S_SCORE * score_rate;
  Enemy_bullet_L::score = DEFAULT_BULLET_L_SCORE * score_rate;
}

void View::start_game() {
  //游戏开始
  rank_hide();
  nowWindow = LOADING;
  this->setCurrentIndex(0);
  bgmlist.setCurrentIndex(1);

  // loading 菜单9.5s后切换到游戏画面
  QTimer::singleShot(9500, this, SLOT(move2game()));

  reimu = new Reimu(this->widget(2));
  reset();
}

void View::pause_game() {
  //暂停游戏
  nowWindow = PAUSE;
  bgm.pause();
  reimu->pause_gif();
  killTimer(event_Timer_Id);
  for (auto &i : enemys) {
    i->pause_gif();
  }
  menu_pos = CONTINUE;
  pause_show();
}

void View::continue_game() {
  //继续游戏
  nowWindow = GAMING;
  bgm.play();
  reimu->begin_gif();
  event_Timer_Id = startTimer(FRAME_TIMER);
  for (auto &i : enemys) {
    i->begin_gif();
  }
  pause_hide();
}

void View::ret2title() {
  //返回标题菜单
  nowWindow = LOADING;
  bgm.stop();
  this->setCurrentIndex(0);
  bgmlist.setCurrentIndex(0);
  bgm.play();

  // loading 菜单9.5s后切换到游戏标题画面
  QTimer::singleShot(9500, this, SLOT(move2menu()));

  delete reimu;

  for (auto &i : reimu_bullet) {
    delete i;
  }
  reimu_bullet.clear();
  clear_enemy();
}

void View::gameover() {
  //疮痍
  killTimer(event_Timer_Id);
  nowWindow = OVER;
  reimu->pause_gif();
  for (auto &i : enemys) {
    i->pause_gif();
  }
  bgm.stop();
  over_bgm->play();
  menu_pos = RESTART;

  over_show();

  //判断是否破纪录
  if (score > high_score[2].score) {
    nowWindow = INPUT_SCORE;
    input_show();
  }
}

void View::reset() {
  //重置游戏，
  bgm.stop();
  bgm.play();
  reimu->reset();
  reimu_diret = QPointF(0, 0);
  mKeyZ = false;
  mKeyUp = false;
  mKeyDown = false;
  mKeyLeft = false;
  mKeyRight = false;
  mKeyShift = false;

  for (auto &i : reimu_bullet) {
    delete i;
  }
  reimu_bullet.clear();

  clear_enemy();

  score = 0;
  life = 2;
  bomb = 2;

  for (auto &i : life_icon) {
    i->show();
  }
  for (auto &i : bomb_icon) {
    i->show();
  }

  reimu->begin_gif();
  L_butterfly_generate_timer = Enemy_L::generatetime;
  S_butterfly_generate_timer = Enemy_S::generatetime;

  event_Timer_Id = startTimer(FRAME_TIMER);

  ui->high_score->display(QString::number(high_score[0].score)
                              .sprintf("%05d", high_score[0].score));
  ui->score->display(QString::number(score).sprintf("%05d", score));

  nowWindow = GAMING;
  over_hide();
  pause_hide();
}

void View::menu_lsit_hide() {
  ui->game_start_0->hide();
  ui->player_data_0->hide();
  ui->quit_0->hide();
  for (auto &i : menu_list) {
    i->hide();
  }
  //  ui->game_start_1->hide();
  //  ui->player_data_1->hide();
  //  ui->quit_1->hide();
}

void View::rank_hide() {
  ui->rank_select->hide();
  for (auto &i : rank_menu_list) {
    i->hide();
  }
  //  ui->easy->hide();
  //  ui->normal->hide();
  //  ui->hard->hide();
  //  ui->lunatic->hide();
}

void View::playdata_hide() {
  ui->player_data->hide();
  ui->score_0->hide();
  ui->score_1->hide();
  ui->score_2->hide();
}

void View::menu_lsit_show() {
  ui->game_start_0->show();
  ui->player_data_0->show();
  ui->quit_0->show();
  menu_list[menu_pos]->show();
}

void View::rank_show() {
  ui->rank_select->show();
  rank_menu_list[rank]->show();
}

void View::playdata_show() {
  ui->player_data->show();
  QString temp =
      QString::fromStdString(high_score[0].name) + '\t' +
      QString::number(high_score[0].score).sprintf("%05d", high_score[0].score);
  ui->score_0->setText(temp);
  temp =
      QString::fromStdString(high_score[1].name) + '\t' +
      QString::number(high_score[1].score).sprintf("%05d", high_score[1].score);
  ui->score_1->setText(temp);
  temp =
      QString::fromStdString(high_score[2].name) + '\t' +
      QString::number(high_score[2].score).sprintf("%05d", high_score[2].score);
  ui->score_2->setText(temp);

  ui->score_0->show();
  ui->score_1->show();
  ui->score_2->show();
}

void View::show_rank() {
  for (auto &i : rank_game_list) {
    i->hide();
  }
  rank_game_list[rank]->show();
}

void View::pause_hide() {
  ui->mask->hide();
  ui->game_pause->hide();
  ui->continue_0->hide();
  ui->restart_0->hide();
  ui->return_title_0->hide();
  for (auto &i : pause_menu_list) {
    i->hide();
  }
}

void View::pause_show() {
  ui->mask->raise();
  ui->game_pause->raise();
  ui->continue_0->raise();
  ui->restart_0->raise();
  ui->return_title_0->raise();
  for (auto &i : pause_menu_list) {
    i->raise();
  }
  ui->mask->show();
  ui->game_pause->show();
  ui->continue_0->show();
  ui->restart_0->show();
  ui->return_title_0->show();
  pause_menu_list[menu_pos]->show();
}

void View::over_hide() {
  ui->mask->hide();
  ui->game_over->hide();
  ui->continue_0->hide();
  ui->restart_0->hide();
  ui->return_title_0->hide();
  for (auto &i : pause_menu_list) {
    i->hide();
  }
}

void View::over_show() {
  ui->mask->raise();
  ui->game_over->raise();
  ui->continue_0->raise();
  ui->restart_0->raise();
  ui->return_title_0->raise();
  for (auto &i : pause_menu_list) {
    i->raise();
  };
  ui->mask->show();
  ui->game_over->show();
  ui->continue_0->show();
  ui->restart_0->show();
  ui->return_title_0->show();
  pause_menu_list[menu_pos]->show();
}

void View::input_hide() {
  ui->high_score_plain->close();
  ui->player_name->close();
  ui->yes->close();
}

void View::input_show() {
  ui->high_score_plain->raise();
  ui->player_name->raise();
  ui->yes->raise();
  ui->high_score_plain->show();
  ui->player_name->clear();
  ui->player_name->show();
  ui->yes->show();
}

void View::load_score() {
  ifstream fin;
  high_score.resize(MAX_HIGH_SCORE, PlayerScore());
  fin.open("score.txt");
  if (!fin.is_open()) {
    qDebug() << "open file fail!";
    ofstream fout("score.txt");
    if (!fout) qDebug() << "open file fail!";
    for (size_t i = 0; i < MAX_HIGH_SCORE; i++) {
      fout << high_score[i].name << "\t" << high_score[i].score << endl;
    }
    fout.close();
    return;
  }

  for (size_t i = 0; i < MAX_HIGH_SCORE; i++) {
    fin >> high_score[i].name >> high_score[i].score;
  }
  fin.close();
}

void View::save_score() {
  ofstream fout("score.txt");
  for (size_t i = 0; i < MAX_HIGH_SCORE; i++) {
    fout << high_score[i].name << "\t" << high_score[i].score << endl;
  }
  fout.close();
}

void View::use_bomb() {
  //如果有炸弹，播放音效，清屏，加分，减bomb数，隐藏图标
  if (bomb > 0) {
    QSound::play(":/sound/bomb.wav");
    bomb--;
    this->score += clear_enemy();
    ui->score->display(QString::number(score).sprintf("%05d", score));
    bomb_icon[bomb]->hide();
  }
}

unsigned View::clear_enemy() {
  //清理所有敌机和子弹，返回分数
  unsigned score = 0;
  // clear enemy and enemy bullet
  for (auto &i : enemy_bullets) {
    score += i->get_score();
    delete i;
  }
  enemy_bullets.clear();

  for (auto &i : enemys) {
    score += i->get_score();
    delete i;
  }
  enemys.clear();

  return score;
}

void View::move2menu() {
  nowWindow = MENU;
  this->setCurrentIndex(1);
  menu_pos = GAME_START;
  menu_lsit_show();
}

void View::move2game() {
  nowWindow = GAMING;
  this->setCurrentIndex(2);
  show_rank();
}

void View::reimu_move() {
  //移动自机
  reimu->fly(reimu_diret);
}

void View::reimu_shoot() {
  //射出新子弹，所有子弹移动，出界则消除
  reimu->shoot(reimu_bullet);
  for (auto i = reimu_bullet.begin(); i != reimu_bullet.end();) {
    if ((*i)->fly())
      i++;
    else {
      delete (*i);
      i = reimu_bullet.erase(i);
    }
  }
}

void View::enemy_move() {
  //敌人移动，如果出界，则消除
  for (auto i = enemys.begin(); i != enemys.end();) {
    if ((*i)->fly())
      i++;
    else {
      delete (*i);
      i = enemys.erase(i);
    }
  }
}

void View::enemy_genrerate() {
  //大小蝴蝶生成计时器每帧--，为0则生成对应敌机加入链表，重置计时器
  if (this->currentIndex() == 2) {
    if (L_butterfly_generate_timer == 0) {
      Enemy *new_enemy_L = new Enemy_L(
          AIR_L + random(AIR_R - AIR_L - ENEMY_L_W), AIR_T, this->widget(2));
      enemys.push_back(new_enemy_L);
      L_butterfly_generate_timer = Enemy_L::generatetime;
    } else {
      L_butterfly_generate_timer--;
    }
    if (S_butterfly_generate_timer == 0) {
      Enemy *new_enemy_S = new Enemy_S(
          AIR_L + random(AIR_R - AIR_L - ENEMY_S_W), AIR_T, this->widget(2));
      enemys.push_back(new_enemy_S);
      S_butterfly_generate_timer = Enemy_S::generatetime;
    } else {
      S_butterfly_generate_timer--;
    }
  }
}

void View::enemy_shoot() {
  //敌机射击，所有敌机子弹移动，出界则消除
  for (auto &i : enemys) {
    i->shoot(enemy_bullets, reimu);
  }
  for (auto i = enemy_bullets.begin(); i != enemy_bullets.end();) {
    if ((*i)->fly())
      i++;
    else {
      delete (*i);
      i = enemy_bullets.erase(i);
    }
  }
}

bool View::crash() {
  // 被弹幕击中或者被敌机体术，则生命值--，清屏，重置自机，隐藏生命图标，重置炸弹数量和图标，如果疮痍返回true
  if (reimu->be_hit(enemy_bullets) || reimu->be_crash(enemys)) {
    QSound::play(":/sound/dead.wav");
    life--;
    if (life < 0) {
      return true;
    }
    clear_enemy();
    reimu->reset();
    if (mKeyZ) reimu->is_shoot = true;
    life_icon[life]->hide();

    bomb = 2;
    for (auto &i : bomb_icon) {
      i->show();
    }
  }
  //所有敌人和自己体积判断击中，如果敌机被击破，则播放音效，加分数，删除敌机
  for (auto i = enemys.begin(); i != enemys.end();) {
    if ((*i)->be_hit(reimu_bullet)) {
      QSound::play(":/sound/enemy_destory.wav");
      score += (*i)->get_score();
      ui->score->display(QString::number(score).sprintf("%05d", score));
      delete (*i);
      i = enemys.erase(i);
    } else {
      i++;
    }
  }
  return false;
}

void View::quit() {
  //退出函数
  Enemy::delete_sound();
  Enemy_L::delete_sound();
  Enemy_S::delete_sound();
  bgm.stop();
  bgmlist.clear();
  delete over_bgm;
  delete movie;
  delete ui;
  menu_list.clear();
  rank_menu_list.clear();
  rank_game_list.clear();
  pause_menu_list.clear();
  high_score.clear();
  life_icon.clear();
  bomb_icon.clear();

  exit(0);
}

void View::on_yes_clicked() {
  //打破记录时候出现的按钮槽函数
  QSound::play(":/sound/check.wav");
  string now_player = ui->player_name->text().toStdString();
  size_t i = 0;
  while (score < high_score[i].score) i++;
  high_score.insert(high_score.begin() + i, PlayerScore(now_player, score));
  high_score.pop_back();
  save_score();
  input_hide();
  nowWindow = OVER;
  this->setFocusProxy(this);
}
