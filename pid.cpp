#include "pid.h"

double Pid::Kp = 0;
double Pid::Kd = 0;
double Pid::Ki = 0;
Pid::Pid(QObject *parent) : QObject(parent)
{
    e_old = 0;
    P = 0;
    D = 0;
    I = 0;
}
