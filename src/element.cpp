#include "element.h"

Element::Element(int x, int y, int w, int h, QWidget* father) : QLabel(father) {
  setGeometry(QRect(x, y, w, h));
}

Element::~Element() {}
