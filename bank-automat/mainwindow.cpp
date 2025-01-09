#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mainUserInterface.h"
#include "ui_mainuserinterface.h"

#include "addPin.h"
#include "ui_addPin.h"

#include "ui_moneySelect.h"

#include <QTimer>
#include <QMessageBox>
#include <QDialog>
#include <QDebug>

#include <QMainWindow>

#include "rfidreaderdll.h"

#include "cdchoice.h"

#include <QRegularExpression>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QImage("C:/bank.background.jpg")));
    this->setPalette(palette);

    // Create RFID-reader object
    RFIDReaderdll *rfidReader = new RFIDReaderdll(this);

    // Connect signal and slot
    connect(rfidReader, &RFIDReaderdll::cardDetected, this, &MainWindow::handleCardDetected); 
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleCardDetected(QString cardID)
{
    // Remove unnecessary information from the end
    cardID.remove(QRegularExpression("[\\n\\r>]"));

    // Identify the card's ID and open the correct window based on it
    qDebug() << "Kortista luetut tiedot:" << cardID;

    if (cardID == "-0600062093") {

        cardNumber = cardID;
        qDebug() << "Debit-kortti";
        // Debit PIN is 1234
        addPin *addPinWindow = new addPin(cardNumber);
        this->close();
        addPinWindow->show();

    } else if (cardID == "-06000621FE") {

        cardNumber = cardID;
        qDebug() << "Credit-kortti ";
        // Credit PIN is 5678
        addPin *addPinWindow = new addPin(cardNumber);
        this->close();
        addPinWindow->show();
    } else {
        ui->begin->setText("Korttia ei tunnistettu");
    }
  }
