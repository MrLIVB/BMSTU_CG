#include "alg.h"
#include <iostream>
#include <math.h>

void horizon(int x1, int y1, int x2, int y2, QVector<int> &top, QVector<int> &bot, QPainter *painter)
{
    if (x2 < x1)
    {
        qSwap(x1, x2);
        qSwap(y1, y2);
    }

    if (x2 == x1)
    {
        top[x2] = qMax(top[x2], y2);
        bot[x2] = qMin(bot[x2], y2);
        painter->drawLine(x1, y1, x2, y2);
    }
    else
    {
        int x_prev = x1;
        int y_prev = y1;
        float m = (y2 - y1) / (float)(x2 - x1);
        for (int x = x1; x <= x2; x++)
        {
            int y = qRound(m * (x - x1) + y1);
            top[x] = qMax(top[x], y);
            bot[x] = qMin(bot[x], y);
            painter->drawLine(x_prev, y_prev, x, y);
        }
    }
}

int visible(int x, int y, QVector<int> top, QVector<int> bot)
{
    if (y < top[x] && y > bot[x])
        return 0;
    else if (y >= top[x])
        return 1;
    return -1;
}

void processEdge(int x, int y, int &xEdge, int &yEdge, QVector<int> &top, QVector<int> &bot, QPainter *painter)
{
    if (xEdge != -1)
        horizon(xEdge, yEdge, x, y, top, bot, painter);
    xEdge = x;
    yEdge = y;
}

void getIntersection(int x1, int y1, int x2, int y2, QVector<int> horizon, int &xi, int &yi)
{
    xi = x1;
    yi = y1;
    int dx = x2 - x1;
    int dy = y2 - y1;
    int dh = horizon[x2] - horizon[x1];
    float m = dh / (float)dx;
    if (!dx)
    {
        xi = x2;
        yi = horizon[x2];
    }
    else if (y1 == horizon[x1] && y2 == horizon[x2])
    {
        xi = x1;
        yi = y1;
    }
    else
    {
        xi = x1 - qRound(dx * (y1 - horizon[x1]) / (float)(dy - dh));
        yi = qRound((xi - x1) * m + y1);
    }
}

void rotate(float &a, float &b, float teta)
{
    teta *= M_PI / 180.;
    float t = a;
    float c = std::cos(teta);
    float s = std::sin(teta);
    a = c * a - s * b;
    b = c * b + s * t;
}

void transform(float x, float y, float z, angles teta, int &res_x, int &res_y)
{
    float coef = 50;
    int xc = 360;
    int yc = 360;

    rotate(y, z, teta.x);
    rotate(x, z, teta.y);
    rotate(x, y, teta.z);

    res_x = qRound(x * coef + xc);
    res_y = qRound(y * coef + yc);
}

void floatingHorizon(range xr, range zr, angles teta, float(*f)(float, float), QPainter *painter)
{
    QVector<int> top(720, 0);
    QVector<int> bot(720, 720);

    int x_left = -1, y_left = -1, x_right = -1, y_right = -1;
    int xp, yp;
    for (float z = zr.e; z > zr.s; z -= zr.d)
    {
        float y_p = f(xr.s, z);

        transform(xr.s, y_p, z, teta, xp, yp);
        processEdge(xp, yp, x_left, y_left, top, bot, painter);
        int pFlag = visible(xp, yp, top, bot);
        for (float x = xr.s; x < xr.e; x += xr.d)
        {
            int xc = 0, yc = 0;
            int xi = 0, yi = 0;
            y_p = f(x, z);

            transform(x, y_p, z, teta, xc, yc);
            int tFlag = visible(xc, yc, top, bot);
            if (tFlag == pFlag){
                if (tFlag)
                    horizon(xp, yp, xc, yc, top, bot, painter);
            }
            else if (!tFlag)
            {
                if (pFlag)
                    getIntersection(xp, yp, xc, yc, top, xi, yi);
                else
                    getIntersection(xp, yp, xc, yc, bot, xi, yi);
            }
            else if (tFlag == 1)
            {
                getIntersection(xp, yp, xc, yc, top, xi, yi);
                horizon(xp, yp, xi, yi, top, bot, painter);

                if (pFlag)
                {
                    getIntersection(xp, yp, xc, yc, bot, xi, yi);
                    horizon(xp, xp, xi, yi, top, bot, painter);
                }
            }
            else
            {
                getIntersection(xp, yp, xc, yc, bot, xi, yi);
                horizon(xp, yp, xi, yi, top, bot, painter);

                if (pFlag)
                {
                    getIntersection(xp, yp, xc, yc, top, xi, yi);
                    horizon(xp, yp, xi, yi, top, bot, painter);
                }
            }
            pFlag = tFlag;
            xp = xc;
            yp = yc;
        }
        processEdge(xp, yp, x_right, y_right, top, bot, painter);
    }
}
