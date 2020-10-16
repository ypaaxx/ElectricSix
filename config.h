#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QFile>
#include <QMap>
#include <QDebug>

/* Создает файлы конфигурации. читает их и всё в этом духе */
class Config : public QObject
{
    Q_OBJECT
    QFile config;
    QMap <QString, qreal> *param ;

    qreal Kp, Kd, Ki, s;
public:
    explicit Config(QObject *parent = nullptr){
        read();
    };

    ~Config(){
        close();
    };

    void read(QString name = "config.txt"){
        config.setFileName(name);
        if (!config.open(QFile::ReadWrite)){
                  qDebug() << "config file error";
                  return;
        }
        qDebug() << "config file "<< name;

        if (!config.size())
            makeFile();
        
        param = new QMap <QString, qreal>;

        QTextStream stream(&config);
        while (!stream.atEnd())
        {
            QString str = stream.readLine(1024);
            if(str.at(0) == '#') //Игнорирование комментариев
                continue;
            QStringList list = str.split(QRegExp(" "));

            if(list.size() != 2)
                qDebug() << "неверное количество параметров в config" << list;

            if (list.at(0) == "Kp")
                Kp = list.at(1).toDouble();
            if (list.at(0) == "Kd")
                Kd = list.at(1).toDouble();
            if (list.at(0) == "Ki")
                Ki = list.at(1).toDouble();
            if (list.at(0) == "s")
                s = list.at(1).toDouble();
        }
        return;
    }

    //Создание файла с нашими константами
    void makeFile(qreal Kp = 0.03, qreal Kd = 0.02, qreal Ki = 0.015, qreal s = 8.38542){
        config.resize(0);
        QTextStream stream(&config);
        qDebug() << "write config file " << config.fileName();
        stream << "Kp " + QString::number(Kp) << endl;
        stream << "Kd " + QString::number(Kd) << endl;
        stream << "Ki " + QString::number(Ki) << endl;
        stream << "s " + QString::number(s) << endl;
    }

    qreal kp(){
        return Kp;
    }
    qreal kd(){
        return Kd;
    }
    qreal ki(){
        return Ki;
    }
    qreal ks(){
        return s;
    }
    qreal setKp(qreal k){
        Kp = k;
        return Kp;
    }
    qreal setKd(qreal k){
        Kd = k;
        return Kd;
    }
    qreal setKi(qreal k){
        Ki = k;
        return Ki;
    }
    qreal setS(qreal k){
        s = k;
        return s;
    }

    void close(){
        makeFile(Kp, Kd, Ki, s);
        config.close();
    }

signals:

};

#endif // CONFIG_H
