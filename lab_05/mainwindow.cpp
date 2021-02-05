#include <QColorDialog>
#include <QMouseEvent>
#include <QThread>
#include <profileapi.h>
#include <iostream>


#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      fgColor(defaultFgColor),
      scene(new QGraphicsScene(0,0,800,720))
{
    ui->setupUi(this);

    //ui->graphicsView->setScene(scene);
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
    default:
        break;
    }

    addPoint(point, type);
}

void MainWindow::addPoint(QPoint &point, drawType type)
{
    int l = vertices.length();
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

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_colorButton_clicked()
{
    fgColor = QColorDialog::getColor(fgColor, this, "Цвет заливки", QColorDialog::DontUseNativeDialog);
    colorLabel(ui->colorlabel, fgColor);
}

void MainWindow::on_addButton_clicked()
{
    int x = ui->xspinBox->text().toInt();
    int y = ui->yspinBox->text().toInt();
    QPoint point(x, y);

    addPoint(point, none);
}

void MainWindow::on_clearButton_clicked()
{
    clearImage();
    imageView();

    closed = false;
    last_closed = 0;
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

int defaultQt(const QLine &line, canvas_t &canvas)
{
    QPixmap pixmap = QPixmap::fromImage(*(canvas.image));
    QPainter painter(&pixmap);
    painter.setPen(*(canvas.boundColor));

    painter.drawLine(line);

    painter.end();
    *(canvas.image) = pixmap.toImage();
    *(canvas.image) = canvas.image->convertToFormat(QImage::Format_ARGB32);

    return 0;
}


int check(const QLine &line, const QPoint &point)
{
    int sx = sign(line.x2() - line.x1());
    int sy = sign(line.y2() - line.y1());
    if (sx * line.x2() > sx * point.x() && sy * line.y2() > sy * point.y())
        return 1;
    return 0;
}

void MainWindow::intersection(const QLine &line, canvas_t &canvas, int extr)
{
    int l = line.x2() - line.x1();
    int m = line.y2() - line.y1();
    int sy = sign(m);

    int y = line.y1();
    float dx = abs((float)l / m) * sign(l);
    int ix;
    float x = line.x1();

    m = abs(m);
    if (!extr)
        m -= 1;
    for (int i = 0; i <= m; i++)
    {
        ix = qRound(x);
        if (canvas.image->pixel(ix, y) == canvas.boundColor->rgb())
            canvas.image->setPixel(ix + 1, y, canvas.boundColor->rgb());
        else
            canvas.image->setPixel(ix, y, canvas.boundColor->rgb());
        x += dx;
        y += sy;
    }
}

void MainWindow::dda(const QLine &line, canvas_t &canvas, int extr)
{
    int dX = line.x2() - line.x1();
    int dY = line.y2() - line.y1();

    int l = qMax(abs(dX), abs(dY));

    double dx = (double) dX / l;
    double dy = (double) dY / l;

    double xf = line.x1();
    double yf = line.y1();
    int x, y;
    int ly = -1;

    if (!extr)
        l -= 1;
    for (int i = 0; i <= l; i++)
    {
        x = qRound(xf);
        y = qRound(yf);
        if (y != ly)
        {
            ly = y;
            if (canvas.image->pixel(x, y) == canvas.boundColor->rgb()) //  проверка для острых углов
                canvas.image->setPixel(x + 1, y, canvas.boundColor->rgb());
            else
                canvas.image->setPixel(x, y, canvas.boundColor->rgb());
        }
        xf += dx;
        yf += dy;
    }
}

int extr_check(QLine &l1,  QLine &l2)
{
    if (sign(l2.y1() - l1.y1()) == sign(l2.y1() - l2.y2()))
        return 1;
    else
        return 0;
}

double MainWindow::fillFlag(QVector<QLine> edges, canvas_t &canvas, float delay)
{
    LARGE_INTEGER t1, t2;
    LARGE_INTEGER frequency;
    double elapsedTime = 0;
    QueryPerformanceFrequency(&frequency);

    int xmax = edges[0].x1(), ymax = edges[0].y1();
    int xmin = edges[0].x1(), ymin = edges[0].y1();
    for (int i = 1; i < edges.length(); i++)
    {
        if (edges[i].x1()  > xmax)
            xmax = edges[i].x1();
        if (edges[i].y1()  > ymax)
            ymax = edges[i].y1();
        if (edges[i].x1()  < xmin)
            xmin = edges[i].x1();
        if (edges[i].y1() < ymin)
            ymin = edges[i].y1();
    }

    for (int j = 0; j < ends.length(); j++)
        for (int i = starts[j]; i < ends[j]; i++)
            intersection(edges[i], canvas, extr_check(edges[i], edges[i + 1]));

    bool flag;
    for (int y = ymax; y >= ymin; y--)
    {
        QueryPerformanceCounter(&t1);
        flag = false;
        for (int x = xmin; x <= xmax; x++)
        {
            if (canvas.image->pixel(x,y) == canvas.boundColor->rgb())
                flag = !flag;

            if (flag)
                canvas.image->setPixel(x, y, canvas.fillColor->rgb());
            else
                canvas.image->setPixel(x, y, canvas.bgColor->rgb());
        }
        QueryPerformanceCounter(&t2);
        elapsedTime += (t2.QuadPart - t1.QuadPart) * 1000. / frequency.QuadPart;
        if (ui->delayCheckBox->isChecked() && delay)
        {
            imageView();
            repaint();
            QThread::msleep(delay);
        }
    }
    return elapsedTime;
}


void MainWindow::on_fillButton_clicked()
{
    double time = 0;
    clearImage();
    canvas_t canvas{&image, &fgColor, &boundColor, &bgColor};
    time = fillFlag(edges, canvas, ui->delaySpinBox->value());
    ui->textBrowser->setText("Потраченное время " + QString::number(time) + "мс");
    imageView();
}
