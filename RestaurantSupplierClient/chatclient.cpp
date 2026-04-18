#include "chatclient.h"

#include <QJsonDocument>
#include <QJsonParseError>
#include <QDateTime>
#include <QAbstractSocket>

ChatClient::ChatClient(QObject *parent)
    : QObject(parent),
    m_socket(new QTcpSocket(this))
{
    connect(m_socket, &QTcpSocket::connected,
            this, &ChatClient::connected);

    connect(m_socket, &QTcpSocket::disconnected,
            this, &ChatClient::disconnected);

    connect(m_socket, &QTcpSocket::readyRead,
            this, &ChatClient::onReadyRead);

    connect(m_socket, &QTcpSocket::errorOccurred, this,
            [this](QAbstractSocket::SocketError) {
                emit errorOccurred(m_socket->errorString());
            });
}

void ChatClient::connectToServer(const QString &host, quint16 port)
{
    m_socket->connectToHost(host, port);
}

bool ChatClient::isConnected() const
{
    return m_socket->state() == QAbstractSocket::ConnectedState;
}

void ChatClient::sendChatMessage(const QString &sender,
                                 const QString &receiver,
                                 const QString &text)
{
    if (!isConnected()) {
        emit errorOccurred("Not connected to server.");
        return;
    }

    QJsonObject dataObject;
    dataObject["text"] = text;

    QJsonObject messageObject;
    messageObject["type"] = "chat_message";
    messageObject["sender"] = sender;
    messageObject["receiver"] = receiver;
    messageObject["timestamp"] = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
    messageObject["data"] = dataObject;

    QJsonDocument doc(messageObject);
    QByteArray payload = doc.toJson(QJsonDocument::Compact);
    payload.append('\n');

    m_socket->write(payload);
}

void ChatClient::onReadyRead()
{
    while (m_socket->canReadLine()) {
        QByteArray line = m_socket->readLine().trimmed();

        emit rawJsonReceived(QString::fromUtf8(line));

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(line, &parseError);

        if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
            emit errorOccurred("Invalid JSON received from server.");
            continue;
        }

        QJsonObject obj = doc.object();
        QString type = obj.value("type").toString();
        QString sender = obj.value("sender").toString();

        if (type == "chat_message") {
            QJsonObject data = obj.value("data").toObject();
            QString text = data.value("text").toString();
            emit chatMessageReceived(sender, text);
        }
    }
}