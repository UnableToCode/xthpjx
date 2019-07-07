#ifndef ELEMENT_H
#define ELEMENT_H

#include "commom.h"

class Element : public QLabel {
 public:
  Element(int x, int y, int w, int h, QWidget* father);
  virtual ~Element();
  virtual void fly(QPointF newPos) = 0;  //自机移动，newPos为移动的向量
  virtual bool fly() = 0;  //其他元素移动，其他元素有自己的移动向量成员
};

#endif  // ELEMENT_H
