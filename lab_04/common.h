#ifndef COMMON_H
#define COMMON_H

#include <QPoint>
#include <QImage>

struct canvas_t
{
    QImage *image;
    QColor *color;
};

void drawQuarters(const QPoint &center, int dx, int dy, canvas_t &canvas);


#endif // COMMON_H
