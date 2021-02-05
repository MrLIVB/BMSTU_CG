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

    QVector<QPoint> clipper;
    QVector<QPoint> polygon;
    QPoint last;
    int last_prod = 0;

private slots:
    void mousePressEvent(QMouseEvent *event);

    void on_rectClrButton_clicked();

    void on_lineClrButton_clicked();

    void on_resClrButton_clicked();

    void on_clearButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_clicked();

    void on_clipButton_clicked();

    void on_closeButton_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;

    enum drawType {none, shift, control};

    void colorLabel(QLabel *label, QColor &color);
    void displayImage();
    void clearImage();
    void imageView();

    void add_vertex(QPoint &point, drawType type);
    void add_line(QPoint &point, drawType type);

    bool closed = false;
    bool closed2 = false;

    QColor clipperColor, lineColor, resColor;
    const QColor defaultClipperColor = QColor(Qt::black);
    const QColor defaultLineColor = QColor(Qt::red);
    const QColor defaultResColor = QColor(85, 85, 255);

    QPixmap pixmap;
    QGraphicsScene *scene;
};
#endif // MAINWINDOW_H
