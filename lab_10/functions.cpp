#include "functions.h"

#include <math.h>
#include <QVector>

using namespace std;

float f1(float x, float z)
{
    return (x + z) / 2;
}

float f2(float x, float z)
{
    return std::sin(x) + std::cos(z);
}

float f3(float x, float z)
{
    return std::abs(std::cos(x) * std::sin(z));
}

float f4(float x, float z)
{
    return std::exp(std::sin(x) * std::cos(z));
}
