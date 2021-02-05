#include "alg.h"
#include <iostream>

void draw_line(canvas_t &canvas, const QLine &line)
{
    QPainter painter(canvas.pixmap);
    painter.setPen(QPen(*(canvas.resColor), 3));
    painter.drawLine(line);
    painter.end();
}

int setupCodes(int xl, int xr, int yb, int yt, const QPoint &point)
{
    int result = 0;
    if (point.x() < xl)
        result |= 1;
    if (point.x() > xr)
        result |= 1 << 1;
    if (point.y() < yb)
        result |= 1 << 2;
    if (point.y() > yt)
        result |= 1 << 3;
    return result;
}

int find_intersection(int xl, int xr, int yb, int yt, QLine &line, QPoint &p)
{
    float m = 1e30;
    float x, y;

    if (line.x2() - line.x1() != 0){
        m = (line.y2() - line.y1()) / (float)(line.x2() - line.x1());


        if (xl >= p.x()){
            y = m * (xl - p.x()) + p.y();
            if ((yt >= y) && (y >= yb)){
                p.setX(xl);
                p.setY(qRound(y));
                return 0;
            }
        }

        if (xr <= p.x()){
            y = m * (xr - p.x()) + p.y();
            if ((yt >= y) && (y >= yb)){
                p.setX(xr);
                p.setY(qRound(y));
                return 0;
            }
        }
        if (m == 0)
            return -2;
    }

    if (p.y() >= yt){
        x = (1 / m) * (yt - p.y()) + p.x();
        if ((xl <= x) && (x <= xr)){
            p.setX(qRound(x));
            p.setY(yt);
            return 0;
        }
    }
    if (yb < p.y())
        return -1;

    x = (1 / m) * (yb - p.y()) + p.x();
    if ((xl <= x) && (x <= xr)){
        p.setX(qRound(x));
        p.setY(yb);
        return 0;
    }
    return -1;
}

void cut_off(int xl, int xr, int yb, int yt, QLine &line, canvas_t &canvas)
{
    int t1 = setupCodes(xl, xr, yb, yt, line.p1());
    int t2 = setupCodes(xl, xr, yb, yt, line.p2());

    int flag = 0;

    if (!t1 && !t2){
        draw_line(canvas, line);
        return;
    }

    if (t1 & t2)
        return;

    QPoint p;
    QPoint res[2];
    int number = 0;

    if (!t1){
        res[0] = line.p1();
        p = line.p2();
        number = 2;
        flag = find_intersection(xl, xr, yb, yt, line, p);
        if (flag == -2)
            return;
    }
    else if (!t2){
        res[0] = line.p2();
        p = line.p1();
        number = 2;
        flag = find_intersection(xl, xr, yb, yt, line, p);
        if (flag == -2)
            return;
    }

    while (number <= 2){
        if (number)
            res[number - 1] = p;
        number++;
        if (number > 2)
            break;
        if (number == 1)
            p = line.p1();
        else
            p = line.p2();
        flag = find_intersection(xl, xr, yb, yt, line, p);
        if (flag == -2)
            return;
        else if (flag == -1)
            break;
    }

    if (!flag)
        draw_line(canvas, QLine(res[0], res[1]));
}
