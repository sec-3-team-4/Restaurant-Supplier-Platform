#include "messagebuilder.h"
#include "messagetypes.h"

#include <QJsonDocument>
#include <QDateTime>

QString MessageBuilder::currentTimestamp()
{
    return QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
}

QJsonObject MessageBuilder::createBaseMessage(const QString& type,
                                              const QString& sender,
                                              const QString& receiver,
                                              const QJsonObject& data)
{
    QJsonObject message;
    message["type"] = type;
    message["sender"] = sender;
    message["receiver"] = receiver;
    message["timestamp"] = currentTimestamp();
    message["data"] = data;
    return message;
}

QJsonObject MessageBuilder::buildRegisterRequest(const QString& username,
                                                 const QString& password,
                                                 const QString& role)
{
    QJsonObject data;
    data["username"] = username;
    data["password"] = password;
    data["role"] = role;

    return createBaseMessage(MessageTypes::RegisterRequest,
                             username,
                             "server",
                             data);
}

QJsonObject MessageBuilder::buildOrderRequest(const QString& sender,
                                              const QString& receiver,
                                              const QString& product,
                                              int quantity,
                                              const QString& unit,
                                              double requestedPrice)
{
    QJsonObject data;
    data["product"] = product;
    data["quantity"] = quantity;
    data["unit"] = unit;
    data["requested_price"] = requestedPrice;

    return createBaseMessage(MessageTypes::OrderRequest,
                             sender,
                             receiver,
                             data);
}

QJsonObject MessageBuilder::buildProductInquiry(const QString& sender,
                                                const QString& receiver,
                                                const QString& product,
                                                const QString& question)
{
    QJsonObject data;
    data["product"] = product;
    data["question"] = question;

    return createBaseMessage(MessageTypes::ProductInquiry,
                             sender,
                             receiver,
                             data);
}

QByteArray MessageBuilder::toByteArray(const QJsonObject& jsonObject)
{
    QJsonDocument doc(jsonObject);
    return doc.toJson(QJsonDocument::Compact);
}