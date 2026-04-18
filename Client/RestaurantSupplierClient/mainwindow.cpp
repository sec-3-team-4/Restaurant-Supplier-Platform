#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);
    socket->connectToHost("127.0.0.1", 1234);

    qDebug() << "Connected to server";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_searchButton_clicked()
{
    QJsonObject obj;
    obj["type"] = "search_request";
    obj["sender"] = "malak";

    QJsonObject data;
    data["category"] = ui->lineEditCategory->text();

    obj["data"] = data;

    QJsonDocument doc(obj);
    socket->write(doc.toJson(QJsonDocument::Compact));

    qDebug() << "Sent:" << doc.toJson(QJsonDocument::Compact);
}
