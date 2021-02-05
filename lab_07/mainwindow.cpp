#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QColorDialog>
#include <QMouseEvent>

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    pixmap(QPixmap(800, 720)),
    scene(new QGraphicsScene(0,0,800,720))
{
    ui->setupUi(this);

    ui->graphicsView->setScene(scene);

    last.setX(-1);

    rectColor = defaultRectColor;
    lineColor = defaultLineColor;
    resColor = defaultResColor;

    colorLabel(ui->rectClrLabel, rectColor);
    colorLabel(ui->lineClrLabel, lineColor);
    colorLabel(ui->resClrLabel, resColor);

    clearImage();
    imageView();
}

MainWindow::~MainWindow()
{
    delete ui;
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

    if (ui->radioButton->isChecked())
    {
        add_rect(point);
        return;
    }

    switch (QApplication::keyboardModifiers()) {
    case Qt::ShiftModifier:
        add_line(point, shift);
        break;
    default:
        add_line(point, none);
        break;
    }
}

void MainWindow::add_rect(const QPoint &point)
{
    QPainter painter(&pixmap);
    if (last.x() != -1)
    {
        ui->X2RspinBox->setValue(point.x());
        ui->Y2RspinBox->setValue(point.y());

        int x1 = point.x();
        int x2 = last.x();
        int y1 = point.y();
        int y2 = last.y();

        if (x1 > x2)
            qSwap(x1, x2);
        if (y1 > y2)
            qSwap(y1, y2);

        rect = QRect(x1, y1, x2 - x1, y2 - y1);

        painter.setPen(QPen(rectColor, 3));
        painter.drawRect(rect);
        painter.end();

        last.setX(-1);
        imageView();
    }
    else
    {
        last = point;
        ui->X1RspinBox->setValue(point.x());
        ui->Y1RspinBox->setValue(point.y());
    }
}

void MainWindow::add_line(QPoint &point, drawType type)
{
    if (last.x() == -1)
    {
        ui->X1LspinBox->setValue(point.x());
        ui->Y1LspinBox->setValue(point.y());
        last = point;
        return;
    }

    QPainter painter(&pixmap);
    switch (type) {
    case shift:
        if (abs(point.x() - last.x()) > abs(point.y() - last.y()))
            point.setY(last.y());
        else
            point.setX(last.x());
        break;
    default:
        break;
    }

    ui->X2LspinBox->setValue(point.x());
    ui->Y2LspinBox->setValue(point.y());

    lines.push_back(QLine(last, point));
    painter.setPen(lineColor);
    painter.drawLine(last, point);
    painter.end();
    imageView();
    last.setX(-1);
    return;
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
    pixmap.fill(Qt::white);
}

void MainWindow::imageView()
{
    scene->addPixmap(pixmap);
}


void MainWindow::on_clearButton_clicked()
{
    clearImage();
    imageView();

    rectColor = defaultRectColor;
    lineColor = defaultLineColor;
    resColor = defaultResColor;

    colorLabel(ui->rectClrLabel, rectColor);
    colorLabel(ui->lineClrLabel, lineColor);
    colorLabel(ui->resClrLabel, resColor);

    lines.clear();
    last.setX(-1);
}

void MainWindow::on_rectClrButton_clicked()
{
    rectColor = QColorDialog::getColor(rectColor, this, "Pick a rect color", QColorDialog::DontUseNativeDialog);
    colorLabel(ui->rectClrLabel, rectColor);
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

void MainWindow::on_fillButton_clicked()
{
    canvas_t canvas{&pixmap, &resColor};

    for (int i = 0; i < lines.length(); i++)
        cut_off(rect.x(), rect.x() + rect.width(),
                rect.y(), rect.y() + rect.height(),
                lines[i], canvas);
    imageView();
}
