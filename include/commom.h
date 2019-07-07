#ifndef COMMOM_H
#define COMMOM_H

#include <QDebug>
#include <QLabel>
#include <QMovie>
#include <QPixmap>
#include <QPointF>
#include <QSound>
#include <QString>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <list>
#include <string>
#include <vector>

using std::endl;
using std::ifstream;
using std::list;
using std::ofstream;
using std::string;
using std::vector;

#define AIR_L 51
#define AIR_T 24
#define AIR_R 666
#define AIR_D 696

static inline int random(int n) {
  vector<int> arr;
  for (int i = 0; i < n; i++) arr.push_back(i);

  random_shuffle(arr.begin(), arr.end());

  return arr[0];
}
#endif  // COMMOM_H
