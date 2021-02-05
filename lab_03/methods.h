#ifndef METHODS_H
#define METHODS_H

#include <QLine>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <math.h>

struct canvas_t
{
    QImage *image;
    QColor *color;
};

int dda(const QLine &line, canvas_t &canvas);

int brezenhem(const QLine &line, canvas_t &canvas);

int brezenhemInt(const QLine &line, canvas_t &canvas);

int brezenhemAntialized(const QLine &line, canvas_t &canvas);

int defaultQt(const QLine &line, canvas_t &canvas);

int wu(const QLine &line, canvas_t &canvas);

#endif // METHODS_H
