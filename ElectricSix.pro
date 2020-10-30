#-------------------------------------------------
#
# Project created by QtCreator 2018-08-13T11:03:32
#
#-------------------------------------------------
# Для сборки:
# C:\Qt\5.14.2\mingw73_32\bin\windeployqt.exe "D:\ProgramsAndSomeScripts\ElectricSix\build-ElectricSix-Desktop_Qt_5_14_2_MinGW_32_bit-Release\release\ElectricSix.exe"
# C:\Qt\5.14.2\mingw73_64\bin\windeployqt.exe "D:\ProgramsAndSomeScripts\ElectricSix\build-ElectricSix-Desktop_Qt_5_14_2_MinGW_64_bit-Release\release\ElectricSix.exe"
# C:\Qt\5.15.1\mingw81_32\bin\windeployqt.exe "D:\ProgramsAndSomeScripts\ElectricSix\build-ElectricSix-Desktop_Qt_5_15_1_MinGW_32_bit-Release\release\ElectricSix.exe"


QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ElectricSix
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp\
    config.cpp \
        mainwindow.cpp \
    pid.cpp

HEADERS  += mainwindow.h \
    config.h \
    pid.h

FORMS    += mainwindow.ui
