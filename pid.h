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

    static qreal uMax;
    static qreal uMin;

    const qreal T = 1; //s

    qreal P, D, I;

    qreal e_old;

    //Ограничение по сигналу
    static qreal u_max;

public:
    explicit Pid(QObject *parent = nullptr);

signals:

public slots:
    qreal u(qreal e){

        //Периодическое обнуление интегрирования
//        static quint8 count=0;
//        if(++count > 30) {
//            I = 0;
//            count = 0;
//        }

        P = Kp * e;                 //Пропорциональный
        D = Kd * (e - e_old) / T;   //Дифференциальный
        I += Ki * e * T / 100;      //Интегрирующий
        e_old = e;

        qreal u = P + D + I;
        //Ограничение сигнала
        if (u > uMax) u = uMax;
        if (u < uMin) u = uMin;
        return u;
    }

    static void changeKp(qreal kp){Kp=kp;}
    static void changeKd(qreal kd){Kd=kd;}
    static void changeKi(qreal ki){Ki=ki;}
    static void setMax(qreal _uMax){uMax=_uMax;}
    static void setMin(qreal _uMin){uMin=_uMin;}
    void resetI(){
        I = 0;
    }
};

#endif // PID_H
