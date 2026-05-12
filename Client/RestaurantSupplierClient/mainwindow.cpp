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

    // ================= CONNECTED =================
    connect(socket, &QTcpSocket::connected, this, [=]() {

        qDebug() << "CONNECTED";

        ui->connectionLabel->setText("Connected");
        ui->connectionLabel->setStyleSheet("color: green;");

        ui->statusLabel->setText("Connected to server");
        ui->statusLabel->setStyleSheet("color: green;");

        // send login after connection
        handleLogin();
    });

    // ================= DISCONNECTED =================
    connect(socket, &QTcpSocket::disconnected, this, [=]() {

        qDebug() << "DISCONNECTED";

        ui->connectionLabel->setText("Disconnected");
        ui->connectionLabel->setStyleSheet("color: red;");

        ui->statusLabel->setText("Disconnected from server");
        ui->statusLabel->setStyleSheet("color: red;");

        ui->loginButton->setEnabled(true);
    });

    // ================= ERROR =================
    connect(socket, &QTcpSocket::errorOccurred, this,
            [=](QAbstractSocket::SocketError) {

                qDebug() << "ERROR:" << socket->errorString();

                ui->connectionLabel->setText("Disconnected");
                ui->connectionLabel->setStyleSheet("color: red;");

                ui->statusLabel->setText("Login failed - server not reachable");
                ui->statusLabel->setStyleSheet("color: red;");

                ui->loginButton->setEnabled(true);
            });

    // ================= RECEIVE RESPONSE =================
    connect(socket, &QTcpSocket::readyRead, this, [=]() {

        QByteArray data = socket->readAll();
        qDebug() << "RAW RESPONSE:" << data;

        QJsonDocument doc = QJsonDocument::fromJson(data);

        if (!doc.isNull() && doc.isObject()) {
            handleResponse(doc.object());
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ================= LOGIN BUTTON =================
void MainWindow::on_loginButton_clicked()
{
    QString username = ui->usernameInput->text();
    QString password = ui->passwordInput->text();

    if (username.isEmpty() || password.isEmpty()) {
        ui->statusLabel->setText("Please fill all fields");
        ui->statusLabel->setStyleSheet("color: red;");
        return;
    }

    if (socket->state() == QAbstractSocket::ConnectingState ||
        socket->state() == QAbstractSocket::ConnectedState)
    {
        qDebug() << "Already connecting/connected";
        return;
    }

    qDebug() << "CONNECTING...";

    ui->statusLabel->setText("Connecting...");
    ui->statusLabel->setStyleSheet("color: orange;");

    ui->loginButton->setEnabled(false);

    socket->connectToHost("10.40.51.86", 1234);
}

// ================= LOGIN REQUEST =================
void MainWindow::handleLogin()
{
    if (socket->state() != QAbstractSocket::ConnectedState)
        return;

    QString username = ui->usernameInput->text();
    QString password = ui->passwordInput->text();

    QJsonObject data;
    data["username"] = username;
    data["password"] = password;

    QJsonObject request;
    request["type"] = "login_request";
    request["sender"] = username;
    request["receiver"] = "server";
    request["data"] = data;

    QByteArray json = QJsonDocument(request).toJson(QJsonDocument::Compact);

    socket->write(json + "\n");
    socket->flush();

    qDebug() << "Sent login request:" << json;

    ui->statusLabel->setText("Logging in...");
    ui->statusLabel->setStyleSheet("color: orange;");
}

// ================= RESPONSE HANDLER =================
void MainWindow::handleResponse(const QJsonObject &response)
{
    QString status = response["status"].toString();

    if (status == "success") {
        ui->statusLabel->setText("Login successful");
        ui->statusLabel->setStyleSheet("color: green;");
    } else {
        ui->statusLabel->setText("Login failed");
        ui->statusLabel->setStyleSheet("color: red;");
    }

    ui->loginButton->setEnabled(true);
}