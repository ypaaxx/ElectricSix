#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <math.h>

#include <QMainWindow>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QTimer>
#include <QByteArray>
#include <QVector>
#include <QFileDialog>
#include <QDate>
#include <QTime>

#include <pid.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
    QSerialPort *serial;
    QTimer *timer;
    
    //unsigned char ms[6] = {0,0,0,0,0,0};
    QByteArray ms;
    Pid pid[6];
    
    qreal nominalRpm;
    QVector <qreal> *coeff;
    
    qreal rpm[6];
    QTextStream *output = nullptr;
    
    QVector <qreal> nom  = {1, 1, 1, 1, 1, 1};

    QVector <qreal> y1  = {0.9, 1.112, 0.9, 1.08, 0.9, 1.08};
    QVector <qreal> y2  = {0.8, 1.204, 0.8, 1.147, 0.8, 1.147};
    QVector <qreal> y3  = {0.7, 1.28, 0.7, 1.203, 0.7, 1.203};

    QVector <qreal> x1  = {0.9, 0.9, 0.9, 1.046, 1.175, 1.046};
    QVector <qreal> x2  = {0.8, 0.8, 0.8, 1.086, 1.311, 1.086};
    QVector <qreal> x3  = {0.7, 0.7, 0.7, 1.12, 1.421, 1.12};

    QVector <qreal> z1  = {0.9, 1, 1.091, 1.091, 1, 0.9};
    QVector <qreal> z2  = {0.8, 1, 1.166, 1.166, 1, 0.8};
    QVector <qreal> z3  = {0.7, 1, 1.229, 1.229, 1, 0.7};
    
public:
    
    bool findArduino()
    {
        /* Поиск порта с ардуинкой */
        QList <QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
        if(ports.isEmpty()){
            qDebug() << "Нет COM портов ;-(";
            return 0;
        }
        
        for (QSerialPortInfo& port: ports){
            if ( port.hasVendorIdentifier() )
                if ((port.vendorIdentifier() == 0x2341)
                        || (port.vendorIdentifier() == 0x1A86))
                    qDebug() << "Base found on" << port.portName();
            serial->setPort(port);
            return serial->open(QIODevice::ReadWrite);
        }
        //Происходит если ничего не нашёл
        return 0;
    }
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void timeout();
    void serialRead();
    
    void on_doubleSpinBox_valueChanged(double arg1);
    void on_doubleSpinBox_2_valueChanged(double arg1);
    void on_doubleSpinBox_3_valueChanged(double arg1);
    
    void on_spinBox_editingFinished();

    void start(){
        timer->stop(); //приостановка действующего таймера
        static QTimer *startTimer = new QTimer;
        
        if (1){
            startTimer->setInterval(500);
            startTimer->start();
            connect(startTimer, SIGNAL(timeout()), this, SLOT(start()));
        }
        
        static quint8 i = 0;
        while (i<6){
            //u[i] = 1500;
        }
        
    }
    
private:
    char crc8(const char *array, quint8 len)
    {
        char crc = 0x00;
        while (len--){
            crc ^= *array++;
            for (quint8 i = 0; i < 8; i++)
                crc = crc & 0x80 ? (crc << 1) ^ 0x07 : crc << 1;
        }
        return crc;
    }
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
