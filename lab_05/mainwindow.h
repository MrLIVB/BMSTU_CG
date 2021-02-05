#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QLabel>
#include "algorithm.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QVector<QPoint> vertices;
    QVector<QLine> edges;
    QVector<int> ends;
    QVector<int> starts;

    bool closed = false;
    int last_closed = 0;

    double fillFlag(QVector<QLine> edges, canvas_t &canvas, float delay);
    void intersection(const QLine &line, canvas_t &canvas, int extr);
    void dda(const QLine &line, canvas_t &canvas, int extr);


private slots:
    void mousePressEvent(QMouseEvent *event);
    void on_colorButton_clicked();

    void on_closeButton_clicked();

    void on_addButton_clicked();

    void on_clearButton_clicked();

    void on_fillButton_clicked();

private:
    Ui::MainWindow *ui;

    enum drawType {none, horizontal, vertical};

    void colorLabel(QLabel *label, QColor &color);
    void clearImage();
    void imageView();

    void addPoint(QPoint &point, drawType type);

    QColor fgColor, boundColor, bgColor;
    const QColor defaultFgColor = QColor(Qt::black);
    const QColor defaultBoundColor = QColor(Qt::black);

    QImage image;
    QGraphicsScene *scene;
};
#endif // MAINWINDOW_H
