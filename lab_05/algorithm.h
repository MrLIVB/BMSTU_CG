#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <QLine>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QPoint>
#include <math.h>


struct canvas_t
{
    QImage *image;
    QColor *fillColor;
    QColor *boundColor;
    QColor *bgColor;
};

int sign(int a);

void brezenhemAntialized(const QLine &line, canvas_t &canvas);

void brezenhem(const QLine &line, canvas_t &canvas);

//void dda(const QLine &line, canvas_t &canvas);

#endif // ALGORITHM_H
