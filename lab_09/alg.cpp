#include "alg.h"
#include <iostream>

int sign(float a)
{
    if (a < 0)
        return -1;
    if (a > 0)
        return 1;
    else
        return 0;
}

float crossProduct(const QPointF &v1, const QPointF &v2)
{
    return v1.x() * v2.y() - v2.x()  * v1.y();
}

float dotProduct(const QPointF &v1, const QPointF &v2)
{
    return v1.x() * v2.x() + v1.y() * v2.y();
}

int on_side(const QPoint &check, const QPoint &a1, const QPoint &a2)
{
    return sign(crossProduct(a1 - check, a1 - a2));
}

bool checkIntersection(const QPoint &p1, const QPoint &p2, const QPoint &a1, const QPoint &a2)
{
    return (on_side(p1, a1, a2) * on_side(p2, a1, a2)) <= 0;
}

QPoint intersection(const QPoint &p1, const QPoint &p2, const QPoint &a1, const QPoint &a2)
{
    float det = crossProduct(p2 - p1, a1 - a2);
    double t = crossProduct(a1 - p1, a1 - a2) / det;
    return p1 + (p2 - p1) * t;
}

QVector<QPoint> clip(const QVector<QPoint> &polygon_t, const QVector<QPoint> &clipper_t, int direction)
{
    QVector<QPoint> result;
    QVector<QPoint> polygon = polygon_t;
    QVector<QPoint> clipper = clipper_t;

    QPoint first, start;

    std::cout << direction << std::endl;
    for (int i = 0; i < clipper.length() - 1; i++){
        for (int j = 0; j < polygon.length(); j++){
            if (!j)
                first = polygon[j];
            else if (checkIntersection(start, polygon[j], clipper[i], clipper[i + 1])){
                result.append(intersection(start, polygon[j], clipper[i], clipper[i + 1]));
            }

            start = polygon[j];
            if (on_side(start, clipper[i], clipper[i+1]) * direction <= 0)
                result.append(start);
            std::cout << on_side(start, clipper[i], clipper[i+1]) << std::endl;
        }

        if (!result.isEmpty()){
            if (checkIntersection(start, first, clipper[i], clipper[i + 1]))
                result.append(intersection(start, first, clipper[i], clipper[i + 1]));
        }

        polygon = result;
        result.clear();
    }
    return polygon;
}
