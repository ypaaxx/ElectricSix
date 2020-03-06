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
    
    QByteArray ms;
    Pid pid[6];
    
    qreal nominalRpm;
    QVector <qreal> *coeff;
    
    qreal rpm[6];
    QTextStream *output = nullptr;    
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
