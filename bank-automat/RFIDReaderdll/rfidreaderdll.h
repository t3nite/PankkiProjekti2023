#ifndef RFIDREADERDLL_H
#define RFIDREADERDLL_H

#include "RFIDReaderdll_global.h"
#include <QObject>
#include <QSerialPort>
#include <QTimer>

class RFIDREADERDLL_EXPORT RFIDReaderdll : public QObject
{
    Q_OBJECT

public:
    explicit RFIDReaderdll(QObject *parent = nullptr);
    QString cardID;

signals:
    void cardDetected(QString cardID);

private slots:
    void readRFID();

private:
    QSerialPort m_serial;
};

#endif // RFIDREADERDLL_H
