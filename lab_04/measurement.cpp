#include "measurement.h"
#include <cmath>
#include <QPainter>

void canonM(int r)
{
    float x;
    for (float y = r; y > 0; y -= 1. / r)
    {
        x = round(sqrt(r * r - y * y));
        round(y);
    }
}

void paramM(int r)
{
    float x, y;
    for (float t = 0; t <= 2; t += 1. / r)
    {
        x = round(cos(t) * r);
        y = round(sin(t) * r);
    }
}

void bresenhemM(int r)
{
    int x = 0;
    int y = r;
    int d = 2 * (1 - r), d1, d2;
    int yk = 0;
    while (y >= yk)
    {
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

void midPointM(int r)
{
    int x = 0;
    int y = r;
    int d = 1 - r;
    while (y > x)
    {
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

void defaultQtDrawM(const QPoint &center, int a, int b, QPainter &painter)
{
    painter.drawEllipse(center, a, b);
}

void canonElM(int a, int b)
{
    int x, y;

    int sqra = a * a;
    int sqrb = b * b;

    float ba = float(b) / a;
    int dX = int(sqra / sqrt(sqra + sqrb));
    for (x = 0; x <= dX; x++)
    {
        y = round(sqrt((sqra - x*x)) * ba);
    }

    float ab = float(a) / b;
    int dY = round(sqrb / sqrt(sqra + sqrb));
    for (y = 0; y <= dY; y++)
    {
        x = round(sqrt((sqrb - y*y)) * ab);
    }
}

void paramElM(int a, int b)
{
    float dt = 1. / qMax(a, b);
    int x, y;
    for (float t = M_PI / 2.; t > -dt / 2.; t -= dt)
    {
        x = round(a * cos(t));
        y = round(b * sin(t));
    }
}

void bresenhemElM(int a, int b)
{
    int x = 0;
    int y = b;

    int sqra = a * a;
    int sqrb = b * b;
    int d2, d1;

    int d = sqra + sqrb - 2 * sqra * y;
    while (y >= 0)
    {
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

void midPointElM(int a, int b)
{
    int x = 0;
    int y = b;

    int sqrb = a * a;
    int sqra = b * b;

    int f = sqra + sqrb * (y - 0.5f) * (y - 0.5) - (long long)(sqrb) * sqra;
    int dX = sqrb / sqrt(sqra + sqrb);
    while (x <= dX)
    {
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
        y--;
        if (f < 0)
        {
            x++;
            f += 2 * sqra * x;
        }
        f += sqrb * (1 - 2 * y);
    }
}
