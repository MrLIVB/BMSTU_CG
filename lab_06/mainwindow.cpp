#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStack>
#include <QColorDialog>
#include <QMouseEvent>
#include <QThread>
#include <profileapi.h>

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    fgColor(defaultFgColor)
{
    ui->setupUi(this);

    ui->drawLabel->setImagePointer(image);

    fgColor = defaultFgColor;
    bgColor = Qt::white;
    boundColor = defaultBoundColor;
    colorLabel(ui->colorlabel, fgColor);

    starts.push_back(0);
    last_closed = 0;

    clearImage();
    imageView();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint point, last;
    int x = event->x() - ui->drawLabel->x();
    int y = event->y() - ui->drawLabel->y();
    point.setX(x);
    point.setY(y);
    if (x < 0 || y < 0 || x >= ui->drawLabel->width() || y >= ui->drawLabel->width())
        return;

    drawType type = none;
    switch (QApplication::keyboardModifiers()) {
    case Qt::ShiftModifier:
        if (vertices.length())
        {
            last = vertices.last();
            if (abs(last.x() - point.x()) > abs(last.y() - point.y()))
                type = horizontal;
            else
                type = vertical;
        }
        break;
    case Qt::ControlModifier:
         type = seed;
    default:
        break;
    }

    addPoint(point, type);
}

void MainWindow::addPoint(QPoint &point, drawType type)
{
    int l = vertices.length();
    if ((type == seed))
    {
        start_seed = point;
        image.setPixelColor(start_seed, fgColor);
        imageView();
        return;
    }

    if (l)
    {
        switch (type)
        {
        case horizontal:
            point.setY(vertices[l - 1].y());
            break;
        case vertical:
            point.setX(vertices[l - 1].x());
            break;
        default:
            break;
        }
    }

    vertices.push_back(point);

    ui->tableWidget->insertRow(l);
    ui->tableWidget->setItem(l, 0, new QTableWidgetItem(QString::number(point.x())));
    ui->tableWidget->setItem(l, 1, new QTableWidgetItem(QString::number(point.y())));

    if (l)
    {
        if (!closed)
            edges.push_back(QLine(closed ? point : vertices[l - 1], point));
        else
            starts.push_back(edges.length());

        canvas_t canvas{&image, &fgColor, &boundColor, &bgColor};
        brezenhem(edges.last(), canvas);
        imageView();

        closed = false;
    }
}

void MainWindow::colorLabel(QLabel *label, QColor &color)
{
    QPalette palette = label->palette();
    palette.setColor(label->backgroundRole(), color);
    label->setAutoFillBackground(true);
    label->setPalette(palette);
}

void MainWindow::clearImage()
{
    image = QImage(721, 721, QImage::Format_ARGB32);
    image.fill(Qt::white);
}

void MainWindow::imageView()
{
    ui->drawLabel->update();
}

void MainWindow::on_colorButton_clicked()
{
    fgColor = QColorDialog::getColor(fgColor, this, "Цвет заливки", QColorDialog::DontUseNativeDialog);
    colorLabel(ui->colorlabel, fgColor);
}

void MainWindow::on_closeButton_clicked()
{
    closed = true;
    int t;

    if (vertices[last_closed] == vertices.last())
    {
        last_closed = vertices.length();
    }
    else
    {
        edges.push_back(QLine(vertices.last(), vertices[last_closed]));
        t = last_closed;
        last_closed = vertices.length();

        ends.push_back(edges.length());

        canvas_t canvas{&image, &fgColor, &boundColor, &bgColor};
        brezenhem(edges.last(), canvas);
        imageView();

        edges.push_back(QLine(vertices[t], vertices[t + 1]));
    }
}

void MainWindow::on_fillButton_clicked()
{
    if (start_seed == QPoint(-1, -1))
        return;

    double time = 0;
    canvas_t canvas{&image, &fgColor, &boundColor, &bgColor};
    time = fill(start_seed, canvas, ui->delaySpinBox->value());
    ui->textBrowser->setText("Потраченное время " + QString::number(time) + "мс");
    imageView();
}

void MainWindow::on_clearButton_clicked()
{
    clearImage();
    imageView();

    closed = false;
    last_closed = 0;

    start_seed = QPoint(-1, -1);

    vertices.clear();
    edges.clear();
    ends.clear();
    starts.clear();
    starts.push_back(0);

    fgColor = defaultFgColor;
    colorLabel(ui->colorlabel, fgColor);
    ui->tableWidget->clearContents();
    ui->tableWidget->model()->removeRows(0, ui->tableWidget->rowCount());
}

int sign(int a)
{
    if (a < 0)
        return -1;
    else if (a > 0)
        return 1;
    else
        return 0;
}


void MainWindow::brezenhem(const QLine &line, canvas_t &canvas)
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

bool isClear(int x, int y, canvas_t &canvas)
{
    return (canvas.image->pixel(x, y) != canvas.boundColor->rgb()) &&
            (canvas.image->pixel(x, y) != canvas.fillColor->rgb());
}

void findNewSeed(QStack<QPoint> *stack, int xl, int xr, int y, canvas_t &canvas)
{
    int x = xl;
    bool flag;
    int xent;

    while(x <= xr)
    {
        flag = false;
        while ((x <= xr) && isClear(x, y, canvas))
        {
            if (!flag)
                flag = true;
            x++;
        }
        if (flag)
        {
            if ((x == xr) && isClear(x, y, canvas))
                stack->push(QPoint(x, y));
            else
                stack->push(QPoint(x - 1, y));
            flag = false;
        }
        xent = x;
        while ((x < xr) && !isClear(x, y, canvas))
            x++;
        if (x == xent)
            x++;
    }
}

double MainWindow::fill(const QPoint &start_seed, canvas_t &canvas, float delay)
{
    LARGE_INTEGER t1, t2;
    LARGE_INTEGER frequency;
    double elapsedTime = 0;
    QueryPerformanceFrequency(&frequency);

    int tick = 0;

    QStack<QPoint> stack;
    QPoint start;
    stack.push(start_seed);
    int xr, xl;

    while(stack.length() > 0)
    {
        QueryPerformanceCounter(&t1);
        start = stack.pop();
        canvas.image->setPixel(start, canvas.fillColor->rgb());
        int tx = start.x();
        int x = start.x() + 1;
        int y = start.y();
        while (canvas.image->pixel(x, y) != canvas.boundColor->rgb())
        {
            Q_ASSERT(x < 800);
            canvas.image->setPixel(x, y, canvas.fillColor->rgb());
            x++;
        }

        xr = x - 1;
        x = tx - 1;
        while (canvas.image->pixel(x, y) != canvas.boundColor->rgb())
        {
            Q_ASSERT(x > 0);
            canvas.image->setPixel(x, y, canvas.fillColor->rgb());
            x--;
        }

        xl = x + 1;
        findNewSeed(&stack, xl, xr, y - 1, canvas);
        findNewSeed(&stack, xl, xr, y + 1, canvas);

        QueryPerformanceCounter(&t2);
        elapsedTime += (t2.QuadPart - t1.QuadPart) * 1000. / frequency.QuadPart;
        if (ui->delayCheckBox->isChecked())
        {
            imageView();
            repaint();
            //QThread::msleep(delay);
            if (tick > delay)
                break;
        }
        tick++;
    }
    return elapsedTime;
}

void MainWindow::on_addButton_clicked()
{
    int x = ui->xspinBox->text().toInt();
    int y = ui->yspinBox->text().toInt();
    QPoint point(x, y);

    if (ui->delayCheckBox->isChecked())
        addPoint(point, seed);
    else
        addPoint(point, none);
}
