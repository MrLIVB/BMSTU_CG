#include "methods.h"

int sign(int a)
{
    if (a < 0)
        return -1;
    else if (a > 0)
        return 1;
    else
        return 0;
}

bool comparePoints(QPoint p1, QPoint p2)
{
    return p1.x() == p2.x() && p1.y() == p2.y();
}

int dda(const QLine &line, canvas_t &canvas)
{
    int dX = line.x2() - line.x1();
    int dY = line.y2() - line.y1();

    int l = qMax(abs(dX), abs(dY));

    if (!l)
    {
        canvas.image->setPixel(line.x1(), line.y1(), canvas.color->rgb());
        return 1;
    }

    double dx = (double) dX / l;
    double dy = (double) dY / l;

    double xf = line.x1();
    double yf = line.y1();
    int x, y;

    for (int i = 0; i <= dX; i++)
    {
        x = qRound(xf);
        y = qRound(yf);
        canvas.image->setPixel(x, y, canvas.color->rgb());
        xf += dx;
        yf += dy;
    }


    if (comparePoints(QPoint(x, y), line.p2()))
        return 1;
    else
        return 0;
}

int brezenhem(const QLine &line, canvas_t &canvas)
{
    if (line.p1() == line.p2())
    {
        canvas.image->setPixel(line.x1(), line.y1(), canvas.color->rgb());
        return 1;
    }

    double x = line.x1(), y = line.y1();
    int dx = line.x2() - line.x1();
    int dy = line.y2() - line.y1();
    int sx = sign(dx);
    int sy = sign(dy);
    dx = abs(dx);
    dy = abs(dy);

    bool swap = dy > dx;
    if (swap)
        qSwap(dx, dy);

    double m = double(dy) / dx;
    double e = m - 0.5;

    for (int i = 0; i <= dx; i++)
    {
        canvas.image->setPixel(x, y, canvas.color->rgb());
        if (e >= 0.0)
        {
            if (swap)
                x += sx;
            else
                y += sy;
            e--;
        }
        if (e < 0)
        {
            if (swap)
                y += sy;
            else
                x += sx;
            e += m;
        }
    }

    if (comparePoints(QPoint(x, y), line.p2()))
        return 1;
    else
        return 0;
}

int brezenhemInt(const QLine &line, canvas_t &canvas)
{
    if (line.p1() == line.p2())
    {
        canvas.image->setPixel(line.x1(), line.y1(), canvas.color->rgb());
        return 1;
    }

    float x = line.x1(), y = line.y1();
    int dx = line.x2() - line.x1();
    int dy = line.y2() - line.y1();
    int sx = sign(dx);
    int sy = sign(dy);
    dx = abs(dx);
    dy = abs(dy);
    int swap = dy > dx;

    if (swap)
        qSwap(dx, dy);

    int e = dy * 2 - dx;

    for (int i = 0; i < dx + 1; i++)
    {
        canvas.image->setPixel(x, y, canvas.color->rgb());
        if (e >= 0)
        {
            if (swap)
                x += sx;
            else
                y += sy;
            e -= 2 * dx;
        }
        if (e < 0)
        {
            if (swap)
                y += sy;
            else
                x += sx;
            e += 2 * dy;
        }
    }
    if (comparePoints(QPoint(x, y), line.p2()))
        return 1;
    else
        return 0;
}

int brezenhemAntialized(const QLine &line, canvas_t &canvas)
{
    if (line.p1() == line.p2())
    {
        canvas.image->setPixel(line.x1(), line.y1(), canvas.color->rgb());
        return 1;
    }

    int i_max = 255;
    int dx = line.x2() - line.x1();
    int dy = line.y2() - line.y1();
    int sx = sign(dx);
    int sy = sign(dy);
    dx = abs(dx);
    dy = abs(dy);
    double x = line.x1();
    double y = line.y1();

    bool swapped = dy > dx;
    if (swapped)
        qSwap(dx, dy);

    double m = 0;
    if (dy)
        m = double(i_max * dy) / dx;

    double e = i_max / 2.0;
    double w = i_max - m;
    QColor color(canvas.color->rgba());
    for (int i = 0; i <= dx; i++)
    {
        color.setAlpha(i_max - e);
        canvas.image->setPixel(x, y, color.rgba());
        if (e <= w)
        {
            if (swapped)
                y += sy;
            else
                x += sx;
            e += m;
        }
        else
        {
            x += sx;
            y += sy;
            e -= w;
        }
    }

    if (comparePoints(QPoint(x, y), line.p2()))
        return 1;
    else
        return 0;
}

int defaultQt(const QLine &line, canvas_t &canvas)
{
    QPixmap pixmap = QPixmap::fromImage(*(canvas.image));
    QPainter painter(&pixmap);
    painter.setPen(*(canvas.color));

    painter.drawLine(line);

    painter.end();
    *(canvas.image) = pixmap.toImage();
    *(canvas.image) = canvas.image->convertToFormat(QImage::Format_ARGB32);

    return 0;
}

int ipart(float a)
{
    return floor(a);
}

float fpart(float a)
{
    return a - floor(a);
}

float rpart(float a)
{
    return 1 - fpart(a);
}

int wu(const QLine &line, canvas_t &canvas)
{
    if (line.p1() == line.p2())
    {
        canvas.image->setPixel(line.x1(), line.y1(), canvas.color->rgb());
        return 1;
    }

    int x1 = line.x1(), y1 = line.y1();
    int x2 = line.x2(), y2 = line.y2();
    int dx = x2 - x1;
    int dy = y2 - y1;

    int swap = abs(dy) > abs(dx);
    if (swap)
    {
        qSwap(x1, y1);
        qSwap(x2, y2);
        qSwap(dx, dy);
    }
    if (x2 < x1)
    {
        qSwap(x1, x2);
        qSwap(y1, y2);
    }

    dx = x2 - x1;
    dy = y2 - y1;

    QColor color(canvas.color->rgba());

    float angle = dx ? (float) dy / dx : 1;
    float y = y1;
    int s;
    for (int x = x1; x <= x2; x++)
    {
        s = sign(y);
        if (swap)
        {
            color.setAlphaF(rpart(y));
            canvas.image->setPixel(ipart(y), x, color.rgba());
            color.setAlphaF(fpart(y));
            canvas.image->setPixel(ipart(y) + s, x, color.rgba());
        }
        else
        {
            color.setAlphaF(rpart(y));
            canvas.image->setPixel(x, ipart(y), color.rgba());
            color.setAlphaF(fpart(y));
            canvas.image->setPixel(x, ipart(y) + s, color.rgba());
        }
        y += angle;
    }

    if (comparePoints(QPoint(x2, y), line.p2()))
        return 1;
    else
        return 0;
}
