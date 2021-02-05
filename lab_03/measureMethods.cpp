#include "measureMethods.h"

int signWod(int a)
{
    if (a < 0)
        return -1;
    else if (a > 0)
        return 1;
    else
        return 0;
}

int ipartWod(float a)
{
    return floor(a);
}

float fpartWod(float a)
{
    return a - floor(a);
}

float rpartWod(float a)
{
    return 1 - fpartWod(a);
}

int ddaCS(QLine &line)
{
    int stairs = 0;

    float dx = line.x2() - line.x1();
    float dy = line.y2() - line.y1();
    float adx = abs(dx), ady = abs(dy);

    bool xb = adx > ady;

    float l = adx > ady ? adx : ady;
    dx = dx / l;
    dy = dy / l;
    float x = line.x1(), y = line.y1();

    for (int i = 0; i < l + 1; i++)
    {
        if (xb)
        {
            if (round(y + dy) > round(y))
                stairs++;
        }
        else
        {
            if (round(x + dx) > round(x))
                stairs++;
        }

        x += dx;
        y += dy;
    }

    return stairs;
}

int brezenhemCS(QLine &line)
{
    int stairs = 0;

    float x = line.x1(), y = line.y1();
    int dx = line.x2() - line.x1();
    int dy = line.y2() - line.y1();
    int sx = signWod(dx);
    int sy = signWod(dy);
    dx = abs(dx);
    dy = abs(dy);
    int swap = dy > dx;

    if (swap)
        qSwap(dx, dy);

    float m = (float)dy / dx;
    float e = m - 0.5;

    for (int i = 0; i < dx + 1; i++)
    {
        if (e >= 0)
        {
            if (swap)
                x += sx;
            else
                y += sy;
            e -= 1;

            stairs++;
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
    return stairs;
}

int brezenhemIntCS(QLine &line)
{
    int stairs = 0;

    float x = line.x1(), y = line.y1();
    int dx = line.x2() - line.x1();
    int dy = line.y2() - line.y1();
    int sx = signWod(dx);
    int sy = signWod(dy);
    dx = abs(dx);
    dy = abs(dy);
    int swap = dy > dx;

    if (swap)
        qSwap(dx, dy);

    int e = dy * 2 - dx;

    for (int i = 0; i < dx + 1; i++)
    {
        if (e >= 0)
        {
            if (swap)
                x += sx;
            else
                y += sy;
            e -= 2 * dx;

            stairs++;
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
    return stairs;
}

int brezenhemAntializedCS(QLine &line)
{
    int stairs = 0;

    int i_max = 255;
    int dx = line.p2().x() - line.p1().x();
    int dy = line.p2().y() - line.p1().y();
    int sx = signWod(dx);
    int sy = signWod(dy);
    dx = qAbs(dx);
    dy = qAbs(dy);
    double x = line.p1().x();
    double y = line.p1().y();

    bool swapped = dy > dx;
    if (swapped)
        qSwap(dx, dy);

    double m = 0;
    if (dy)
        m = double(i_max * dy) / dx;

    double e = i_max / 2.0;
    double w = i_max - m;
    for (int i = 0; i <= dx; ++i)
    {
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

            stairs++;
        }
    }

    return stairs;
}

int wuCS(QLine &line)
{
    int stairs = 0;

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

    float angle = (float) dy / dx;
    float y = y1;
    int s;
    for (int x = x1; x <= x2; x++)
    {
        s = signWod(y);
        if (round(y + angle) > round(y))
            stairs++;

        y += angle;
    }
    return stairs;
}

