#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      pixmap(QPixmap(7200, 720)),
      scene(new QGraphicsScene(0,0,720,720))
{
    ui->setupUi(this);

    resColor = defaultResColor;
    bgColor = defaultBgColor;

    for (auto f : func_list)
        ui->functionComboBox->addItem(f.name);

    ui->graphicsView->setScene(scene);
    clearImage();
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
    return;
}

void MainWindow::clearImage()
{
    pixmap.fill(Qt::white);
}

void MainWindow::imageView()
{
    scene->addPixmap(pixmap);
}

MainWindow::~MainWindow()
{
    delete ui;
}

range MainWindow::getXRange()
{
    range res;
    res.s = ui->xs->value();
    res.e = ui->xe->value();
    res.d = ui->xd->value();
    return res;
}

range MainWindow::getZRange()
{
    range res;
    res.s = ui->zs->value();
    res.e = ui->ze->value();
    res.d = ui->zd->value();
    return res;
}

void MainWindow::getAngles()
{
    teta.x += ui->tx->value();
    teta.y += ui->ty->value();
    teta.z += ui->tz->value();
}

void MainWindow::on_buildButton_clicked()
{
    clearImage();
    range xr = getXRange();
    range zr = getZRange();

    teta.x = 0;
    teta.y = 0;
    teta.z = 0;

    QPainter painter(&pixmap);
    painter.setPen(resColor);

    floatingHorizon(xr, zr, teta, func_list[ui->functionComboBox->currentIndex()].f, &painter);

    painter.end();
    imageView();
}

void MainWindow::on_rotateButton_clicked()
{
    clearImage();
    range xr = getXRange();
    range zr = getZRange();

    getAngles();

    QPainter painter(&pixmap);
    painter.setPen(resColor);

    floatingHorizon(xr, zr, teta, func_list[ui->functionComboBox->currentIndex()].f, &painter);

    painter.end();
    imageView();
}
