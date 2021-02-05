#include <profileapi.h>
#include "measurement.h"
#include "statistics.h"
#include "ui_statistics.h"

#define REPEAT 5
#define R 5
#define STEPS 72

double measureCircle(LARGE_INTEGER frequency, int r, void(*method)(int))
{
    LARGE_INTEGER t1, t2;
    double elapsedTime;

    QueryPerformanceCounter(&t1);
    for (int i = 0; i < REPEAT; i++)
        method(r);
    QueryPerformanceCounter(&t2);

    elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000000. / REPEAT / frequency.QuadPart;
    return elapsedTime;
}

double measureEllipse(LARGE_INTEGER frequency, int a, int b, void(*method)(int, int))
{
    LARGE_INTEGER t1, t2;
    double elapsedTime;

    QueryPerformanceCounter(&t1);
    for (int i = 0; i < REPEAT; i++)
        method(a, b);
    QueryPerformanceCounter(&t2);

    elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000000. / REPEAT / frequency.QuadPart;
    return elapsedTime;
}

double measureQt(LARGE_INTEGER frequency, int a, int b)
{
    LARGE_INTEGER t1, t2;
    QPoint center(360, 360);
    double elapsedTime;

    QPixmap pixmap = QPixmap::fromImage(QImage(721, 721, QImage::Format_ARGB32));
    QPainter painter(&pixmap);
    painter.setPen(Qt::white);


    QueryPerformanceCounter(&t1);
    for (int i = 0; i < REPEAT; i++)
        defaultQtDrawM(center, a, b, painter);
    QueryPerformanceCounter(&t2);

    painter.end();

    elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000000. / REPEAT / frequency.QuadPart;
    return elapsedTime;
}

QVector<QVector<double>> circlesData()
{
    QVector<QVector<double>> timeData(5);

    LARGE_INTEGER frequency;        // ticks per second
    QueryPerformanceFrequency(&frequency);

    void (*method[4])(int);

    method[0] = canonM;
    method[1] = paramM;
    method[2] = bresenhemM;
    method[3] = midPointM;

    for (int j = 0; j < 4; j++)
        for (int r = 1; r < STEPS; r++)
            timeData[j].push_back(measureCircle(frequency, r * R, method[j]));

    for (int r = 1; r < STEPS; r++)
        timeData[4].push_back(measureQt(frequency, r * R, r * R));

    return timeData;
}

QVector<QVector<double>> ellipsesData()
{
    QVector<QVector<double>> timeData(5);

    LARGE_INTEGER frequency;        // ticks per second
    QueryPerformanceFrequency(&frequency);

    void (*method[4])(int, int);

    method[0] = canonElM;
    method[1] = paramElM;
    method[2] = bresenhemElM;
    method[3] = midPointElM;

    for (int j = 0; j < 4; j++)
        for (int r = 1; r < STEPS; r++)
            timeData[j].push_back(measureEllipse(frequency, r * R, R + r * R, method[j]));

    for (int r = 1; r < STEPS; r++)
        timeData[4].push_back(measureQt(frequency, r * R, R + r * R));

    return timeData;
}

Statistics::Statistics(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Statistics)
{
    ui->setupUi(this);

    setupFirstGraph();
    setupSecondGraph();
}

Statistics::~Statistics()
{
    delete ui;
}

void Statistics::setupFirstGraph()
{
    QCustomPlot *customPlot = ui->widget;

    customPlot->legend->setVisible(true);
    customPlot->yAxis->setLabel("Потраченное время в микросекундах");
    customPlot->legend->setFont(QFont("Helvetica", 9));
    QStringList lineNames;
    lineNames << "Каноническое ур." << "Параметрическое ур." << "Брезенхем"
              << "Средней точки" << "Библиотечный";

    for (int i = 0; i < 5; i++)
    {
        customPlot->addGraph();
        customPlot->graph(i)->setName(lineNames.at(i));
    }

    customPlot->graph(0)->setPen(QPen(Qt::blue));
    customPlot->graph(1)->setPen(QPen(Qt::red));
    customPlot->graph(2)->setPen(QPen(QColor(128, 0, 128)));
    customPlot->graph(3)->setPen(QPen(Qt::black));
    customPlot->graph(4)->setPen(QPen(QColor(210, 105, 38)));

    // generate some points of data (y0 for first, y1 for second graph):
    static QVector<QVector<double>> y(circlesData());
    QVector<double> x(STEPS);
    for (int i = 0; i < STEPS; i++)
        x[i] = i * R;

    // pass data points to graphs:
    for (int i = 0; i < 5; i++)
        customPlot->graph(i)->setData(x, y[i]);
    customPlot->graph(0)->rescaleAxes();
    for (int i = 1; i < 5; i++)
        customPlot->graph(i)->rescaleAxes(true);

    customPlot->axisRect()->setRangeZoom(Qt::Vertical);
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}

void Statistics::setupSecondGraph()
{
    QCustomPlot *customPlot = ui->widget_2;

    customPlot->legend->setVisible(true);
    customPlot->yAxis->setLabel("Потраченное время в микросекундах");
    customPlot->legend->setFont(QFont("Helvetica", 9));
    QStringList lineNames;
    lineNames << "Каноническое ур." << "Параметрическое ур." << "Брезенхем"
              << "Средней точки" << "Библиотечный";

    for (int i = 0; i < 5; i++)
    {
        customPlot->addGraph();
        customPlot->graph(i)->setName(lineNames.at(i));
    }


    customPlot->graph(0)->setPen(QPen(Qt::blue));
    customPlot->graph(1)->setPen(QPen(Qt::red));
    customPlot->graph(2)->setPen(QPen(QColor(128, 0, 128)));
    customPlot->graph(3)->setPen(QPen(Qt::black));
    customPlot->graph(4)->setPen(QPen(QColor(210, 105, 38)));

    // generate some points of data (y0 for first, y1 for second graph):
    static QVector<QVector<double>> y(circlesData());
    QVector<double> x(STEPS);
    for (int i = 0; i < STEPS; i++)
        x[i] = i * R;

    // pass data points to graphs:
    for (int i = 0; i < 5; i++)
        customPlot->graph(i)->setData(x, y[i]);
    customPlot->graph(0)->rescaleAxes();
    for (int i = 1; i < 5; i++)
        customPlot->graph(i)->rescaleAxes(true);

    customPlot->axisRect()->setRangeZoom(Qt::Vertical);
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}
