#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>

class ChatClient : public QObject
{
    Q_OBJECT

public:
    explicit ChatClient(QObject *parent = nullptr);

    void connectToServer(const QString &host, quint16 port);
    void sendChatMessage(const QString &sender,
                         const QString &receiver,
                         const QString &text);

    bool isConnected() const;

signals:
    void connected();
    void disconnected();
    void errorOccurred(const QString &message);
    void chatMessageReceived(const QString &sender, const QString &text);
    void rawJsonReceived(const QString &jsonText);

private slots:
    void onReadyRead();

private:
    QTcpSocket *m_socket;
};

#endif // CHATCLIENT_H
