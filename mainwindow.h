#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <math.h>

#include <QMainWindow>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QTimer>
#include <QByteArray>
#include <QVector>
#include <QFileDialog>
#include <QDate>
#include <QTime>

#include <pid.h>
#include <config.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
    QSerialPort *serial;
    QTcpServer *server;
    QDataStream *stream;
    QTcpSocket *socket;
    //QList <QTcpSocket*> sockets;
    QTimer *timer;
    
    QByteArray ms;
    Pid pid[6];
    
    qreal nominalRpm = 1000;
    QVector <qreal> *coeff;
    qreal nEngine = 3040.0/360.0; // Коэффициент скольжения или типа того. Fr*nEngine = rpm;
    qreal nMax = 2500; //Максимальная частота. превышение должно пресекаться
    
    qreal rpm[6];
    QTextStream *output = nullptr;

    Config* config;
public:
    
    bool findArduino();
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
    void stop();

    void newConnect();
    
    void on_doubleSpinBox_engine_n_valueChanged(double arg1);

    void on_radioButton_toggled(bool checked);

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
