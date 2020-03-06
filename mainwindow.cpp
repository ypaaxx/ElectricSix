#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("ElectricSix");
    timer = new QTimer();
    timer->setInterval(1000);

    ms.append(13, 0);

    serial = new QSerialPort();
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setParity(QSerialPort::NoParity);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    Pid::changeKp(ui->doubleSpinBox->value());
    Pid::changeKd(ui->doubleSpinBox_2->value());
    Pid::changeKi(ui->doubleSpinBox_3->value());
    Pid::setMax(2300);
    Pid::setMin(800);

    if (findArduino())
    {
        timer->start();
        connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
        connect(serial, SIGNAL(readyRead()), this, SLOT(serialRead()));

    }
    else {
        qDebug() << "wtf arduino not found";
    }

}

/* Происходит остановка всех двигателей, закрытие файлов */
MainWindow::~MainWindow()
{

    timer->stop();
    
    for(quint8 i = 0; i<12;){   //Остановка всех двигателей при выходе из программы
        ms[i++] = 800 >> 8;
        ms[i++] = 800 & 0xFF;
    }
    ms[12] = crc8(ms, 12);
    serial->write(ms);
    serial->waitForBytesWritten(300);
    
    if(output != nullptr) output->device()->close();
    qDebug() << "Exit";
    delete ui;
}

/* По срабатыванию таймера отправляет в серийный порт массив команд */
void MainWindow::timeout(){
    qDebug() << "timeout";
    if( !serial->write(ms) )
        qDebug() << "Fail send";
}

void MainWindow::serialRead(){

    //Сбор данных с порта
    if (serial->bytesAvailable() < 13) return;
    QByteArray mess = serial->readAll();

    qDebug() << "input " << mess.count() << " bytes: " << mess;

    //Проверка контрольных сумм
    if(crc8(mess.data(), 12) != mess.at(12)){
        qDebug() << "crc is not Ok";
        return;
    }

    //Приходящее значение в герц
    static quint16 hz[6];
    for (quint8 i = 0; i < 6; i++){
        hz[i] = mess.at(2*i) << 8;
        hz[i] += mess.at(2*i + 1) & 0xFF;
        hz[i] >>= 1;
        rpm[i] = hz[i] * 8.38542;
        qDebug() << "i:" << i << hz[i] << "Hz " << rpm[i] << "rpm ";
    }

    ui->hz_1->setText( QString::number(hz[0]) + " Hz");
    ui->lcdNumber->display(rpm[0]);


    static qreal needRPM[6];
    for (quint8 i = 0; i < 6; i++){
        needRPM[i] = ui->spinBox->value()*coeff->at(i);
        qDebug() << "i:" << needRPM[i] << " need";
    }

    static qreal err[6];
    for (quint8 i = 0; i < 6; i++){
        err[i] = fabs(pow(1 + (needRPM[i] - rpm[i])/needRPM[i], 2)*100 - 100);
    }
    ui->prErr1->setText(QString::number(err[0], 'f', 1) + "%");
    if(err[0] < 2)  {
       // Чтото должно было произойти...
    }

    static quint16 u[6];
    if (ui->radioButton->isChecked()){
        for (quint8 i = 0; i < 6; i++){
            if (err[i] > 2){
                u[i] += pid[i].u(needRPM[i] - rpm[i]);
                if (u[i] > 2300) u[i] = 2300;
                if (u[i] < 800) u[i] = 800;
            }
        }
    } else {
        for (quint8 i = 0; i < 6; i++)
            u[i] = ui->spinBox_2->value();
    }
    ui->label_4->setNum(u[0]);

    for (quint8 i = 0; i < 6; i++){
        qDebug() <<"u[" << i << "] " << u[i];
        ms[i*2] = u[i] >> 8;
        ms[i*2 + 1] = u[i] & 0xFF;
    }
    ms[12] = crc8(ms.data(), 12);
}

//Изменение параметров регулятора
void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    Pid::changeKp(arg1);
}
void MainWindow::on_doubleSpinBox_2_valueChanged(double arg1)
{
    Pid::changeKd(arg1);
}
void MainWindow::on_doubleSpinBox_3_valueChanged(double arg1)
{
    Pid::changeKi(arg1);
}

void MainWindow::on_spinBox_editingFinished()
{
    nominalRpm = ui->spinBox->value();
}
