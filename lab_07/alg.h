#ifndef ALG_H
#define ALG_H

#include <QRect>
#include <QPixmap>
#include <QPainter>

struct canvas_t
{
    QPixmap *pixmap;
    QColor *resColor;
};

void cut_off(int xl, int xr, int yb, int yt, QLine &line, canvas_t &canvas);

#endif // ALG_H
