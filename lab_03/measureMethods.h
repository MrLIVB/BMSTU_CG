#ifndef MEASUREMETHODS_H
#define MEASUREMETHODS_H

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

int ddaCS(QLine &line);

int brezenhemCS(QLine &line);

int brezenhemIntCS(QLine &line);

int brezenhemAntializedCS(QLine &line);

int wuCS(QLine &line);

#endif // MEASUREMETHODS_H
