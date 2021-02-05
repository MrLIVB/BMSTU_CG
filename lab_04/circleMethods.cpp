#include "circleMethods.h"
#include <cmath>
#include <QPainter>

void canon(const QPoint &center, int r, canvas_t &canvas)
{
    float x;
    for (float y = r; y > 0; y -= 1. / r)
    {
        x = sqrt(r * r - y * y);
        drawQuarters(center, round(x), round(y), canvas);
    }
}

void param(const QPoint &center, int r, canvas_t &canvas)
{
    float x, y;
    for (float t = 0; t <= 2; t += 1. / r)
    {
        x = qRound(cos(t) * r);
        y = qRound(sin(t) * r);
        drawQuarters(center, x, y, canvas);
    }
}

void bresenhem(const QPoint &center, int r, canvas_t &canvas)
{
    int x = 0;
    int y = r;
    int d = 2 * (1 - r), d1, d2;
    int yk = 0;
    while (y >= yk)
    {
        drawQuarters(center, x, y, canvas);
        if (d < 0)
        {
            d1 = 2 * (d + y) - 1;
            x++;
            if (d1 < 0) // горизонтальный шаг
                d += 2 * x + 1;
            else // диагональный шаг
            {
                y--;
                d += + 2 * (x - y + 1);
            }
        }
        else if (d == 0) // диагональный шаг
        {
            x++;
            y--;
            d += + 2 * (x - y + 1);
        }
        else if (d > 0)
        {
            d2 = 2 * d - 2 * x - 1;
            y--;
            if (d2 < 0) // диагональный шаг
            {
                x++;
                d += + 2 * (x - y + 1);
            }
            else // вертикальный шаг
                d -= 2 * y - 1;
        }
    }
}

void midPoint(const QPoint &center, int r, canvas_t &canvas)
{
    int x = 0;
    int y = r;
    int d = 1 - r;
    while (y > x)
    {
        drawQuarters(center, x, y, canvas);
        drawQuarters(center, y, x, canvas);
        x++;
        if (d < 0)
            d += 2 * x + 1;
        else
        {
            y--;
            d += 2 * (x - y) + 1;
        }
    }
}

void defaultQtDraw(const QPoint &center, int r, QPainter &painter)
{
    painter.drawEllipse(center, r, r);
}

void defaultQt(const QPoint &center, int r, canvas_t &canvas)
{
    QPixmap pixmap = QPixmap::fromImage(*canvas.image);
    QPainter painter(&pixmap);
    painter.setPen(canvas.color->rgb());

    defaultQtDraw(center, r, painter);

    painter.end();
    *canvas.image = pixmap.toImage();
}
