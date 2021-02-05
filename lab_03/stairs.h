#ifndef STAIRS_H
#define STAIRS_H

#include <QDialog>
#include "qcustomplot.h"

namespace Ui {
class stairs;
}

class stairs : public QDialog
{
    Q_OBJECT

private:
    void createUI();

public:
    explicit stairs(QWidget *parent = nullptr);
    ~stairs();

private:
    Ui::stairs *ui;
};

#endif // STAIRS_H
