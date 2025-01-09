#include "moneyselect.h"
#include "ui_moneyselect.h"
#include "mainuserinterface.h"
#include "ui_mainuserinterface.h"
#include "selectamount.h"
#include "ui_selectamount.h"

moneySelect::moneySelect(QByteArray& token,QString cardNumber, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::moneySelect)
    , webToken(token)
    , cardNumber(cardNumber)
{
    ui->setupUi(this);

    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QImage("C:/bank.background.jpg")));
    this->setPalette(palette);

    connect(ui->backToMenu, &QPushButton::clicked, this, &moneySelect::handleBackToMenu);
    connect(ui->selectAmount, &QPushButton::clicked, this, &moneySelect::handleOtherAmount);

    connect(ui->twentyEuro, &QPushButton::clicked, this, &moneySelect::twentyEuroClickedPut);
    connect(ui->twentyEuro, &QPushButton::clicked, this, &moneySelect::twentyEuroClickedGet);
    connect(ui->twentyEuro, &QPushButton::clicked, this, &moneySelect::twentyEuroClickedPutCredit);
    connect(ui->twentyEuro, &QPushButton::clicked, this, &moneySelect::twentyEuroClickedGetCredit);

    connect(ui->fortyEuro, &QPushButton::clicked, this, &moneySelect::fortyEuroClickedPut);
    connect(ui->fortyEuro, &QPushButton::clicked, this, &moneySelect::fortyEuroClickedGet);
    connect(ui->fortyEuro, &QPushButton::clicked, this, &moneySelect::fortyEuroClickedPutCredit);
    connect(ui->fortyEuro, &QPushButton::clicked, this, &moneySelect::fortyEuroClickedGetCredit);

    connect(ui->fiftyEuro, &QPushButton::clicked, this, &moneySelect::fiftyEuroClickedPut);
    connect(ui->fiftyEuro, &QPushButton::clicked, this, &moneySelect::fiftyEuroClickedGet);
    connect(ui->fiftyEuro, &QPushButton::clicked, this, &moneySelect::fiftyEuroClickedPutCredit);
    connect(ui->fiftyEuro, &QPushButton::clicked, this, &moneySelect::fiftyEuroClickedGetCredit);

    connect(ui->hundredEuro, &QPushButton::clicked, this, &moneySelect::hundredEuroClickedPut);
    connect(ui->hundredEuro, &QPushButton::clicked, this, &moneySelect::hundredEuroClickedGet);
    connect(ui->hundredEuro, &QPushButton::clicked, this, &moneySelect::hundredEuroClickedPutCredit);
    connect(ui->hundredEuro, &QPushButton::clicked, this, &moneySelect::hundredEuroClickedGetCredit);

    // For testing purposes
    /*connect(ui->insertMoney, &QPushButton::clicked, this, &moneySelect::insertHundredClickedPut);
    connect(ui->insertMoney, &QPushButton::clicked, this, &moneySelect::insertHundredClickedPost);
    connect(ui->insertMoney, &QPushButton::clicked, this, &moneySelect::insertHundredClickedPutCredit);
    connect(ui->insertMoney, &QPushButton::clicked, this, &moneySelect::insertHundredClickedPostCredit);*/
}

moneySelect::~moneySelect()
{
    delete ui;
}

void moneySelect::handleBackToMenu()
{
    mainUserInterface *mainUserInterfaceWindow = new mainUserInterface(webToken, cardNumber);
    mainUserInterfaceWindow->show();

    this->close();
}

void moneySelect::handleOtherAmount()
{
    selectAmount *selectAmountWindow = new selectAmount(webToken, cardNumber);
    selectAmountWindow->show();

    this->close();
}

void moneySelect::twentyEuroClickedPut()
{
    // PUT-method

    if (cardNumber != "-0600062093") {
        return;
    }

    QJsonObject putObj;
    putObj.insert("account_balance",20);

    QString put_url="http://localhost:3000/accounts/update";
    QNetworkRequest putRequest((put_url));
    putRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    //WEBTOKEN START
    QByteArray myToken="Bearer "+webToken;
    putRequest.setRawHeader(QByteArray("Authorization"),(myToken));
    //WEBTOKEN END

    putManager = new QNetworkAccessManager(this);
    connect(putManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(put20Slot(QNetworkReply*)));

    QJsonDocument putDoc(putObj);
    QByteArray putData = putDoc.toJson();

    putReply = putManager->put(putRequest, putData);
}

void moneySelect::put20Slot(QNetworkReply *putReply)
{
    putResponse_data=putReply->readAll();
    qDebug()<<putResponse_data;
    putReply->deleteLater();
    putManager->deleteLater();

    // Parse the response into JSON format
    QJsonDocument jsonResponse = QJsonDocument::fromJson(putResponse_data);
    QJsonObject jsonObject = jsonResponse.object();

    int changed=jsonObject["changedRows"].toInt();
    qDebug()<<"Changed="<<changed;

    if (changed == 0)
    {   // The account balance would go negative, cannot withdraw money.
        ui->chooseLabel->setText("Tilillä ei ole tarpeeksi katetta");
    } else {
        // The withdrawal was successful, and the account balance remains positive
        ui->chooseLabel->setText("Nosto onnistui");
    }
}

void moneySelect::twentyEuroClickedGet()
{
    if (cardNumber != "-0600062093") {
        qDebug() << "Kortin numero ei ole sallittu.";
        return;
    }
    // Perform a GET request to retrieve the account balance
    QString site_url = "http://localhost:3000/accounts/getaccountbalance/1";
    QNetworkRequest getRequest(site_url);

    //WEBTOKEN START
    QByteArray myToken="Bearer "+webToken;
    getRequest.setRawHeader(QByteArray("Authorization"),(myToken));
    //WEBTOKEN END

    getManager = new QNetworkAccessManager(this);
    connect(getManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(twentyEuroClickedPost(QNetworkReply*)));
    getReply = getManager->get(getRequest);
}

void moneySelect::twentyEuroClickedPost(QNetworkReply *getReply)
{
    // Ensure that the response is available and there is no error
    if (getReply->error() == QNetworkReply::NoError) {
        QByteArray response_data = getReply->readAll();
        qDebug() << "GET-pyynnön vastaus: " << response_data;

        // Handle the response in JSON format
        QJsonDocument json_doc = QJsonDocument::fromJson(response_data);

        if (json_doc.isObject()) {
            QJsonObject json_obj = json_doc.object();
            QString account_balance = json_obj.value("account_balance").toString();
            qDebug() << "Current account balance: " << account_balance;

        // Check the account balance and make the POST request only if the balance is sufficient
        if (account_balance.toDouble() >= 20.0) {

            QJsonObject postObj;
            postObj.insert("idaccount", 1);
            postObj.insert("transactions", "1");
            postObj.insert("amount", -20);
            postObj.insert("date", QDateTime::currentDateTime().toString(Qt::ISODate)); // Add current date

            QString post_url = "http://localhost:3000/accountinformation/create";
            QNetworkRequest postRequest(post_url);
            postRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

            //WEBTOKEN START
            QByteArray myToken="Bearer "+webToken;
            postRequest.setRawHeader(QByteArray("Authorization"),(myToken));
            //WEBTOKEN END

            postManager = new QNetworkAccessManager(this);
            connect(postManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(post20Slot(QNetworkReply*)));

            QJsonDocument postDoc(postObj);
            QByteArray postData = postDoc.toJson();

            postReply = postManager->post(postRequest, postData);
        } else {
            qDebug() << "Tilillä ei ole tarpeeksi katetta";  
        }
    } else {
        qDebug() << "Virhe GET-pyynnön suorittamisessa: " << getReply->errorString();
    }

    // Free resources
    getReply->deleteLater();
    getManager->deleteLater();
}
}

void moneySelect::post20Slot(QNetworkReply *postReply)
{
    postResponse_data=postReply->readAll();
    qDebug()<<postResponse_data;
    postReply->deleteLater();
    postManager->deleteLater();
}

void moneySelect::twentyEuroClickedPutCredit()
{
    // PUT-method

    if (cardNumber != "-06000621FE") {
        return;
    }

    QJsonObject putObj;
    putObj.insert("credit_limit",20);

    QString put_url="http://localhost:3000/accounts/updatecredit";
    QNetworkRequest putRequest((put_url));
    putRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    //WEBTOKEN START
    QByteArray myToken="Bearer "+webToken;
    putRequest.setRawHeader(QByteArray("Authorization"),(myToken));
    //WEBTOKEN END

    putManagerCredit = new QNetworkAccessManager(this);
    connect(putManagerCredit, SIGNAL(finished(QNetworkReply*)), this, SLOT(put20SlotCredit(QNetworkReply*)));

    QJsonDocument putDoc(putObj);
    QByteArray putData = putDoc.toJson();

    putReplyCredit = putManagerCredit->put(putRequest, putData);
}

void moneySelect::put20SlotCredit(QNetworkReply *putReplyCredit)
{
    putResponse_dataCredit=putReplyCredit->readAll();
    qDebug()<<putResponse_dataCredit;
    putReplyCredit->deleteLater();
    putManagerCredit->deleteLater();

    // Parse the response into JSON format
    QJsonDocument jsonResponse = QJsonDocument::fromJson(putResponse_dataCredit);
    QJsonObject jsonObject = jsonResponse.object();

    int changed=jsonObject["changedRows"].toInt();
    qDebug()<<"Changed="<<changed;

    if (changed == 0)
    {   // The account balance would go negative, cannot withdraw money.
        ui->chooseLabel->setText("Tilillä ei ole tarpeeksi katetta");
    } else {
        // The withdrawal was successful, and the account balance remains positive
        ui->chooseLabel->setText("Nosto onnistui");
    }
}

void moneySelect::twentyEuroClickedGetCredit()
{
    if (cardNumber != "-06000621FE") {
        qDebug() << "Kortin numero ei ole sallittu.";
        return;
    }
    // Perform a GET request to retrieve the account balance
    QString site_url = "http://localhost:3000/accounts/getcreditlimit/4";
    QNetworkRequest getRequest(site_url);

    //WEBTOKEN START
    QByteArray myToken="Bearer "+webToken;
    getRequest.setRawHeader(QByteArray("Authorization"),(myToken));
    //WEBTOKEN END

    getManagerCredit = new QNetworkAccessManager(this);
    connect(getManagerCredit, SIGNAL(finished(QNetworkReply*)), this, SLOT(twentyEuroClickedPostCredit(QNetworkReply*)));
    getReplyCredit = getManagerCredit->get(getRequest);
}

void moneySelect::twentyEuroClickedPostCredit(QNetworkReply *getReplyCredit)
{
    // Ensure that the response is available and there is no error
    if (getReplyCredit->error() == QNetworkReply::NoError) {
        QByteArray response_data = getReplyCredit->readAll();
        qDebug() << "GET-pyynnön vastaus: " << response_data;

        // Handle the response in JSON format
        QJsonDocument json_doc = QJsonDocument::fromJson(response_data);

        if (json_doc.isObject()) {
            QJsonObject json_obj = json_doc.object();
            QString credit_limit = json_obj.value("credit_limit").toString();
            qDebug() << "Current credit limit: " << credit_limit;

            // Check the account balance and make the POST request only if the balance is sufficient
            if (credit_limit.toDouble() >= -80.0) {

                QJsonObject postObj;
                postObj.insert("idaccount", 4);
                postObj.insert("transactions", "1");
                postObj.insert("amount", -20);
                postObj.insert("date", QDateTime::currentDateTime().toString(Qt::ISODate)); // Add current date

                QString post_url = "http://localhost:3000/accountinformation/createcredit";
                QNetworkRequest postRequest(post_url);
                postRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

                //WEBTOKEN START
                QByteArray myToken="Bearer "+webToken;
                postRequest.setRawHeader(QByteArray("Authorization"),(myToken));
                //WEBTOKEN END

                postManagerCredit = new QNetworkAccessManager(this);
                connect(postManagerCredit, SIGNAL(finished(QNetworkReply*)), this, SLOT(post20SlotCredit(QNetworkReply*)));

                QJsonDocument postDoc(postObj);
                QByteArray postData = postDoc.toJson();

                postReplyCredit = postManagerCredit->post(postRequest, postData);
            } else {
                qDebug() << "Tilillä ei ole tarpeeksi katetta";
            }
        } else {
            qDebug() << "Virhe GET-pyynnön suorittamisessa: " << getReplyCredit->errorString();
        }

        // Free resources
        getReplyCredit->deleteLater();
        getManagerCredit->deleteLater();
    }
}

void moneySelect::post20SlotCredit(QNetworkReply *postReplyCredit)
{
    postResponse_dataCredit=postReplyCredit->readAll();
    qDebug()<<postResponse_dataCredit;
    postManagerCredit->deleteLater();
    postReplyCredit->deleteLater();
}

void moneySelect::fortyEuroClickedPut()
{
    if (cardNumber != "-0600062093") {
        return;
    }
    QJsonObject jsonObj;
    jsonObj.insert("account_balance",40);

    QString site_url="http://localhost:3000/accounts/update";
    QNetworkRequest request((site_url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    //WEBTOKEN START
    QByteArray myToken="Bearer "+webToken;
    request.setRawHeader(QByteArray("Authorization"),(myToken));
    //WEBTOKEN END

    putManager = new QNetworkAccessManager(this);
    connect(putManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(put40Slot(QNetworkReply*)));

    putReply = putManager->put(request, QJsonDocument(jsonObj).toJson());
}

void moneySelect::put40Slot(QNetworkReply *PutReply)
{
    putResponse_data=PutReply->readAll();
    qDebug()<<putResponse_data;
    putReply->deleteLater();
    putManager->deleteLater();

    QJsonDocument jsonResponse = QJsonDocument::fromJson(putResponse_data);
    QJsonObject jsonObject = jsonResponse.object();

    int changed=jsonObject["changedRows"].toInt();
    qDebug()<<"Changed="<<changed;

    if (changed == 0)
    {
        ui->chooseLabel->setText("Tilillä ei ole tarpeeksi katetta");
    } else {
        ui->chooseLabel->setText("Nosto onnistui");
    }
}

void moneySelect::fortyEuroClickedGet()
{
    if (cardNumber != "-0600062093") {
        return;
    }
    QString site_url = "http://localhost:3000/accounts/getaccountbalance/1";
    QNetworkRequest getRequest(site_url);

    //WEBTOKEN START
    QByteArray myToken="Bearer "+webToken;
    getRequest.setRawHeader(QByteArray("Authorization"),(myToken));
    //WEBTOKEN END

    getManager = new QNetworkAccessManager(this);
    connect(getManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(fortyEuroClickedPost(QNetworkReply*)));
    getReply = getManager->get(getRequest);
}

void moneySelect::fortyEuroClickedPost(QNetworkReply *getReply)
{
    if (getReply->error() == QNetworkReply::NoError) {
        QByteArray response_data = getReply->readAll();
        qDebug() << "GET-pyynnön vastaus: " << response_data;

        QJsonDocument json_doc = QJsonDocument::fromJson(response_data);

        if (json_doc.isObject()) {
            QJsonObject json_obj = json_doc.object();
            QString account_balance = json_obj.value("account_balance").toString();
            qDebug() << "Current account balance: " << account_balance;

        if (account_balance.toDouble() >= 40.0) {

            QJsonObject postObj;
            postObj.insert("idaccount", 1);
            postObj.insert("transactions", "1");
            postObj.insert("amount", -40);
            postObj.insert("date", QDateTime::currentDateTime().toString(Qt::ISODate));

            QString post_url = "http://localhost:3000/accountinformation/create";
            QNetworkRequest postRequest(post_url);
            postRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

            //WEBTOKEN START
            QByteArray myToken="Bearer "+webToken;
            postRequest.setRawHeader(QByteArray("Authorization"),(myToken));
            //WEBTOKEN END

            postManager = new QNetworkAccessManager(this);
            connect(postManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(post40Slot(QNetworkReply*)));

            QJsonDocument postDoc(postObj);
            QByteArray postData = postDoc.toJson();

            postReply = postManager->post(postRequest, postData);
        } else {
            qDebug() << "Tilillä ei ole tarpeeksi katetta";
        }
    } else {
        qDebug() << "Virhe GET-pyynnön suorittamisessa: " << getReply->errorString();
    }
    getReply->deleteLater();
    getManager->deleteLater();
 }
}

void moneySelect::post40Slot(QNetworkReply *postReply)
{
    postResponse_data=postReply->readAll();
    qDebug()<<postResponse_data;
    postReply->deleteLater();
    postManager->deleteLater();
}

void moneySelect::fortyEuroClickedPutCredit()
{
    // PUT-method

    if (cardNumber != "-06000621FE") {
        return;
    }

    QJsonObject putObj;
    putObj.insert("credit_limit",40);

    QString put_url="http://localhost:3000/accounts/updatecredit";
    QNetworkRequest putRequest((put_url));
    putRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    //WEBTOKEN START
    QByteArray myToken="Bearer "+webToken;
    putRequest.setRawHeader(QByteArray("Authorization"),(myToken));
    //WEBTOKEN END

    putManagerCredit = new QNetworkAccessManager(this);
    connect(putManagerCredit, SIGNAL(finished(QNetworkReply*)), this, SLOT(put40SlotCredit(QNetworkReply*)));

    QJsonDocument putDoc(putObj);
    QByteArray putData = putDoc.toJson();

    putReplyCredit = putManagerCredit->put(putRequest, putData);
}

void moneySelect::put40SlotCredit(QNetworkReply *putReplyCredit)
{
    putResponse_dataCredit=putReplyCredit->readAll();
    qDebug()<<putResponse_dataCredit;
    putReplyCredit->deleteLater();
    putManagerCredit->deleteLater();

    // Parse the response into JSON format
    QJsonDocument jsonResponse = QJsonDocument::fromJson(putResponse_dataCredit);
    QJsonObject jsonObject = jsonResponse.object();

    int changed=jsonObject["changedRows"].toInt();
    qDebug()<<"Changed="<<changed;

    if (changed == 0)
    {   // The account balance would go negative, cannot withdraw money.
        ui->chooseLabel->setText("Tilillä ei ole tarpeeksi katetta");
    } else {
        // The withdrawal was successful, and the account balance remains positive
        ui->chooseLabel->setText("Nosto onnistui");
    }
}

void moneySelect::fortyEuroClickedGetCredit()
{
    if (cardNumber != "-06000621FE") {
        qDebug() << "Kortin numero ei ole sallittu.";
        return;
    }
    // Perform a GET request to retrieve the account balance
    QString site_url = "http://localhost:3000/accounts/getcreditlimit/4";
    QNetworkRequest getRequest(site_url);

    //WEBTOKEN START
    QByteArray myToken="Bearer "+webToken;
    getRequest.setRawHeader(QByteArray("Authorization"),(myToken));
    //WEBTOKEN END

    getManagerCredit = new QNetworkAccessManager(this);
    connect(getManagerCredit, SIGNAL(finished(QNetworkReply*)), this, SLOT(fortyEuroClickedPostCredit(QNetworkReply*)));
    getReplyCredit = getManagerCredit->get(getRequest);
}

void moneySelect::fortyEuroClickedPostCredit(QNetworkReply *getReplyCredit)
{
    // Ensure that the response is available and there is no error
    if (getReplyCredit->error() == QNetworkReply::NoError) {
        QByteArray response_data = getReplyCredit->readAll();
        qDebug() << "GET-pyynnön vastaus: " << response_data;

        // Handle the response in JSON format
        QJsonDocument json_doc = QJsonDocument::fromJson(response_data);

        if (json_doc.isObject()) {
            QJsonObject json_obj = json_doc.object();
            QString credit_limit = json_obj.value("credit_limit").toString();
            qDebug() << "Current credit limit: " << credit_limit;

            // Check the account balance and make the POST request only if the balance is sufficient
            if (credit_limit.toDouble() >= -60.0) {

                QJsonObject postObj;
                postObj.insert("idaccount", 4);
                postObj.insert("transactions", "1");
                postObj.insert("amount", -40);
                postObj.insert("date", QDateTime::currentDateTime().toString(Qt::ISODate)); // Add current date

                QString post_url = "http://localhost:3000/accountinformation/createcredit";
                QNetworkRequest postRequest(post_url);
                postRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

                //WEBTOKEN START
                QByteArray myToken="Bearer "+webToken;
                postRequest.setRawHeader(QByteArray("Authorization"),(myToken));
                //WEBTOKEN END

                postManagerCredit = new QNetworkAccessManager(this);
                connect(postManagerCredit, SIGNAL(finished(QNetworkReply*)), this, SLOT(post40SlotCredit(QNetworkReply*)));

                QJsonDocument postDoc(postObj);
                QByteArray postData = postDoc.toJson();

                postReplyCredit = postManagerCredit->post(postRequest, postData);
            } else {
                qDebug() << "Tilillä ei ole tarpeeksi katetta";
            }
        } else {
            qDebug() << "Virhe GET-pyynnön suorittamisessa: " << getReplyCredit->errorString();
        }

        // Free resources
        getReplyCredit->deleteLater();
        getManagerCredit->deleteLater();
    }
}

void moneySelect::post40SlotCredit(QNetworkReply *postReplyCredit)
{
    postResponse_dataCredit=postReplyCredit->readAll();
    qDebug()<<postResponse_dataCredit;
    postManagerCredit->deleteLater();
    postReplyCredit->deleteLater();
}


void moneySelect::fiftyEuroClickedPut()
{
    if (cardNumber != "-0600062093") {
        return;
    }
    QJsonObject jsonObj;
    jsonObj.insert("account_balance",50);

    QString site_url="http://localhost:3000/accounts/update";
    QNetworkRequest request((site_url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    //WEBTOKEN START
    QByteArray myToken="Bearer "+webToken;
    request.setRawHeader(QByteArray("Authorization"),(myToken));
    //WEBTOKEN END

    putManager = new QNetworkAccessManager(this);
    connect(putManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(put50Slot(QNetworkReply*)));

    putReply = putManager->put(request, QJsonDocument(jsonObj).toJson());
}

void moneySelect::put50Slot(QNetworkReply *putReply)
{
    putResponse_data=putReply->readAll();
    qDebug()<<putResponse_data;
    putReply->deleteLater();
    putManager->deleteLater();

    QJsonDocument jsonResponse = QJsonDocument::fromJson(putResponse_data);
    QJsonObject jsonObject = jsonResponse.object();

    int changed=jsonObject["changedRows"].toInt();
    qDebug()<<"Changed="<<changed;

    if (changed == 0)
    {
        ui->chooseLabel->setText("Tilillä ei ole tarpeeksi katetta");
    } else {
        ui->chooseLabel->setText("Nosto onnistui");
    }
 }

void moneySelect::fiftyEuroClickedGet()
{
    if (cardNumber != "-0600062093") {
        return;
    }
    QString site_url = "http://localhost:3000/accounts/getaccountbalance/1";
    QNetworkRequest getRequest(site_url);

    //WEBTOKEN START
    QByteArray myToken="Bearer "+webToken;
    getRequest.setRawHeader(QByteArray("Authorization"),(myToken));
    //WEBTOKEN END

    getManager = new QNetworkAccessManager(this);
    connect(getManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(fiftyEuroClickedPost(QNetworkReply*)));
    getReply = getManager->get(getRequest);
}

void moneySelect::fiftyEuroClickedPost(QNetworkReply *getReply)
{
    if (getReply->error() == QNetworkReply::NoError) {
        QByteArray response_data = getReply->readAll();
        qDebug() << "GET-pyynnön vastaus: " << response_data;

        QJsonDocument json_doc = QJsonDocument::fromJson(response_data);

        if (json_doc.isObject()) {
            QJsonObject json_obj = json_doc.object();
            QString account_balance = json_obj.value("account_balance").toString();
            qDebug() << "Current account balance: " << account_balance;

        if (account_balance.toDouble() >= 50.0) {

            QJsonObject postObj;
            postObj.insert("idaccount", 1);
            postObj.insert("transactions", "1");
            postObj.insert("amount", -50);
            postObj.insert("date", QDateTime::currentDateTime().toString(Qt::ISODate));

            QString post_url = "http://localhost:3000/accountinformation/create";
            QNetworkRequest postRequest(post_url);
            postRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

            //WEBTOKEN START
            QByteArray myToken="Bearer "+webToken;
            postRequest.setRawHeader(QByteArray("Authorization"),(myToken));
            //WEBTOKEN END

            postManager = new QNetworkAccessManager(this);
            connect(postManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(post50Slot(QNetworkReply*)));

            QJsonDocument postDoc(postObj);
            QByteArray postData = postDoc.toJson();

            postReply = postManager->post(postRequest, postData);
        } else {
            qDebug() << "Tilillä ei ole tarpeeksi katetta";
        }
    } else {
        qDebug() << "Virhe GET-pyynnön suorittamisessa: " << getReply->errorString();
    }
    getReply->deleteLater();
    getManager->deleteLater();
}
}

void moneySelect::post50Slot(QNetworkReply *postReply)
{
    postResponse_data=postReply->readAll();
    qDebug()<<postResponse_data;
    postReply->deleteLater();
    postManager->deleteLater();
}

void moneySelect::fiftyEuroClickedPutCredit()
{
    // PUT-method

    if (cardNumber != "-06000621FE") {
        return;
    }

    QJsonObject putObj;
    putObj.insert("credit_limit",50);

    QString put_url="http://localhost:3000/accounts/updatecredit";
    QNetworkRequest putRequest((put_url));
    putRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    //WEBTOKEN START
    QByteArray myToken="Bearer "+webToken;
    putRequest.setRawHeader(QByteArray("Authorization"),(myToken));
    //WEBTOKEN END

    putManagerCredit = new QNetworkAccessManager(this);
    connect(putManagerCredit, SIGNAL(finished(QNetworkReply*)), this, SLOT(put50SlotCredit(QNetworkReply*)));

    QJsonDocument putDoc(putObj);
    QByteArray putData = putDoc.toJson();

    putReplyCredit = putManagerCredit->put(putRequest, putData);
}

void moneySelect::put50SlotCredit(QNetworkReply *putReplyCredit)
{
    putResponse_dataCredit=putReplyCredit->readAll();
    qDebug()<<putResponse_dataCredit;
    putReplyCredit->deleteLater();
    putManagerCredit->deleteLater();

    // Parse the response into JSON format
    QJsonDocument jsonResponse = QJsonDocument::fromJson(putResponse_dataCredit);
    QJsonObject jsonObject = jsonResponse.object();

    int changed=jsonObject["changedRows"].toInt();
    qDebug()<<"Changed="<<changed;

    if (changed == 0)
    {   // The account balance would go negative, cannot withdraw money.
        ui->chooseLabel->setText("Tilillä ei ole tarpeeksi katetta");
    } else {
        // The withdrawal was successful, and the account balance remains positive
        ui->chooseLabel->setText("Nosto onnistui");
    }
}

void moneySelect::fiftyEuroClickedGetCredit()
{
    if (cardNumber != "-06000621FE") {
        qDebug() << "Kortin numero ei ole sallittu.";
        return;
    }
    // Perform a GET request to retrieve the account balance
    QString site_url = "http://localhost:3000/accounts/getcreditlimit/4";
    QNetworkRequest getRequest(site_url);

    //WEBTOKEN START
    QByteArray myToken="Bearer "+webToken;
    getRequest.setRawHeader(QByteArray("Authorization"),(myToken));
    //WEBTOKEN END

    getManagerCredit = new QNetworkAccessManager(this);
    connect(getManagerCredit, SIGNAL(finished(QNetworkReply*)), this, SLOT(fiftyEuroClickedPostCredit(QNetworkReply*)));
    getReplyCredit = getManagerCredit->get(getRequest);
}

void moneySelect::fiftyEuroClickedPostCredit(QNetworkReply *getReplyCredit)
{
    // Ensure that the response is available and there is no error
    if (getReplyCredit->error() == QNetworkReply::NoError) {
        QByteArray response_data = getReplyCredit->readAll();
        qDebug() << "GET-pyynnön vastaus: " << response_data;

        // Handle the response in JSON format
        QJsonDocument json_doc = QJsonDocument::fromJson(response_data);

        if (json_doc.isObject()) {
            QJsonObject json_obj = json_doc.object();
            QString credit_limit = json_obj.value("credit_limit").toString();
            qDebug() << "Current credit limit: " << credit_limit;

            // Check the account balance and make the POST request only if the balance is sufficient
            if (credit_limit.toDouble() >= -50.0) {

                QJsonObject postObj;
                postObj.insert("idaccount", 4);
                postObj.insert("transactions", "1");
                postObj.insert("amount", -50);
                postObj.insert("date", QDateTime::currentDateTime().toString(Qt::ISODate)); // Add current date

                QString post_url = "http://localhost:3000/accountinformation/createcredit";
                QNetworkRequest postRequest(post_url);
                postRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

                //WEBTOKEN START
                QByteArray myToken="Bearer "+webToken;
                postRequest.setRawHeader(QByteArray("Authorization"),(myToken));
                //WEBTOKEN END

                postManagerCredit = new QNetworkAccessManager(this);
                connect(postManagerCredit, SIGNAL(finished(QNetworkReply*)), this, SLOT(post50SlotCredit(QNetworkReply*)));

                QJsonDocument postDoc(postObj);
                QByteArray postData = postDoc.toJson();

                postReplyCredit = postManagerCredit->post(postRequest, postData);
            } else {
                qDebug() << "Tilillä ei ole tarpeeksi katetta";
            }
        } else {
            qDebug() << "Virhe GET-pyynnön suorittamisessa: " << getReplyCredit->errorString();
        }

        // Free resources
        getReplyCredit->deleteLater();
        getManagerCredit->deleteLater();
    }
}

void moneySelect::post50SlotCredit(QNetworkReply *postReplyCredit)
{
    postResponse_dataCredit=postReplyCredit->readAll();
    qDebug()<<postResponse_dataCredit;
    postManagerCredit->deleteLater();
    postReplyCredit->deleteLater();
}

void moneySelect::hundredEuroClickedPut()
{
    if (cardNumber != "-0600062093") {
        return;
    }
    QJsonObject jsonObj;
    jsonObj.insert("account_balance",100);

    QString site_url="http://localhost:3000/accounts/update";
    QNetworkRequest request((site_url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    //WEBTOKEN START
    QByteArray myToken="Bearer "+webToken;
    request.setRawHeader(QByteArray("Authorization"),(myToken));
    //WEBTOKEN END

    putManager = new QNetworkAccessManager(this);
    connect(putManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(put100Slot(QNetworkReply*)));

    putReply = putManager->put(request, QJsonDocument(jsonObj).toJson());
}

void moneySelect::put100Slot(QNetworkReply *putReply)
{
    putResponse_data=putReply->readAll();
    qDebug()<<putResponse_data;
    putReply->deleteLater();
    putManager->deleteLater();

    QJsonDocument jsonResponse = QJsonDocument::fromJson(putResponse_data);
    QJsonObject jsonObject = jsonResponse.object();

    int changed=jsonObject["changedRows"].toInt();
    qDebug()<<"Changed="<<changed;

    if (changed == 0)
    {
        ui->chooseLabel->setText("Tilillä ei ole tarpeeksi katetta");
    } else {
        ui->chooseLabel->setText("Nosto onnistui");
    }
}

void moneySelect::hundredEuroClickedGet()
{
    if (cardNumber != "-0600062093") {
        return;
    }
    QString site_url = "http://localhost:3000/accounts/getaccountbalance/1";
    QNetworkRequest getRequest(site_url);

    //WEBTOKEN START
    QByteArray myToken="Bearer "+webToken;
    getRequest.setRawHeader(QByteArray("Authorization"),(myToken));
    //WEBTOKEN END

    getManager = new QNetworkAccessManager(this);
    connect(getManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(hundredEuroClickedPost(QNetworkReply*)));
    getReply = getManager->get(getRequest);
}

void moneySelect::hundredEuroClickedPost(QNetworkReply *getReply)
{
    if (getReply->error() == QNetworkReply::NoError) {
        QByteArray response_data = getReply->readAll();
        qDebug() << "GET-pyynnön vastaus: " << response_data;

        QJsonDocument json_doc = QJsonDocument::fromJson(response_data);

        if (json_doc.isObject()) {
            QJsonObject json_obj = json_doc.object();
            QString account_balance = json_obj.value("account_balance").toString();
            qDebug() << "Current account balance: " << account_balance;

        if (account_balance.toDouble() >= 100.0) {

            QJsonObject postObj;
            postObj.insert("idaccount", 1);
            postObj.insert("transactions", "1");
            postObj.insert("amount", -100);
            postObj.insert("date", QDateTime::currentDateTime().toString(Qt::ISODate));

            QString post_url = "http://localhost:3000/accountinformation/create";
            QNetworkRequest postRequest(post_url);
            postRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

            //WEBTOKEN START
            QByteArray myToken="Bearer "+webToken;
            postRequest.setRawHeader(QByteArray("Authorization"),(myToken));
            //WEBTOKEN END

            postManager = new QNetworkAccessManager(this);
            connect(postManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(post100Slot(QNetworkReply*)));

            QJsonDocument postDoc(postObj);
            QByteArray postData = postDoc.toJson();

            postReply = postManager->post(postRequest, postData);
        } else {
            qDebug() << "Tilillä ei ole tarpeeksi katetta";
        }
    } else {
        qDebug() << "Virhe GET-pyynnön suorittamisessa: " << getReply->errorString();
    }
    getReply->deleteLater();
    getManager->deleteLater();
}
}

void moneySelect::post100Slot(QNetworkReply *postReply)
{
    postResponse_data=postReply->readAll();
    qDebug()<<postResponse_data;
    postReply->deleteLater();
    postManager->deleteLater();
}

void moneySelect::hundredEuroClickedPutCredit()
{
    // PUT-method

    if (cardNumber != "-06000621FE") {
        return;
    }

    QJsonObject putObj;
    putObj.insert("credit_limit",100);

    QString put_url="http://localhost:3000/accounts/updatecredit";
    QNetworkRequest putRequest((put_url));
    putRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    //WEBTOKEN START
    QByteArray myToken="Bearer "+webToken;
    putRequest.setRawHeader(QByteArray("Authorization"),(myToken));
    //WEBTOKEN END

    putManagerCredit = new QNetworkAccessManager(this);
    connect(putManagerCredit, SIGNAL(finished(QNetworkReply*)), this, SLOT(put100SlotCredit(QNetworkReply*)));

    QJsonDocument putDoc(putObj);
    QByteArray putData = putDoc.toJson();

    putReplyCredit = putManagerCredit->put(putRequest, putData);
}

void moneySelect::put100SlotCredit(QNetworkReply *putReplyCredit)
{
    putResponse_dataCredit=putReplyCredit->readAll();
    qDebug()<<putResponse_dataCredit;
    putReplyCredit->deleteLater();
    putManagerCredit->deleteLater();

    // Parse the response into JSON format
    QJsonDocument jsonResponse = QJsonDocument::fromJson(putResponse_dataCredit);
    QJsonObject jsonObject = jsonResponse.object();

    int changed=jsonObject["changedRows"].toInt();
    qDebug()<<"Changed="<<changed;

    if (changed == 0)
    {   // The account balance would go negative, cannot withdraw money.
        ui->chooseLabel->setText("Tilillä ei ole tarpeeksi katetta");
    } else {
        // The withdrawal was successful, and the account balance remains positive
        ui->chooseLabel->setText("Nosto onnistui");
    }
}

void moneySelect::hundredEuroClickedGetCredit()
{
    if (cardNumber != "-06000621FE") {
        qDebug() << "Kortin numero ei ole sallittu.";
        return;
    }
    // Perform a GET request to retrieve the account balance
    QString site_url = "http://localhost:3000/accounts/getcreditlimit/4";
    QNetworkRequest getRequest(site_url);

    //WEBTOKEN START
    QByteArray myToken="Bearer "+webToken;
    getRequest.setRawHeader(QByteArray("Authorization"),(myToken));
    //WEBTOKEN END

    getManagerCredit = new QNetworkAccessManager(this);
    connect(getManagerCredit, SIGNAL(finished(QNetworkReply*)), this, SLOT(hundredEuroClickedPostCredit(QNetworkReply*)));
    getReplyCredit = getManagerCredit->get(getRequest);
}

void moneySelect::hundredEuroClickedPostCredit(QNetworkReply *getReplyCredit)
{
    // Ensure that the response is available and there is no error
    if (getReplyCredit->error() == QNetworkReply::NoError) {
        QByteArray response_data = getReplyCredit->readAll();
        qDebug() << "GET-pyynnön vastaus: " << response_data;

        // Handle the response in JSON format
        QJsonDocument json_doc = QJsonDocument::fromJson(response_data);

        if (json_doc.isObject()) {
            QJsonObject json_obj = json_doc.object();
            QString credit_limit = json_obj.value("credit_limit").toString();
            qDebug() << "Current credit limit: " << credit_limit;

            // Check the account balance and make the POST request only if the balance is sufficient
            if (credit_limit.toDouble() >= 0) {

                QJsonObject postObj;
                postObj.insert("idaccount", 4);
                postObj.insert("transactions", "1");
                postObj.insert("amount", -100);
                postObj.insert("date", QDateTime::currentDateTime().toString(Qt::ISODate)); // Add current date

                QString post_url = "http://localhost:3000/accountinformation/createcredit";
                QNetworkRequest postRequest(post_url);
                postRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

                //WEBTOKEN START
                QByteArray myToken="Bearer "+webToken;
                postRequest.setRawHeader(QByteArray("Authorization"),(myToken));
                //WEBTOKEN END

                postManagerCredit = new QNetworkAccessManager(this);
                connect(postManagerCredit, SIGNAL(finished(QNetworkReply*)), this, SLOT(post100SlotCredit(QNetworkReply*)));

                QJsonDocument postDoc(postObj);
                QByteArray postData = postDoc.toJson();

                postReplyCredit = postManagerCredit->post(postRequest, postData);
            } else {
                qDebug() << "Tilillä ei ole tarpeeksi katetta";
            }
        } else {
            qDebug() << "Virhe GET-pyynnön suorittamisessa: " << getReplyCredit->errorString();
        }

        // Free resources
        getReplyCredit->deleteLater();
        getManagerCredit->deleteLater();
    }
}

void moneySelect::post100SlotCredit(QNetworkReply *postReplyCredit)
{
    postResponse_dataCredit=postReplyCredit->readAll();
    qDebug()<<postResponse_dataCredit;
    postManagerCredit->deleteLater();
    postReplyCredit->deleteLater();
}


// The following functions are for testing purposes

/*
void moneySelect::insertHundredClickedPut()
{
    if (cardNumber != "-0600062093") {
        return;
    }
    QJsonObject jsonObj;
    jsonObj.insert("account_balance",-100);

    QString site_url="http://localhost:3000/accounts/update";
    QNetworkRequest request((site_url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    //WEBTOKEN START
    QByteArray myToken="Bearer "+webToken;
    request.setRawHeader(QByteArray("Authorization"),(myToken));
    //WEBTOKEN END

    putManager = new QNetworkAccessManager(this);
    connect(putManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(putInsertedMoneySlot(QNetworkReply*)));

    putReply = putManager->put(request, QJsonDocument(jsonObj).toJson());

    ui->chooseLabel->setText("100 € lisätty tilille");
}

void moneySelect::putInsertedMoneySlot(QNetworkReply *putReply)
{
    putResponse_data=putReply->readAll();
    qDebug()<<putResponse_data;
    putReply->deleteLater();
    putManager->deleteLater();
}

void moneySelect::insertHundredClickedPost()
{
    if (cardNumber != "-0600062093") {
        return;
    }
    QJsonObject postObj;
    postObj.insert("idaccount", 1);
    postObj.insert("transactions", "1");
    postObj.insert("amount", +100);
    postObj.insert("date", QDateTime::currentDateTime().toString(Qt::ISODate));

    QString post_url = "http://localhost:3000/accountinformation/create";
    QNetworkRequest postRequest(post_url);
    postRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    //WEBTOKEN START
    QByteArray myToken="Bearer "+webToken;
    postRequest.setRawHeader(QByteArray("Authorization"),(myToken));
    //WEBTOKEN END

    postManager = new QNetworkAccessManager(this);
    connect(postManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(postInsertedMoneySlot(QNetworkReply*)));

    QJsonDocument postDoc(postObj);
    QByteArray postData = postDoc.toJson();

    postReply = postManager->post(postRequest, postData);
}

void moneySelect::postInsertedMoneySlot(QNetworkReply *postReply)
{
    postResponse_data=postReply->readAll();
    qDebug()<<postResponse_data;
    postReply->deleteLater();
    postManager->deleteLater();
}

void moneySelect::insertHundredClickedPutCredit()
{
    if (cardNumber != "-06000621FE") {
        return;
    }
    QJsonObject jsonObj;
    jsonObj.insert("credit_limit",-100);

    QString site_url="http://localhost:3000/accounts/updatecredit";
    QNetworkRequest request((site_url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    //WEBTOKEN START
    QByteArray myToken="Bearer "+webToken;
    request.setRawHeader(QByteArray("Authorization"),(myToken));
    //WEBTOKEN END

    putManagerCredit = new QNetworkAccessManager(this);
    connect(putManagerCredit, SIGNAL(finished(QNetworkReply*)), this, SLOT(putInsertedMoneySlotCredit(QNetworkReply*)));

    putReplyCredit = putManagerCredit->put(request, QJsonDocument(jsonObj).toJson());

    ui->chooseLabel->setText("100 € lisätty tilille");
}

void moneySelect::putInsertedMoneySlotCredit(QNetworkReply *putReplyCredit)
{
    putResponse_dataCredit=putReplyCredit->readAll();
    qDebug()<<putResponse_dataCredit;
    putReplyCredit->deleteLater();
    putManagerCredit->deleteLater();
}

void moneySelect::insertHundredClickedPostCredit()
{
    if (cardNumber != "-06000621FE") {
        return;
    }
    QJsonObject postObj;
    postObj.insert("idaccount", 4);
    postObj.insert("transactions", "1");
    postObj.insert("amount", +100);
    postObj.insert("date", QDateTime::currentDateTime().toString(Qt::ISODate));

    QString post_url = "http://localhost:3000/accountinformation/createcredit";
    QNetworkRequest postRequest(post_url);
    postRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    //WEBTOKEN START
    QByteArray myToken="Bearer "+webToken;
    postRequest.setRawHeader(QByteArray("Authorization"),(myToken));
    //WEBTOKEN END

    postManagerCredit = new QNetworkAccessManager(this);
    connect(postManagerCredit, SIGNAL(finished(QNetworkReply*)), this, SLOT(postInsertedMoneySlotCredit(QNetworkReply*)));

    QJsonDocument postDoc(postObj);
    QByteArray postData = postDoc.toJson();

    postReplyCredit = postManagerCredit->post(postRequest, postData);
}

void moneySelect::postInsertedMoneySlotCredit(QNetworkReply *postReplyCredit)
{
    postResponse_dataCredit=postReplyCredit->readAll();
    qDebug()<<postResponse_dataCredit;
    postReplyCredit->deleteLater();
    postManagerCredit->deleteLater();
}*/
