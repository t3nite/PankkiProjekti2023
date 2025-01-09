#include "mainuserinterface.h"
#include "ui_mainuserinterface.h"
#include "moneyselect.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "accountbalance.h"
#include "ui_accountbalance.h"
#include "accwithdrawals.h"
#include "ui_accwithdrawals.h"
#include <QProcess>
#include <QStandardItem>
#include <QPixmap>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QLabel>

mainUserInterface::mainUserInterface(QByteArray& token, QString cardNumber, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mainUserInterface)
    , webToken(token)
    , cardNumber(cardNumber)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose);

    // Fetch the picture and show it
    QNetworkAccessManager *picManager = new QNetworkAccessManager(this);
    QNetworkReply *picReply = picManager->get(QNetworkRequest(QUrl("http://localhost:3000/images/mike.jpg")));

    QObject::connect(picReply, &QNetworkReply::finished, [=]() {
        if (picReply->error() == QNetworkReply::NoError) {
            QByteArray imageData = picReply->readAll();
            QPixmap originalPixmap;
            originalPixmap.loadFromData(imageData);

            if (!originalPixmap.isNull() && cardNumber == "-06000621FE") {

                // Scale the picture to a proper size
                QSize labelSize = ui->imageLabel->size();
                QPixmap scaledPixmap = originalPixmap.scaled(labelSize, Qt::KeepAspectRatio);

                ui->imageLabel->setPixmap(scaledPixmap);
            } else {
                qDebug() << "Kuvan lataaminen epäonnistui";
            }
        } else {
            qDebug() << "Virhe kuvaa ladattaessa:" << picReply->errorString();
        }
        picReply->deleteLater();
    });

    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QImage("C:/bank.background.jpg")));
    this->setPalette(palette);

    connect(ui->logOut, &QPushButton::clicked, this, &mainUserInterface::logoutClicked);
    connect(ui->withdrawMoney, &QPushButton::clicked, this, &mainUserInterface::withdrawMoneyClicked);
    connect(ui->showTransactions, &QPushButton::clicked, this, &mainUserInterface::handleTransactionsClicked);
    connect(ui->showTransactions, &QPushButton::clicked, this, &mainUserInterface::handleCreditTransactionsClicked);
    connect(ui->showBalance, &QPushButton::clicked, this , &mainUserInterface::getDebitBalance);
    connect(ui->showBalance, &QPushButton::clicked, this , &mainUserInterface::getCreditBalance);
}

mainUserInterface::~mainUserInterface()
{
    delete ui;
}

void mainUserInterface::withdrawMoneyClicked()
{
    // Create and show moneySelect-object
    moneySelect *moneySelectWindow = new moneySelect(webToken, cardNumber);
    moneySelectWindow->show();

    // Close mainUserInterface-window
    this->close();
}

void mainUserInterface::handleTransactionsClicked()
{
    QString site_url="http://localhost:3000/accountinformation/info";
    QNetworkRequest request((site_url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    //WEBTOKEN START
    QByteArray myToken="Bearer "+webToken;
    request.setRawHeader(QByteArray("Authorization"),(myToken));
    //WEBTOKEN END

    getManager = new QNetworkAccessManager(this);
    connect(getManager, SIGNAL(finished(QNetworkReply*)),this, SLOT(transactionsNetworkReqFin(QNetworkReply*)));
    reply = getManager->get(request);
}

void mainUserInterface::transactionsNetworkReqFin(QNetworkReply *reply)
{
    // Debit transactions

    QByteArray transResponse_data=reply->readAll();
    if (cardNumber != "-0600062093") {
        reply->deleteLater();
        getManager->deleteLater();
        return;
    }
    qDebug() << "" << transResponse_data;

    QJsonDocument json_doc = QJsonDocument::fromJson(transResponse_data);
    QJsonArray json_array = json_doc.array();

    QStandardItemModel *table_model = new QStandardItemModel(json_array.size(),2);
    table_model->setHeaderData(0, Qt::Horizontal, QObject::tr("Määrä"));
    table_model->setHeaderData(1, Qt::Horizontal, QObject::tr("Päiväys"));

    for (int row = 0; row < json_array.size(); ++row) {
        QJsonObject json_obj = json_array[row].toObject();
        QStandardItem *amount = new QStandardItem(json_obj["amount"].toString());
        table_model->setItem(row, 0, amount);
        QStandardItem *date = new QStandardItem(json_obj["date"].toString());
        table_model->setItem(row, 1, date);
    }

    // Find the accwithdrawals window and set the model to its transTableWidget
    accWithdrawals *accWithdrawalsWindow = new accWithdrawals(webToken, cardNumber);
    accWithdrawalsWindow->transTableWidget(table_model);
    accWithdrawalsWindow->show();

    // Close mainUserInterface-window
    this->close();

    reply->deleteLater();
    getManager->deleteLater();
}

void mainUserInterface::handleCreditTransactionsClicked()
{
    QString site_url="http://localhost:3000/accountinformation/infocredit";
    QNetworkRequest request((site_url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    //WEBTOKEN START
    QByteArray myToken="Bearer "+webToken;
    request.setRawHeader(QByteArray("Authorization"),(myToken));
    //WEBTOKEN END

    getCreditManager = new QNetworkAccessManager(this);
    connect(getCreditManager, SIGNAL(finished(QNetworkReply*)),this, SLOT(transactionsCreditNetworkReqFin(QNetworkReply*)));
    creditReply = getCreditManager->get(request);
}

void mainUserInterface::transactionsCreditNetworkReqFin(QNetworkReply *creditReply)
{
    //Credit transactions

    QByteArray transCreditResponse_data=creditReply->readAll();
    if (cardNumber !="-06000621FE") {
        creditReply->deleteLater();
        getCreditManager->deleteLater();
        return;
    }
    qDebug() << "" << transCreditResponse_data;

    QJsonDocument json_doc = QJsonDocument::fromJson(transCreditResponse_data);
    QJsonArray json_array = json_doc.array();

    QStandardItemModel *table_model = new QStandardItemModel(json_array.size(),2);
    table_model->setHeaderData(0, Qt::Horizontal, QObject::tr("Määrä"));
    table_model->setHeaderData(1, Qt::Horizontal, QObject::tr("Päiväys"));

    for (int row = 0; row < json_array.size(); ++row) {
        QJsonObject json_obj = json_array[row].toObject();
        QStandardItem *amount = new QStandardItem(json_obj["amount"].toString());
        table_model->setItem(row, 0, amount);
        QStandardItem *date = new QStandardItem(json_obj["date"].toString());
        table_model->setItem(row, 1, date);
    }

    // Find the accwithdrawals window and set the model to its transTableWidget
    accWithdrawals *accWithdrawalsWindow = new accWithdrawals(webToken, cardNumber);
    accWithdrawalsWindow->transTableWidget(table_model);
    accWithdrawalsWindow->show();

    // Close mainUserInterface-window
    this->close();

    creditReply->deleteLater();
    getCreditManager->deleteLater();
}

void mainUserInterface::getDebitBalance()
{
    qDebug() << "Card number: " << cardNumber;
    //If the URL changes on the REST API side, then there should also be a corresponding change here
    QString site_url="http://localhost:3000/accounts/getaccountbalance/1";
    QNetworkRequest request((site_url));

    //WEBTOKEN START
    QByteArray myToken="Bearer "+webToken;
    request.setRawHeader(QByteArray("Authorization"),(myToken));
    //WEBTOKEN END

    pgetManager = new QNetworkAccessManager(this);
    connect(pgetManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(debitRequestFinished(QNetworkReply*)));
    preply = pgetManager->get(request);
}

void mainUserInterface::debitRequestFinished(QNetworkReply *preply)
{
    // Debit balance
    qDebug() << "Card number in debitRequestFinished(): " << cardNumber;

    QByteArray response_data=preply->readAll();
    if (cardNumber == "-06000621FE") {
        preply->deleteLater();
        pgetManager->deleteLater();
    } else {
   // QString cleaned_response_data = QString::fromUtf8(response_data);
   // qDebug()<<"DATA : "+cleaned_response_data;
    qDebug()<<"DATA : "+response_data;

    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);

    //QJsonDocument json_doc = QJsonDocument::fromJson(cleaned_response_data.toUtf8());

    if (json_doc.isObject()) {
        QJsonObject json_obj = json_doc.object();
        QString account_balance = json_obj.value("account_balance").toString();
        qDebug() << "Current account balance: " << account_balance;

        // Create a new window and a user interface object
        accountBalance *accountBalanceWindow = new accountBalance(webToken, cardNumber);

        QLabel *debitLabel = accountBalanceWindow->findChild<QLabel*>("debitlabel");
        if (debitLabel) {
            debitLabel->setText(account_balance);
        }
        accountBalanceWindow->show();

        // Close the current window
        this->close();

        preply->deleteLater();
        pgetManager->deleteLater();
    }
 }
}

void mainUserInterface::getCreditBalance()
{
    //If the URL changes on the REST API side, then there should also be a corresponding change here
    QString Creditsite_url="http://localhost:3000/accounts/getcreditlimit/4";
    QNetworkRequest Creditrequest((Creditsite_url));

    //WEBTOKEN START
    QByteArray myToken="Bearer "+webToken;
    Creditrequest.setRawHeader(QByteArray("Authorization"),(myToken));
    //WEBTOKEN END

    pgetCreditManager = new QNetworkAccessManager(this);
    connect(pgetCreditManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(creditRequestFinished(QNetworkReply*)));
    pCreditreply = pgetCreditManager->get(Creditrequest);
}

void mainUserInterface::creditRequestFinished(QNetworkReply *pCreditreply)
{
    // Credit balance

    QByteArray creditResponse_data=pCreditreply->readAll();
    if (cardNumber == "-0600062093") {
        pCreditreply->deleteLater();
        pgetCreditManager->deleteLater();
    } else {
    qDebug()<<"DATA : "+creditResponse_data;

    QJsonDocument json_doc = QJsonDocument::fromJson(creditResponse_data);

    if (json_doc.isObject()) {
        QJsonObject json_obj = json_doc.object();
        QString credit_limit = json_obj.value("credit_limit").toString();
        qDebug() << "Current credit limit: " << credit_limit;

        // Create a new window and a user interface object
        accountBalance *accountBalanceWindow = new accountBalance(webToken, cardNumber);

        QLabel *creditLabel = accountBalanceWindow->findChild<QLabel*>("creditLabel");
        if (creditLabel) {
            creditLabel->setText(credit_limit);
        }
        accountBalanceWindow->show();

        // Close the current window
        this->close();

        pCreditreply->deleteLater();
        pgetCreditManager->deleteLater();
    }
}
}

void mainUserInterface::logoutClicked()
{
    // Close the app
    qApp->quit();

    // Restart the app
    QProcess::startDetached(QApplication::applicationFilePath());
}
