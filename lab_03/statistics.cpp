#include <profileapi.h>
#include "statistics.h"
#include "ui_statistics.h"
#include "methods.h"

#define REPEAT 200

float toRadm(float x) {return x * M_PI / 180;}

double measure(LARGE_INTEGER frequency, QLine lines[90], canvas_t canvas, int(*method)(const QLine&, canvas_t&))
{
    LARGE_INTEGER t1, t2;
    double elapsedTime;

    QueryPerformanceCounter(&t1);
    for (int i = 0; i < REPEAT; i++)
    {
        for (int j = 0; j < 90; j++)
        {
            method(lines[j], canvas);
        }
    }

    QueryPerformanceCounter(&t2);
    elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000000.0 / (REPEAT * 90) / frequency.QuadPart;
    return elapsedTime;
}

void defaultQtCore(const QLine &line, QPainter &painter)
{
    painter.drawLine(line);
}

double measureQt(LARGE_INTEGER frequency, QLine lines[90], canvas_t canvas)
{
    QPixmap pixmap = QPixmap::fromImage((*canvas.image));
    QPainter painter(&pixmap);
    painter.setPen(canvas.color->rgb());

    LARGE_INTEGER t1, t2;
    double elapsedTime;

    QueryPerformanceCounter(&t1);
    for (int i = 0; i < REPEAT; i++)
    {
        for (int j = 0; j < 90; j++)
        {
            defaultQtCore(lines[j], painter);
        }
    }

    QueryPerformanceCounter(&t2);
    painter.end();
    elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000000.0 / (REPEAT * 90) / frequency.QuadPart;
    return elapsedTime;
}

Statistics::Statistics(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Statistics)
{
    ui->setupUi(this);

    QCustomPlot *customPlot = ui->customPlot;
    QCPBars *time = new QCPBars(customPlot->xAxis, customPlot->yAxis);

    time->setAntialiased(false);

    time->setName("Время построения");
    time->setPen(QPen(QColor(111, 9, 176).lighter(170)));
    time->setBrush(QColor(111, 9, 176));

    // prepare x axis with country labels:
    QVector<double> ticks;
    QVector<QString> labels;
    ticks << 1 << 2 << 3 << 4 << 5 << 6;
    labels << "ЦДА" << "Брез." << "Брез.(с устр.ступ.)" << "Брез.(целочисл.)" << "Библиот." << "Ву";
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    customPlot->xAxis->setTicker(textTicker);
    customPlot->xAxis->setTickLabelRotation(30);
    customPlot->xAxis->setSubTicks(false);
    customPlot->xAxis->setTickLength(0, 4);
    customPlot->xAxis->setRange(0, 8);
    customPlot->xAxis->setBasePen(QPen(Qt::black));
    customPlot->xAxis->setTickPen(QPen(Qt::black));
    customPlot->xAxis->grid()->setVisible(true);
    customPlot->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    customPlot->xAxis->setTickLabelColor(Qt::black);
    customPlot->xAxis->setLabelColor(Qt::black);

    // prepare y axis:
    customPlot->yAxis->setRange(0, 12);
    customPlot->yAxis->setPadding(5); // a bit more space to the left border
    customPlot->yAxis->setLabel("Потраченное время в микросекундах");
    customPlot->yAxis->setBasePen(QPen(Qt::black));
    customPlot->yAxis->setTickPen(QPen(Qt::black));
    customPlot->yAxis->setSubTickPen(QPen(Qt::black));
    customPlot->yAxis->grid()->setSubGridVisible(true);
    customPlot->yAxis->setTickLabelColor(Qt::black);
    customPlot->yAxis->setLabelColor(Qt::black);
    customPlot->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
    customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

    // Add data:
    QVector<double> timeData;

    QColor color{Qt::white};
    QImage image = QImage(721, 721, QImage::Format_ARGB32);
    canvas_t canvas {&image , &color};
    static QLine lines[90];
    for (int i = 0; i < 90; i++)
        lines[i] = QLine(360, 360, 360 + 180 * cos(toRadm(i * 2)), 360 + 180 * sin(toRadm(i * 2)));


    LARGE_INTEGER frequency;        // ticks per second
    QueryPerformanceFrequency(&frequency);

    static double timeDDA = measure(frequency, lines, canvas, dda);
    static double timeBrezenhem = measure(frequency, lines, canvas, brezenhem);
    static double timeBrezenhemAA = measure(frequency, lines, canvas, brezenhemAntialized);
    static double timeBrezenhemInt = measure(frequency, lines, canvas, brezenhemInt);
    static double timeDefaultQt = measureQt(frequency, lines, canvas);
    static double timeWu = measure(frequency, lines, canvas, wu);

    timeData << timeDDA << timeBrezenhem << timeBrezenhemAA << timeBrezenhemInt \
             << timeDefaultQt << timeWu;

    time->setData(ticks, timeData);

    // setup legend:
    customPlot->legend->setVisible(true);
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
    customPlot->legend->setBrush(QColor(255, 255, 255, 100));
    customPlot->legend->setBorderPen(Qt::NoPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    customPlot->legend->setFont(legendFont);
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

Statistics::~Statistics()
{
    delete ui;
}
