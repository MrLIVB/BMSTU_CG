#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include <cmath>
#include <QPainter>

void canonM(int r);

void paramM(int r);

void bresenhemM(int r);

void midPointM(int r);

void defaultQtDrawM(const QPoint &center, int a, int b, QPainter &painter);

void canonElM(int a, int b);

void paramElM(int a, int b);

void bresenhemElM(int a, int b);

void midPointElM(int a, int b);

#endif // MEASUREMENT_H
