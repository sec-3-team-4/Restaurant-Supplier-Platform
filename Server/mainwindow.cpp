#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    server = new QTcpServer(this);

    connect(server, &QTcpServer::newConnection,
            this, &MainWindow::handleNewConnection);

    server->listen(QHostAddress::Any, 1234);

    qDebug() << "Server started on port 1234";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleNewConnection()
{
    clientSocket = server->nextPendingConnection();

    connect(clientSocket, &QTcpSocket::readyRead,
            this, &MainWindow::readClient);

    qDebug() << "Client connected";
}

void MainWindow::readClient()
{
    QByteArray data = clientSocket->readAll();

    qDebug() << "Raw:" << data;

    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (!doc.isNull() && doc.isObject()) {
        QJsonObject obj = doc.object();

        qDebug() << "Type:" << obj["type"].toString();
        qDebug() << "Sender:" << obj["sender"].toString();
        qDebug() << "Data:" << obj["data"].toObject();
    } else {
        qDebug() << "Invalid JSON received";
    }
}
