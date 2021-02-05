#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QGraphicsScene>

#include "alg.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QRect rect;
    QVector<QLine> lines;
    QPoint last;

private slots:
    void mousePressEvent(QMouseEvent *event);

    void on_rectClrButton_clicked();

    void on_lineClrButton_clicked();

    void on_resClrButton_clicked();

    void on_clearButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_clicked();

    void on_fillButton_clicked();

private:
    Ui::MainWindow *ui;

    enum drawType {none, shift};

    void colorLabel(QLabel *label, QColor &color);
    void clearImage();
    void imageView();

    void add_rect(const QPoint &point);
    void add_line(QPoint &point, drawType type);

    QColor rectColor, lineColor, resColor;
    const QColor defaultRectColor = QColor(Qt::black);
    const QColor defaultLineColor = QColor(Qt::red);
    const QColor defaultResColor = QColor(85, 85, 255);

    QPixmap pixmap;
    QGraphicsScene *scene;
};
#endif // MAINWINDOW_H
