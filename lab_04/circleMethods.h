#ifndef CIRCLEMETHODS_H
#define CIRCLEMETHODS_H

#include "common.h"

void drawQuarters(const QPoint &center, int dx, int dy, canvas_t &canvas);

void canon(const QPoint &center, int r, canvas_t &canvas);

void param(const QPoint &center, int r, canvas_t &canvas);

void bresenhem(const QPoint &center, int r, canvas_t &canvas);

void midPoint(const QPoint &center, int r, canvas_t &canvas);

void defaultQtDraw(const QPoint &center, int r, QPainter &painter);

void defaultQt(const QPoint &center, int r, canvas_t &canvas);

#endif // CIRCLEMETHODS_H
