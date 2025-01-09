#include "rfidreaderdll.h"
#include <QSerialPortInfo>
#include <QDebug>
#include <QRegularExpression>

RFIDReaderdll::RFIDReaderdll(QObject *parent)
    : QObject(parent)
{
    // Find RFID-reader from COM5-port
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        if (info.portName() == "COM5") {
            m_serial.setPort(info);
            if (m_serial.open(QIODevice::ReadOnly)) {
                connect(&m_serial, &QSerialPort::readyRead, this, &RFIDReaderdll::readRFID);
                qDebug() << "RFID-lukija löydetty COM5-portista";
            } else {
                qDebug() << "Sarjaportin avaaminen epäonnistui";
            }
            return;
        }
    }
    qDebug() << "RFID-lukijaa ei löydetty COM5-portista";
}

void RFIDReaderdll::readRFID()
{
    // Loading data from the serial port and storing it in a data object.
    // QByteArray is a dynamically resizable byte buffer; its size can change
    QByteArray data = m_serial.readAll();
    //The "data" is converted to a QString using the Latin-1 character set.
    //The "trimmed" method removes unnecessary spaces and line breaks
    QString cardID = QString::fromLatin1(data).trimmed();
    //Send the signal
    emit cardDetected(cardID);
}
