#include "ui_stairs.h"
#include "stairs.h"
#include "measureMethods.h"
#include <iostream>

stairs::stairs(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::stairs)
{
    ui->setupUi(this);

    this->setWindowTitle("Ступенчатость");

    this->createUI();
}

stairs::~stairs()
{
    delete ui;
}

void stairs::createUI()
{
    int c = 9;

    ui->tableWidget->setColumnCount(c - 1); // Указываем число колонок
    ui->tableWidget->setRowCount(5);
    ui->tableWidget->setShowGrid(true); // Включаем сетку

    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QStringList hHeaders, vHeaders;
    for (int i = 90 / c; i < 90; i += 90 / c)
        hHeaders << QString::number(i) + "°";
    vHeaders << QString("ЦДА") << QString("Брезенхем") << QString("Брезенхем цел.")
             << QString("Брезенхем сгл.") << QString("Ву");

    ui->tableWidget->setHorizontalHeaderLabels(hHeaders);
    ui->tableWidget->setVerticalHeaderLabels(vHeaders);


    int (*methods[5])(QLine&);
    methods[0] = ddaCS;
    methods[1] = brezenhemCS;
    methods[2] = brezenhemIntCS;
    methods[3] = brezenhemAntializedCS;
    methods[4] = wuCS;

    QLine line(0,0,0,0);
    float x, y;
    int va = 0;

    for (int angle = 90 / c; angle < 90; angle += 90 / c)
    {
        x = 100 * cos(angle * M_PI / 180);
        y = 100 * sin(angle * M_PI / 180);
        line.setP2(QPoint(x, y));
        for (int i = 0; i < 5; i++)
        {
            va = methods[i](line);
            ui->tableWidget->setItem(i, angle / (90 / c) - 1, new QTableWidgetItem(QString::number(va)));
        }
        std::cout << std::endl;
    }
}
