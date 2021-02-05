#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QGraphicsScene>
#include "circleMethods.h"
#include "ellipseMethods.h"

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
    void on_clearBtn_clicked();

    void on_defaultFgBtn_clicked();

    void on_bgToFgBtn_clicked();

    void on_bgColorBtn_clicked();

    void on_fgColorBtn_clicked();

    void on_circleBtn_clicked();

    void on_concCirclesBtn_clicked();

    void on_ellipseBtn_clicked();

    void on_concEllipseBtn_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    void colorLabel(QLabel *label, QColor &color);
    void clearImage();
    void imageView();

    void drawCircle(const QPoint &center, int r, canvas_t &canvas);
    void drawEllipse(const QPoint &center, int a, int b, canvas_t &canvas);

    QColor fgColor, bgColor;
    const QColor defaultBgColor = QColor(Qt::white);
    const QColor defaultFgColor = QColor(Qt::black);

    QImage image;
    QGraphicsScene *scene;
};
#endif // MAINWINDOW_H
