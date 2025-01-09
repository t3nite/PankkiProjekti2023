#include "accountbalance.h"
#include "ui_accountbalance.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QCloseEvent>
#include "mainuserinterface.h"

accountBalance::accountBalance(QByteArray& token, QString cardNumber,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::accountBalance)
    , webToken(token)
    , cardNumber(cardNumber)
{
    ui->setupUi(this);

    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QImage("C:/bank.background.jpg")));
    this->setPalette(palette);

    connect(ui->logoutButton2, &QPushButton::clicked, this, &accountBalance::handleLogoutClicked);
    connect(ui->backButton, &QPushButton::clicked, this, &accountBalance::handleBackClicked);
}

accountBalance::~accountBalance()
{
    delete ui;
}

void accountBalance::handleBackClicked()
{
    mainUserInterface *mainUserInterfaceWindow = new mainUserInterface(webToken, cardNumber);
    mainUserInterfaceWindow->show();

    this->close();
}

void accountBalance::handleLogoutClicked()
{
    qApp->quit();

    QProcess::startDetached(QApplication::applicationFilePath());
}
