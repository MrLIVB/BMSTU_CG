#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QColorDialog>
#include <QMouseEvent>
#include <QMessageBox>

#include <math.h>
#include <iostream>

using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    pixmap(QPixmap(800, 720)),
    scene(new QGraphicsScene(0,0,800,720))
{
    ui->setupUi(this);

    ui->graphicsView->setScene(scene);

    last.setX(-1);

    setMouseTracking(true);

    clipperColor = defaultClipperColor;
    lineColor = defaultLineColor;
    resColor = defaultResColor;

    colorLabel(ui->rectClrLabel, clipperColor);
    colorLabel(ui->lineClrLabel, lineColor);
    colorLabel(ui->resClrLabel, resColor);

    clearImage();
    imageView();
}

MainWindow::~MainWindow()
{
    delete ui;
}

int sign(int a)
{
    if (a > 0)
        return 1;
    else if (a < 0)
        return -1;
    return 0;
}

float dist(const QLine &line, QPoint &p)
{
    float ch = qAbs(float((line.y2() - line.y1()) * p.x() - (line.x2() - line.x1()) * p.y() + line.x2()*line.y1() - line.y2()*line.x1()));
    float zn = sqrt(line.dy() * line.dy() + line.dx() * line.dx());
    return ch / zn;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint point;

    int x = event->x() - ui->graphicsView->x();
    int y = event->y() - ui->graphicsView->y();
    point.setX(x);
    point.setY(y);
    if (x < 0 || y < 0 || x >= ui->graphicsView->width() || y >= ui->graphicsView->width())
        return;

    switch (QApplication::keyboardModifiers()) {
    case Qt::ShiftModifier:
        if (ui->radioButton->isChecked())
            add_vertex(point, shift);
        else
            add_line(point, shift);
        break;
    case Qt::ControlModifier:
        if (ui->radioButton->isChecked())
            add_vertex(point, none);
        else
            add_line(point, control);
        break;
        break;
    default:
        if (ui->radioButton->isChecked())
            add_vertex(point, none);
        else
            add_line(point, none);
        break;
    }
}

void MainWindow::add_vertex(QPoint &point, drawType type)
{
    if (clipper.length())
    {
        switch (type) {
        case shift:
            if (abs(point.x() - last.x()) > abs(point.y() - last.y()))
                point.setY(clipper.last().y());
            else
                point.setX(clipper.last().x());
            break;
        default:
            break;
        }
    }
    int l = clipper.length();
    if (l == 2)
        last_prod = sign(crossProduct(QPointF(clipper[l - 1] - clipper[l - 2]), QPointF(point - clipper[l - 1])));
    else if (l > 2){
        int tmp = sign(crossProduct(QPointF(clipper[l - 1] - clipper[l - 2]), QPointF(point - clipper[l - 1])));
        if (last_prod == 0)
            last_prod = tmp;
        else if (tmp && (last_prod != tmp)){
            QMessageBox::critical(this, "Error", "Отсекатель должен быть выпуклым!");
            return;
        }
    }

    clipper.append(point);
    last = point;
    //clipper.push(point);
    ui->X1RspinBox->setValue(point.x());
    ui->Y1RspinBox->setValue(point.y());

    displayImage();
    imageView();
}

void MainWindow::add_line(QPoint &point, drawType type)
{
    if (last.x() == -1)
    {
        ui->X1LspinBox->setValue(point.x());
        ui->Y1LspinBox->setValue(point.y());
        last = point;

        if (type != control || clipper.length() < 2)
            return;
        float md, d;
        int mi = 0;
        md = dist(QLine(clipper[0], clipper[1]), point);
        for (int i = 1; i < clipper.length() - 1; i++){
            d = dist(QLine(clipper[i], clipper[i+1]), point);
            if (d < md)
            {
                md = d;
                mi = i;
            }
        }
        parallel = clipper[mi] - clipper[mi + 1];
        return;
    }

    switch (type) {
    case shift:
        if (abs(point.x() - last.x()) > abs(point.y() - last.y()))
            point.setY(last.y());
        else
            point.setX(last.x());
        break;
    case control:
        point.setX(last.x() + parallel.x() / 2);
        point.setY(last.y() + parallel.y() / 2);
        break;
    default:
        break;
    }

    ui->X2LspinBox->setValue(point.x());
    ui->Y2LspinBox->setValue(point.y());

    lines.append(QLine(point, last));
    displayImage();
    imageView();
    last.setX(-1);
}

void MainWindow::colorLabel(QLabel *label, QColor &color)
{
    QPalette palette = label->palette();
    palette.setColor(label->backgroundRole(), color);
    label->setAutoFillBackground(true);
    label->setPalette(palette);
}

void MainWindow::displayImage()
{
    pixmap.fill();

    QPainter painter(&pixmap);

    painter.setPen(QPen(lineColor, 2));
    for(auto line: lines)
        painter.drawLine(line);

    painter.setPen(QPen(clipperColor, 3));
    for(int i = 0; i < clipper.length() - 1; i++)
        painter.drawLine(clipper[i], clipper[i + 1]);
    if (closed)
        painter.drawLine(clipper.last(), clipper.first());
    painter.end();
}

void MainWindow::clearImage()
{
    pixmap.fill(Qt::white);
}

void MainWindow::imageView()
{
    scene->addPixmap(pixmap);
}


void MainWindow::on_clearButton_clicked()
{
    closed = false;
    clearImage();
    imageView();

    clipperColor = defaultClipperColor;
    lineColor = defaultLineColor;
    resColor = defaultResColor;

    colorLabel(ui->rectClrLabel, clipperColor);
    colorLabel(ui->lineClrLabel, lineColor);
    colorLabel(ui->resClrLabel, resColor);

    lines.clear();
    last.setX(-1);

    clipper.clear();
    last_prod = 0;
}

void MainWindow::on_rectClrButton_clicked()
{
    clipperColor = QColorDialog::getColor(clipperColor, this, "Pick a rect color", QColorDialog::DontUseNativeDialog);
    colorLabel(ui->rectClrLabel, clipperColor);
}

void MainWindow::on_lineClrButton_clicked()
{
    lineColor = QColorDialog::getColor(lineColor, this, "Pick a line color", QColorDialog::DontUseNativeDialog);
    colorLabel(ui->lineClrLabel, lineColor);
}

void MainWindow::on_resClrButton_clicked()
{
    resColor = QColorDialog::getColor(resColor, this, "Pick a res color", QColorDialog::DontUseNativeDialog);
    colorLabel(ui->resClrLabel, resColor);
}

void MainWindow::on_radioButton_2_clicked()
{
    if (last.x() != -1)
        ui->radioButton->setChecked(true);
}

void MainWindow::on_radioButton_clicked()
{
    if (last.x() != -1)
        ui->radioButton_2->setChecked(true);
}

void MainWindow::on_clipButton_clicked()
{
    QVector<QLine> edges;
    for (int i = 0; i < clipper.length() - 1; i++)
        edges.append(QLine(clipper[i], clipper[i + 1]));

    QPainter painter(&pixmap);
    painter.setPen(QPen(resColor, 3));
    for (auto line : lines)
        clip(line, edges, last_prod, painter);
    painter.end();
    imageView();
}

void MainWindow::on_closeButton_clicked()
{
    if (closed){
        QMessageBox::critical(this, "Error", "Отсекатель уже замкнут!");
        return;
    }
    int tmp = sign(crossProduct(QPointF(clipper.last() - clipper.first()), QPointF(clipper.last() - clipper[clipper.length() - 2])));
    if (last_prod == 0)
        last_prod = tmp;
    else if (tmp && (last_prod != tmp)){
        QMessageBox::critical(this, "Error", "Отсекатель должен быть выпуклым!");
        return;
    }
    if (last_prod == 0){
        QMessageBox::critical(this, "Error", "Отсекатель выраждается в прямую!");
        return;
    }

    closed = true;
    clipper.append(clipper.first());
    displayImage();
    imageView();
    last.setX(-1);
    ui->radioButton_2->setChecked(true);
}

void MainWindow::on_AddRectBtn_clicked()
{
    int x = ui->X1RspinBox->value();
    int y = ui->Y1RspinBox->value();
    QPoint p(x, y);
    add_vertex(p, none);
}
