QT       += core gui
QT += serialport
QT +=network
QT += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    accountbalance.cpp \
    accwithdrawals.cpp \
    addPin.cpp \
    main.cpp \
    mainuserinterface.cpp \
    mainwindow.cpp \
    moneyselect.cpp \
    selectamount.cpp

HEADERS += \
    accountbalance.h \
    accwithdrawals.h \
    addPin.h \
    mainuserinterface.h \
    mainwindow.h \
    moneyselect.h \
    selectamount.h

FORMS += \
    accountbalance.ui \
    accwithdrawals.ui \
    addPin.ui \
    mainuserinterface.ui \
    mainwindow.ui \
    moneyselect.ui \
    selectamount.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/RFIDReaderdll/build/release/ -lRFIDReaderdll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/RFIDReaderdll/build/debug/ -lRFIDReaderdll

INCLUDEPATH += $$PWD/RFIDReaderdll
DEPENDPATH += $$PWD/RFIDReaderdll
