#ifndef PID_H
#define PID_H

#include <QObject>
#include <QDebug>

class Pid : public QObject
{
    Q_OBJECT

    // Константы
    static qreal Kp;
    static qreal Kd;
    static qreal Ki;

    static qreal duMax;

    const qreal T = 1; //s

    qreal P, D, I;

    //Ограничение по сигналу
    static qreal u_max;

public:
    explicit Pid(QObject *parent = nullptr);

signals:

public slots:
    qreal u(qreal e){
        static qreal e_old = 0;

        P = Kp * e;                 //Пропорциональный
        D = Kd * (e - e_old) / T;   //Дифференциальный
        I += Ki * e * T / 100;      //Интегрирующий

        e_old = e;

        qreal u = P + D + I;
        //Ограничение сигнала
        if (u > duMax) u = duMax;
        if (u < -duMax) u = -duMax;
        return u;
    }

    static void changeKp(qreal kp){Kp=kp;}
    static void changeKd(qreal kd){Kd=kd;}
    static void changeKi(qreal ki){Ki=ki;}
    static void setDuMax(qreal _duMax){duMax=_duMax;}

    void resetI(){
        I = 0;
    }
};

#endif // PID_H
