#include "alg.h"
#include <iostream>

float crossProduct(const QPointF &v1, const QPointF &v2)
{
    return v1.x() * v2.y() - v2.x()  * v1.y();
}

float dotProduct(const QPointF &v1, const QPointF &v2)
{
    return v1.x() * v2.x() + v1.y() * v2.y();
}

QPointF perpendicular(const QPointF &vector)
{
    return QPointF(-vector.y(), vector.x());
}

void clip(const QLineF &line, const QVector<QLine> &clipper, int direction, QPainter &painter)
{
    QPointF d(line.p2() - line.p1());
    float tl = 0;
    float th = 1;

    for (auto edge: clipper)
    {
        QPointF n = direction * perpendicular(edge.p2() - edge.p1());
        QPointF w = line.p1() - edge.p1();

        float wScalar = dotProduct(w, n);
        float dScalar = dotProduct(d, n);

        if (dScalar == 0){
            if (wScalar < 0)
                return;
        } else {
            float t = - wScalar / dScalar;
            if (dScalar > 0){
                if (t > 1)
                    return;
                tl = qMax(tl, t);
            }
            else{
                if (t < 0)
                    return;
                th = qMin(th, t);
            }
        }
    }
    if (tl <= th)
        painter.drawLine(line.p1() + d * tl, line.p1() + d * th);
}
