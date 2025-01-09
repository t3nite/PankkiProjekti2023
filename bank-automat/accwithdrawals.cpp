#include "accwithdrawals.h"
#include "ui_accwithdrawals.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItem>

accWithdrawals::accWithdrawals(QByteArray& token, QString cardNumber, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::accWithdrawals)
    , webToken(token)
    , cardNumber(cardNumber)
{
    ui->setupUi(this);

    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QImage("C:/bank.background.jpg")));
    this->setPalette(palette);

    // Create a new QNetworkAccessManager object
    deleteManager = new QNetworkAccessManager(this);

    connect(ui->withBackButton, &QPushButton::clicked, this, &accWithdrawals::handleBack);

    // For testing purposes
    // Clears all account withdrawals from every account
    // connect(ui->deleteActionsButton, &QPushButton::clicked, this, &accWithdrawals::deleteResource);

    // Set the transTableWidget to a new QStandardItemModel object
    transTableWidget(new QStandardItemModel(this));
}

accWithdrawals::~accWithdrawals()
{
    delete ui;
}

void accWithdrawals::transTableWidget(QStandardItemModel *model)
{
    ui->transTableWidget->setModel(model);
}

void accWithdrawals::handleBack()
{
    mainUserInterface *mainUserInterfaceWindow = new mainUserInterface(webToken, cardNumber);
    mainUserInterfaceWindow->show();

    this->close();
}

// The following function is for testing purposes

/*
void accWithdrawals::deleteResource()
{
    // Create DELETE-request
    QUrl url("http://localhost:3000/accountinformation/delete");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    //WEBTOKEN START
    QByteArray myToken="Bearer "+webToken;
    request.setRawHeader(QByteArray("Authorization"),(myToken));
    //WEBTOKEN END

    // Make the DELETE request
    QNetworkReply *deleteReply = deleteManager->deleteResource(request);

    // Handle the response when it arrives
    connect(deleteReply, &QNetworkReply::finished, this, [=]() {
        if (deleteReply->error() == QNetworkReply::NoError) {
            // Response successful
            qDebug() << "DELETE-pyyntö onnistui";
            // Clear transTableWidget
            QStandardItemModel *model = qobject_cast<QStandardItemModel*>(ui->transTableWidget->model());
            if (model) {
                model->clear();
            }
        } else {
            // Response failed
            qDebug() << "Virhe DELETE-pyynnön suorittamisessa:" << deleteReply->errorString();
        }
        // Release resources
        deleteReply->deleteLater();
    });
}*/
