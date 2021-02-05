#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QVector>

struct function{
    const char *name;
    float(*f)(float, float);
};

float f1(float x, float z);
float f2(float x, float z);
float f3(float x, float z);
float f4(float x, float z);

#endif // FUNCTIONS_H
