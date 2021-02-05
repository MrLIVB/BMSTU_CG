#include "ellipseMethods.h"
#include <cmath>
#include <QPainter>

void canonEl(const QPoint &center, int a, int b, canvas_t &canvas)
{
    int x, y;

    int sqra = a * a;
    int sqrb = b * b;

    float ba = float(b) / a;
    int dX = int(sqra / sqrt(sqra + sqrb));
    for (x = 0; x <= dX; x++)
    {
        y = round(sqrt((sqra - x*x)) * ba);
        drawQuarters(center, round(x), round(y), canvas);
    }

    float ab = float(a) / b;
    int dY = round(sqrb / sqrt(sqra + sqrb));
    for (y = 0; y <= dY; y++)
    {
        x = round(sqrt((sqrb - y*y)) * ab);
        drawQuarters(center, round(x), round(y), canvas);
    }
}

void paramEl(const QPoint &center, int a, int b, canvas_t &canvas)
{
    float dt = 1. / qMax(a, b);
    int x, y;
    for (float t = M_PI / 2.; t > -dt / 2.; t -= dt)
    {
        x = round(a * cos(t));
        y = round(b * sin(t));
        drawQuarters(center, x, y, canvas);
    }
}

void bresenhemEl(const QPoint &center, int a, int b, canvas_t &canvas)
{
    int x = 0;
    int y = b;

    int sqra = a * a;
    int sqrb = b * b;
    int d2, d1;

    int d = sqra + sqrb - 2 * sqra * y;
    while (y >= 0)
    {
        drawQuarters(center, x, y, canvas);
        if (d < 0)
        {
            d1 = 2 * (d + sqra * y) - sqra;
            x++;
            if (d1 < 0)
                d += sqrb * (2 * x + 1);
            else
            {
                y--;
                d += 2 * (sqrb * x - sqra * y) + sqra + sqrb;
            }
        }
        else if (d == 0)
        {
            x++;
            y--;
            d += 2 * (sqrb * x - sqra * y) + sqra + sqrb;
        }
        else if (d > 0)
        {
            d2 = 2 * (d - sqrb * x) - sqrb;
            y--;
            if (d2 >= 0)
                d += sqra * (1 - 2 * y);
            else
            {
                x++;
                d += 2 * (sqrb * x - sqra * y) + sqra + sqrb;
            }
        }
    }
}

void midPointEl(const QPoint &center, int a, int b, canvas_t &canvas)
{
    int x = 0;
    int y = b;

    int sqrb = a * a;
    int sqra = b * b;

    int f = sqra + sqrb * (y - 0.5f) * (y - 0.5) - (long long)(sqrb) * sqra;
    int dX = sqrb / sqrt(sqra + sqrb);
    while (x <= dX)
    {
        drawQuarters(center, x, y, canvas);

        x++;
        if (f > 0)
        {
            y--;
            f += -2 * sqrb * y;
        }
        f += sqra * (2 * x + 1);
    }

    f += 0.75f * (sqrb - sqra) - (sqra * x + sqrb * y);
    while (y >= 0)
    {
        drawQuarters(center, x, y, canvas);

        y--;
        if (f < 0)
        {
            x++;
            f += 2 * sqra * x;
        }
        f += sqrb * (1 - 2 * y);
    }
}

void defaultQtDrawEl(const QPoint &center, int a, int b, QPainter &painter)
{
    painter.drawEllipse(center, a, b);
}

void defaultQtEl(const QPoint &center, int a, int b, canvas_t &canvas)
{
    QPixmap pixmap = QPixmap::fromImage(*canvas.image);
    QPainter painter(&pixmap);
    painter.setPen(*canvas.color);

    defaultQtDrawEl(center, a, b, painter);

    painter.end();
    *canvas.image = pixmap.toImage();
}
