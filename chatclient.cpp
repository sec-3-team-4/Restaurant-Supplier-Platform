#include "chatclient.h"

ChatClient::ChatClient(QObject *parent) : QObject(parent) {
    m_socket = new QTcpSocket(this);
    connect(m_socket, &QTcpSocket::connected, this, &ChatClient::connected);
    connect(m_socket, &QTcpSocket::disconnected, this, &ChatClient::disconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &ChatClient::onReadyRead);
}

void ChatClient::connectToServer(const QString &host, quint16 port) {
    m_socket->connectToHost(host, port);
}

void ChatClient::sendMessage(const QJsonObject &json) {
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        // We add a '\n' or use a specific format so the server knows the message ended
        m_socket->write(QJsonDocument(json).toJson(QJsonDocument::Compact));
    }
}

void ChatClient::onReadyRead() {
    // Jana's Async Read Loop - This is the "brain" that listens to the server
    QByteArray data = m_socket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (!doc.isNull() && doc.isObject()) {
        emit messageReceived(doc.object()); // This sends the data to your UI
    }
} // <-- Don't forget this closing bracket!