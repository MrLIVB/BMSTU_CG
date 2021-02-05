#include <QColorDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "statistics.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
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

void MainWindow::clearImage()
{
    image = QImage(721, 721, QImage::Format_ARGB32);
    image.fill(bgColor);
}

void MainWindow::imageView() {
    scene->addPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_clearBtn_clicked()
{
    clearImage();
    imageView();
}

void MainWindow::on_defaultFgBtn_clicked()
{
    fgColor = defaultFgColor;
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

void MainWindow::on_fgColorBtn_clicked()
{
    fgColor = QColorDialog::getColor(fgColor, this, "Pick a FG color", QColorDialog::DontUseNativeDialog);
    colorLabel(ui->colorLbl, fgColor);
}

void MainWindow::drawCircle(const QPoint &center, int r, canvas_t &canvas)
{
    int ch = ui->cmethodCBox->currentIndex();
    if (ch == 0)
        canon(center, r, canvas);
    else if (ch == 1)
        param(center, r, canvas);
    else if (ch == 2)
        bresenhem(center, r, canvas);
    else if (ch == 3)
        midPoint(center, r, canvas);
    else if (ch == 4)
        defaultQt(center, r, canvas);
}

void MainWindow::on_circleBtn_clicked()
{
    int xc = ui->xcEnt->text().toInt();
    int yc = ui->ycEnt->text().toInt();
    int r = ui->rEnt->text().toInt();

    canvas_t canvas{&image, &fgColor};

    drawCircle(QPoint(xc, yc), r, canvas);

    imageView();
}

void MainWindow::on_concCirclesBtn_clicked()
{
    int xc = ui->xcEnt->text().toInt();
    int yc = ui->ycEnt->text().toInt();

    int rs = ui->rsEnt->text().toInt();
    int re = ui->reEnt->text().toInt();
    int dr = ui->drEnt->text().toInt();
    int n = ui->nEnt->text().toInt();

    canvas_t canvas{&image, &fgColor};

    if (n == 0)
        for (int r = rs; r < re; r += dr)
            drawCircle(QPoint(xc, yc), r, canvas);
    else if (dr == 0)
        for (int r = rs; r < re; r += (re - rs) / n)
            drawCircle(QPoint(xc, yc), r, canvas);

    imageView();
}

void MainWindow::drawEllipse(const QPoint &center, int a, int b, canvas_t &canvas)
{
    int ch = ui->cmethodCBox->currentIndex();
    if (ch == 0)
        canonEl(center, a, b, canvas);
    else if (ch == 1)
        paramEl(center, a, b, canvas);
    else if (ch == 2)
        bresenhemEl(center, a, b, canvas);
    else if (ch == 3)
        midPointEl(center, a, b, canvas);
    else if (ch == 4)
        defaultQtEl(center, a, b, canvas);
}

void MainWindow::on_ellipseBtn_clicked()
{
    int xc = ui->elxcEnt->text().toInt();
    int yc = ui->elycEnt->text().toInt();
    int a = ui->aEnt->text().toInt();
    int b = ui->bEnt->text().toInt();

    canvas_t canvas{&image, &fgColor};

    drawEllipse(QPoint(xc, yc), a, b, canvas);

    imageView();
}

void MainWindow::on_concEllipseBtn_clicked()
{
    int xc = ui->elxcEnt->text().toInt();
    int yc = ui->elycEnt->text().toInt();

    int a0 = ui->a0Ent->text().toInt();
    int b0 = ui->b0Ent->text().toInt();
    int n = ui->elNEnt->text().toInt();
    int dr = ui->elDrEnt->text().toInt();

    int a = a0, b = b0;
    canvas_t canvas{&image, &fgColor};

    for (int i = 0; i < n; i++)
        drawEllipse(QPoint(xc, yc), a + dr * i, b + dr * i, canvas);

    imageView();
}

void MainWindow::on_pushButton_clicked()
{
    Statistics popup;
    popup.setModal(true);
    popup.exec();
}
