#include "pid.h"

qreal Pid::Kp = 0;
qreal Pid::Kd = 0;
qreal Pid::Ki = 0;
qreal Pid::uMax = 0;
qreal Pid::uMin = 0;

Pid::Pid(QObject *parent) : QObject(parent)
{
    e_old = 0;
    P = 0;
    D = 0;
    I = 0;
}
