#ifndef PID_H
#define PID_H

#include <QObject>
#include <QDebug>

class Pid : public QObject
{
    Q_OBJECT

    static double Kp;
    static double Kd;
    static double Ki;

    const double T = 1; //s

    double P, D, I;

    double e_old;

public:
    explicit Pid(QObject *parent = nullptr);

signals:

public slots:
    double u(double e){

        static quint8 count=0;
        if(++count > 30) {
            I = 0;
            count = 0;
        }

        P = Kp * e;
        D = Kd * (e - e_old) / T;
        I += Ki * e * T / 100;

        e_old = e;

        //qDebug() << "pid: " << P << D << I;
        //qDebug() << "pid: "<< P + D + I;
        return P + D + I;
    }

    static void changeKp(double kp){Kp=kp;}
    static void changeKd(double kd){Kd=kd;}
    static void changeKi(double ki){Ki=ki;}
};

#endif // PID_H
