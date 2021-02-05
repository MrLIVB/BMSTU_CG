#ifndef ALG_H
#define ALG_H

#include <QVector>
#include <QLine>

float crossProduct(const QPointF &v1, const QPointF &v2);

QVector<QPoint> clip(const QVector<QPoint> &polygon_t, const QVector<QPoint> &clipper_t, int direction);

#endif // ALG_H
