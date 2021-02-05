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
        add_vertex(point, shift);
        break;
    default:
        add_vertex(point, none);
        break;
    }
}

void MainWindow::add_vertex(QPoint &point, drawType type)
{
    if (ui->radioButton->isChecked()){
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
    }
    else{
        if (polygon.length())
        {
            switch (type) {
            case shift:
                if (abs(point.x() - last.x()) > abs(point.y() - last.y()))
                    point.setY(polygon.last().y());
                else
                    point.setX(polygon.last().x());
                break;
            default:
                break;
            }
        }
        polygon.append(point);
    }

    last = point;
    ui->X1RspinBox->setValue(point.x());
    ui->Y1RspinBox->setValue(point.y());

    displayImage();
    imageView();
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

    painter.setPen(QPen(lineColor, 1));
    for(int i = 0; i < polygon.length() - 1; i++)
        painter.drawLine(polygon[i], polygon[i + 1]);
    if (closed2)
        painter.drawLine(polygon.last(), polygon.first());

    painter.setPen(QPen(clipperColor, 1));
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
    closed2 = false;
    clearImage();
    imageView();

    clipperColor = defaultClipperColor;
    lineColor = defaultLineColor;
    resColor = defaultResColor;

    colorLabel(ui->rectClrLabel, clipperColor);
    colorLabel(ui->lineClrLabel, lineColor);
    colorLabel(ui->resClrLabel, resColor);

    last.setX(-1);

    clipper.clear();
    polygon.clear();
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
    QVector<QPoint> r = clip(polygon, clipper, last_prod);

    QPainter painter(&pixmap);
    painter.setPen(QPen(resColor, 3));
    for (int i = 0; i < r.length() - 1; i++)
        painter.drawLine(r[i], r[i + 1]);
    painter.drawLine(r.last(), r.first());
    painter.end();
    imageView();
}

void MainWindow::on_closeButton_clicked()
{
    if (ui->radioButton_2->isChecked())
    {
        if (closed2){
            QMessageBox::critical(this, "Error", "Многоугольник уже замкнут!");
            return;
        }
        closed2 = true;
        polygon.append(polygon[0]);

        ui->radioButton->setChecked(true);
    }
    else{
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

        ui->radioButton_2->setChecked(true);
    }

    displayImage();
    imageView();
    last.setX(-1);
}

void MainWindow::on_pushButton_clicked()
{
    clipper.clear();
    closed = false;
}

void MainWindow::on_pushButton_2_clicked()
{
    polygon.clear();
    closed2 = false;
}
