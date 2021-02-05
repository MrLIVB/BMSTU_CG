#include <QPoint>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <math.h>
#include "common.h"

void drawQuarters(const QPoint &center, int dx, int dy, canvas_t &canvas)
{
    canvas.image->setPixel(center.x() + dx, center.y() + dy, canvas.color->rgb());
    canvas.image->setPixel(center.x() + dx, center.y() - dy, canvas.color->rgb());
    canvas.image->setPixel(center.x() - dx, center.y() + dy, canvas.color->rgb());
    canvas.image->setPixel(center.x() - dx, center.y() - dy, canvas.color->rgb());
}
