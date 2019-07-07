#include "plane.h"

Plane::Plane(int x, int y, int w, int h, const QString& gifFile,
             QWidget* father)
    : Element(x, y, w, h, father) {
  gif = new QMovie(gifFile);
  this->setMovie(gif);
  gif->start();
  this->raise();
  this->show();
}

Plane::~Plane() {
  delete gif;
  gif = nullptr;
}

void Plane::pause_gif() { gif->setPaused(true); }

void Plane::begin_gif() { gif->setPaused(false); }
