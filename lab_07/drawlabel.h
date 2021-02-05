#ifndef DRAWLABEL_H
#define DRAWLABEL_H

#include <QLabel>
#include <QPixmap>
#include <QPainter>

class DrawLabel : public QLabel
{
    Q_OBJECT

public:
    DrawLabel(QWidget *widget);
    void setImagePointer(QImage &rimage);
    void paintEvent(QPaintEvent *event);

private:
    QImage *image;
};

#endif // DRAWLABEL_H
