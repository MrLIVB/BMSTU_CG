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

float crossProduct(const QPointF &v1, const QPointF &v2);

void clip(const QLineF &line, const QVector<QLine> &clipper, int direction, QPainter &painter);

#endif // ALG_H
