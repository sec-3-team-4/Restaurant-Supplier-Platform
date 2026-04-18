#ifndef MESSAGEBUILDER_H
#define MESSAGEBUILDER_H

#include <QJsonObject>
#include <QByteArray>
#include <QString>

class MessageBuilder
{
public:
    static QJsonObject buildRegisterRequest(const QString& username,
                                            const QString& password,
                                            const QString& role);

    static QJsonObject buildOrderRequest(const QString& sender,
                                         const QString& receiver,
                                         const QString& product,
                                         int quantity,
                                         const QString& unit,
                                         double requestedPrice);

    static QJsonObject buildProductInquiry(const QString& sender,
                                           const QString& receiver,
                                           const QString& product,
                                           const QString& question);

    static QByteArray toByteArray(const QJsonObject& jsonObject);

private:
    static QString currentTimestamp();

    static QJsonObject createBaseMessage(const QString& type,
                                         const QString& sender,
                                         const QString& receiver,
                                         const QJsonObject& data);
};

#endif // MESSAGEBUILDER_H