#ifndef ALG_H
#define ALG_H

#include <QVector>
#include <QPoint>
#include <QPainter>

struct range{
    float s;
    float e;
    float d;
};

struct angles{
    float x;
    float y;
    float z;
};

void floatingHorizon(range xr, range zr, angles teta, float(*f)(float, float), QPainter *painter);

#endif // ALG_H
