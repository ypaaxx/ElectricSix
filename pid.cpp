#include "pid.h"

qreal Pid::Kp = 0;
qreal Pid::Kd = 0;
qreal Pid::Ki = 0;
qreal Pid::duMax = 100;

Pid::Pid(QObject *parent) : QObject(parent)
{
    P = 0;
    D = 0;
    I = 0;
}
