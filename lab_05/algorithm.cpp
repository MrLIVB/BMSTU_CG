#include "algorithm.h"

#include <QWidget>

int sign(int a)
{
    if (a < 0)
        return -1;
    else if (a > 0)
        return 1;
    else
        return 0;
}

void brezenhem(const QLine &line, canvas_t &canvas)
{
    if (line.p1() == line.p2())
        canvas.image->setPixel(line.x1(), line.y1(), canvas.boundColor->rgb());

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
        canvas.image->setPixel(x, y, canvas.boundColor->rgb());
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
}


void dda_t(const QLine &line, canvas_t &canvas)
{
    int dX = line.x2() - line.x1();
    int dY = line.y2() - line.y1();

    int l = qMax(abs(dX), abs(dY));

    double dx = (double) dX / l;
    double dy = (double) dY / l;

    double xf = line.x1();
    double yf = line.y1();
    int x, y;
    int ly = qRound(yf);

    x = qRound(xf);
    y = qRound(yf);

    if (canvas.image->pixel(x, y) == canvas.boundColor->rgb())
        canvas.image->setPixel(x + 1, y, canvas.boundColor->rgb());
    else
        canvas.image->setPixel(x, y, canvas.boundColor->rgb());

    for (int i = 0; i <= l; i++)
    {
        x = qRound(xf);
        y = qRound(yf);
        if (y > ly || y < ly)
        {
            ly = y;
            if (canvas.image->pixel(x, y) == canvas.boundColor->rgb())
                canvas.image->setPixel(x + 1, y, canvas.boundColor->rgb());
            else
                canvas.image->setPixel(x, y, canvas.boundColor->rgb());
        }
        xf += dx;
        yf += dy;
    }
}
