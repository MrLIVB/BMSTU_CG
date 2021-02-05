#ifndef ELLIPSEMETHODS_H
#define ELLIPSEMETHODS_H

#include "common.h"

void canonEl(const QPoint &center, int a, int b, canvas_t &canvas);

void paramEl(const QPoint &center, int a, int b, canvas_t &canvas);

void bresenhemEl(const QPoint &center, int a, int b, canvas_t &canvas);

void midPointEl(const QPoint &center, int a, int b, canvas_t &canvas);

void defaultQtDrawEl(const QPoint &center, int a, int b, QPainter &painter);

void defaultQtEl(const QPoint &center, int a, int b, canvas_t &canvas);

#endif // ELLIPSEMETHODS_H
