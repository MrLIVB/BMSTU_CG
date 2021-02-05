#include "drawlabel.h"

DrawLabel::DrawLabel(QWidget *widget) : QLabel(widget) {}

void DrawLabel::setImagePointer(QImage &rimage)
{
    image = &rimage;
}

void DrawLabel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    //painter.drawPixmap(QPoint(0, 0), *pixmap);
    painter.drawImage(QPoint(0,0), *image);
    painter.end();
}
