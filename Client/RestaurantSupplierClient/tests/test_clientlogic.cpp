#include <QtTest>
#include <QJsonObject>
#include <QJsonDocument>

#include "../logic/messagebuilder.h"
#include "../logic/inputvalidator.h"
#include "../logic/messageservice.h"
#include "../logic/inetworkclient.h"

class MockNetworkClient : public INetworkClient
{
public:
    bool wasCalled = false;
    QByteArray lastPayload;

    bool sendMessage(const QByteArray& payload) override
    {
        wasCalled = true;
        lastPayload = payload;
        return true;
    }
};

class TestClientLogic : public QObject
{
    Q_OBJECT

private slots:
    void testRegisterRequestJsonBuildsCorrectly();
    void testOrderRequestWithMockNetworkClient();
    void testProductInquiryMessageStructure();
    void testEmptyFieldValidationRejectsSend();
    void testWrongFormatInputValidation();
};

void TestClientLogic::testRegisterRequestJsonBuildsCorrectly()
{
    QJsonObject json =
        MessageBuilder::buildRegisterRequest("joudy",
                                             "123456",
                                             "restaurant");

    QCOMPARE(json["type"].toString(),
             QString("register_request"));

    QCOMPARE(json["sender"].toString(),
             QString("joudy"));

    QCOMPARE(json["receiver"].toString(),
             QString("server"));

    QVERIFY(json.contains("data"));
    QVERIFY(json["data"].isObject());

    QJsonObject data = json["data"].toObject();

    QCOMPARE(data["username"].toString(),
             QString("joudy"));

    QCOMPARE(data["password"].toString(),
             QString("123456"));

    QCOMPARE(data["role"].toString(),
             QString("restaurant"));
}

void TestClientLogic::testOrderRequestWithMockNetworkClient()
{
    MockNetworkClient mockNetwork;

    MessageService service(&mockNetwork);

    bool result =
        service.sendOrderRequest("restaurant_user",
                                 "supplier_user",
                                 "Tomatoes",
                                 20,
                                 "kg",
                                 35.0);

    QVERIFY(result);

    QVERIFY(mockNetwork.wasCalled);

    QVERIFY(!mockNetwork.lastPayload.isEmpty());

    QJsonDocument doc =
        QJsonDocument::fromJson(mockNetwork.lastPayload);

    QVERIFY(doc.isObject());

    QJsonObject json = doc.object();

    QCOMPARE(json["type"].toString(),
             QString("order_request"));

    QCOMPARE(json["sender"].toString(),
             QString("restaurant_user"));

    QCOMPARE(json["receiver"].toString(),
             QString("supplier_user"));

    QVERIFY(json.contains("data"));

    QJsonObject data = json["data"].toObject();

    QCOMPARE(data["product"].toString(),
             QString("Tomatoes"));

    QCOMPARE(data["quantity"].toInt(),
             20);

    QCOMPARE(data["unit"].toString(),
             QString("kg"));

    QCOMPARE(data["requested_price"].toDouble(),
             35.0);
}

void TestClientLogic::testProductInquiryMessageStructure()
{
    QJsonObject json =
        MessageBuilder::buildProductInquiry("restaurant_user",
                                            "supplier_user",
                                            "Cheese",
                                            "Do you have 10 kg available?");

    QCOMPARE(json["type"].toString(),
             QString("product_inquiry"));

    QCOMPARE(json["sender"].toString(),
             QString("restaurant_user"));

    QCOMPARE(json["receiver"].toString(),
             QString("supplier_user"));

    QVERIFY(json.contains("data"));

    QVERIFY(json["data"].isObject());

    QJsonObject data = json["data"].toObject();

    QCOMPARE(data["product"].toString(),
             QString("Cheese"));

    QCOMPARE(data["question"].toString(),
             QString("Do you have 10 kg available?"));
}

void TestClientLogic::testEmptyFieldValidationRejectsSend()
{
    ValidationResult result =
        InputValidator::validateRegister("",
                                         "123456",
                                         "restaurant");

    QVERIFY(!result.success);

    QCOMPARE(result.errorMessage,
             QString("Username cannot be empty."));
}

void TestClientLogic::testWrongFormatInputValidation()
{
    ValidationResult result =
        InputValidator::validateRegister("joudy",
                                         "123456",
                                         "admin");

    QVERIFY(!result.success);

    QCOMPARE(result.errorMessage,
             QString("Role must be either restaurant or supplier."));
}

QTEST_MAIN(TestClientLogic)

#include "test_clientlogic.moc"