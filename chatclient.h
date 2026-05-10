#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>

class ChatClient : public QObject {
    Q_OBJECT
public:
    explicit ChatClient(QObject *parent = nullptr);
    void connectToServer(const QString &host, quint16 port);
    void sendMessage(const QJsonObject &json); // Jana: This sends your Chat JSON

signals:
    void connected();    // This is the signal you are connecting to
    void disconnected();
    void messageReceived(const QJsonObject &json);

private slots:
    void onReadyRead(); // The mandatory Async Read Loop for Milestone 3

private:
    QTcpSocket *m_socket; // We use m_socket to avoid naming conflicts
};

#endif