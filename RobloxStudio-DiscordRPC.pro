QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# QttpServer
#CONFIG += debug libraries/QttpServer/qttpserver.pro

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES = $$files(*.cpp, true)
#SOURCES += main.cpp

HEADERS = $$files(*.h, true)

FORMS = $$files(*.ui, true)
UI_DIR = ./interface

# libraries
INCLUDEPATH += libs/

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
