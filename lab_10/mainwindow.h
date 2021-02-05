#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QGraphicsScene>

#include "alg.h"
#include "functions.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_buildButton_clicked();

    void on_rotateButton_clicked();

private:
    Ui::MainWindow *ui;

    void colorLabel(QLabel *label, QColor &color);
    void displayImage();
    void clearImage();
    void imageView();

    range getXRange();
    range getZRange();
    void getAngles();

    QColor resColor, bgColor;
    const QColor defaultResColor = QColor(Qt::black);
    const QColor defaultBgColor = QColor(Qt::white);

    QVector<function> func_list = {{"(x + z) / 2", f1},
                                   {"sin(x) + cos(z)", f2},
                                   {"|cos(x) * sin(x)|", f3},
                                   {"exp(sin(x) * cos(z))", f4}};

    angles teta{0, 0, 0};

    QPixmap pixmap;
    QGraphicsScene *scene;
};
#endif // MAINWINDOW_H
