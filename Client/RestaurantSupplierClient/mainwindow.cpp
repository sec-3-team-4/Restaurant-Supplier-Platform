#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpSocket>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);

    qDebug() << "Client started...";

    // ================= CONNECTION STATUS =================
    connect(socket, &QTcpSocket::connected, this, [=]() {
        qDebug() << "CONNECTED";

        ui->connectionLabel->setText("Connected");
        ui->connectionLabel->setStyleSheet("color: green;");
    });

    connect(socket, &QTcpSocket::disconnected, this, [=]() {
        qDebug() << "DISCONNECTED";

        ui->connectionLabel->setText("Disconnected");
        ui->connectionLabel->setStyleSheet("color: red;");
    });

    // ================= RECEIVE SERVER RESPONSE =================
    connect(socket, &QTcpSocket::readyRead, this, [=]() {

        QByteArray responseData = socket->readAll();

        qDebug() << "RAW RESPONSE:" << responseData;

        QJsonDocument doc = QJsonDocument::fromJson(responseData);

        if (!doc.isNull() && doc.isObject()) {
            handleResponse(doc.object());
        } else {
            qDebug() << "Invalid JSON received";
        }
    });

    // ================= LOGIN BUTTON =================
    connect(ui->loginButton, &QPushButton::clicked,
            this, &MainWindow::on_loginButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ================= CONNECT + LOGIN =================
void MainWindow::on_loginButton_clicked()
{
    qDebug() << "CLICKED → CONNECTING...";

    socket->connectToHost("10.40.51.86", 1234);
}

// ================= LOGIN FUNCTION =================
void MainWindow::handleLogin()
{
    QString username = ui->usernameInput->text();
    QString password = ui->passwordInput->text();

    if (username.isEmpty() || password.isEmpty()) {
        ui->statusLabel->setText("Please fill all fields");
        ui->statusLabel->setStyleSheet("color: red;");
        return;
    }

    if (socket->state() != QAbstractSocket::ConnectedState) {
        ui->statusLabel->setText("Not connected to server");
        ui->statusLabel->setStyleSheet("color: red;");
        qDebug() << "Socket not connected yet";
        return;
    }

    ui->statusLabel->setText("Logging in...");
    ui->statusLabel->setStyleSheet("color: orange;");
    ui->loginButton->setEnabled(false);

    QJsonObject data;
    data["username"] = username;
    data["password"] = password;

    QJsonObject request;
    request["type"] = "login_request";
    request["sender"] = username;
    request["receiver"] = "server";
    request["data"] = data;

    QByteArray jsonData =
        QJsonDocument(request).toJson(QJsonDocument::Compact);

    socket->write(jsonData + "\n");
    socket->flush();

    qDebug() << "Sent login request:" << jsonData;
}

// ================= RESPONSE HANDLER =================
void MainWindow::handleResponse(const QJsonObject &response)
{
    qDebug() << "Parsed response:" << response;

    QString status = response["status"].toString();

    if (status == "success") {
        ui->statusLabel->setText("Login successful");
        ui->statusLabel->setStyleSheet("color: green;");
    } else {
        QString message = response["message"].toString();

        if (message.isEmpty())
            message = "Login failed";

        ui->statusLabel->setText(message);
        ui->statusLabel->setStyleSheet("color: red;");
    }

    ui->loginButton->setEnabled(true);
}