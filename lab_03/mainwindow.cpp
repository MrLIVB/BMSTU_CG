#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QColorDialog>
#include <cmath>
#include <iostream>

#include "methods.h"
#include "statistics.h"
#include "stairs.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	fgColor(defaultFgColor),
    bgColor(defaultBgColor),
	scene(new QGraphicsScene(0, 0, 720, 720))
{
	ui->setupUi(this);

	ui->graphicsView->setScene(scene);

    fgColor = defaultFgColor;
    bgColor = defaultBgColor;
    colorLabel(ui->colorLbl, fgColor);
    colorLabel(ui->bgColorLbl, bgColor);

	clearImage();
    imageView();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::colorLabel(QLabel *label, QColor &color)
{
	QPalette palette = label->palette();
	palette.setColor(label->backgroundRole(), color);
	label->setAutoFillBackground(true);
	label->setPalette(palette);
}

void MainWindow::clearImage() {
	image = QImage(721, 721, QImage::Format_ARGB32);
    image.fill(bgColor);
}

void MainWindow::imageView() {
	scene->addPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_fgColorBtn_clicked()
{
    fgColor = QColorDialog::getColor(fgColor, this, "Pick a FG color", QColorDialog::DontUseNativeDialog);
    colorLabel(ui->colorLbl, fgColor);
}

void MainWindow::on_bgToFgBtn_clicked()
{
    fgColor = bgColor;
    colorLabel(ui->colorLbl, fgColor);

}

void MainWindow::on_bgColorBtn_clicked()
{
    bgColor = QColorDialog::getColor(bgColor, this, "Pick a BG color", QColorDialog::DontUseNativeDialog);
    colorLabel(ui->bgColorLbl, bgColor);
}

bool MainWindow::drawLine(QLine &line, canvas_t &canvas)
{
    int ch = ui->methodCBox->currentIndex();
    if (ch == 0)
        dda(line, canvas);
    else if (ch == 1)
        brezenhem(line, canvas);
    else if (ch == 2)
        brezenhemAntialized(line, canvas);
    else if (ch == 3)
        brezenhemInt(line, canvas);
    else if (ch == 4)
        defaultQt(line, canvas);
    else if (ch == 5)
        wu(line, canvas);
    return true;
}

void MainWindow::on_lineBtn_clicked()
{
    float x1 = ui->x1Ent->text().toFloat();
    float x2 = ui->x2Ent->text().toFloat();
    float y1 = ui->y1Ent->text().toFloat();
    float y2 = ui->y2Ent->text().toFloat();
    QLine line(x1, y1, x2, y2);

    canvas_t canvas{&image, &fgColor};

    drawLine(line, canvas);

    imageView();
}

void MainWindow::on_clearBtn_clicked()
{
    clearImage();
    imageView();
}

float toRad(float x) {return x * M_PI / 180;}

void MainWindow::drawPoint(QPoint point)
{
    QPixmap pixmap = QPixmap::fromImage(image);
    QPainter painter(&pixmap);
    painter.setPen(Qt::white);

    painter.drawEllipse(point, 4, 4);

    painter.end();
    image = pixmap.toImage();
}


void MainWindow::on_sunLinesBtn_clicked()
{
    int x2, y2;
    canvas_t canvas{&image, &fgColor};
    QLine line(360, 360, 360, 360);

    int step = ui->degreeStepSpn->text().toInt();
    int length = ui->lengthSpn->text().toInt();

    for (int angle = 0; angle < 360; angle += step) {
        x2 = 360 + round(length * cos(toRad(angle)));
        y2 = 360 - round(length * sin(toRad(angle)));
        line.setP2(QPoint(x2, y2));
        if (!drawLine(line, canvas))
            drawPoint(QPoint(x2, y2));
    }
    imageView();
}

void MainWindow::on_defaultFgBtn_clicked()
{
    fgColor = defaultFgColor;
    colorLabel(ui->colorLbl, fgColor);
}

void MainWindow::on_pushButton_clicked()
{
    Statistics popup;
    popup.setModal(true);
    popup.exec();
}

void MainWindow::on_pushButton_2_clicked()
{
    stairs popup;
    popup.setModal(true);
    popup.exec();
}
