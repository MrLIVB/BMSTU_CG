#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QGraphicsScene>
#include "methods.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	void colorLabel(QLabel *label, QColor &color);
	void clearImage();
	void imageView();

    bool drawLine(QLine &line, canvas_t &canvas);
    void drawPoint(QPoint point);

private slots:

    void on_fgColorBtn_clicked();

    void on_bgToFgBtn_clicked();

    void on_bgColorBtn_clicked();

    void on_lineBtn_clicked();

    void on_clearBtn_clicked();

    void on_sunLinesBtn_clicked();

    void on_defaultFgBtn_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
	Ui::MainWindow *ui;

    QColor fgColor, bgColor;
    const QColor defaultBgColor = QColor(Qt::white);
    const QColor defaultFgColor = QColor(Qt::black);

	QImage image;
	QGraphicsScene *scene;
};

#endif // MAINWINDOW_H
