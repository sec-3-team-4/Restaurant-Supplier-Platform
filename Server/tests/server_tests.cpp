#include <gtest/gtest.h>

#include "UserManager.h"
#include "JsonValidator.h"
#include "MessageRouter.h"


TEST(UserManagerTest, AddMalak)
{
    UserManager um;
    um.addUser("Malak");
    EXPECT_TRUE(um.isOnline("Malak"));
}

TEST(UserManagerTest, AddAndRemoveJana)
{
    UserManager um;
    um.addUser("Jana");
    um.removeUser("Jana");
    EXPECT_FALSE(um.isOnline("Jana"));
}

TEST(UserManagerTest, AddSalsabeelIsOnline)
{
    UserManager um;
    um.addUser("Salsabeel");
    EXPECT_TRUE(um.isOnline("Salsabeel"));
}

TEST(JsonValidatorTest, ValidMessageFromJoudy)
{
    JsonValidator v;

    std::string msg = R"({
        "type":"chat_message",
        "sender":"Joudy",
        "receiver":"Malak",
        "data":{"text":"hello"}
    })";

    EXPECT_TRUE(v.isValid(msg));
}

TEST(MessageRouterTest, RouterHandlesJanaMessage)
{
    MessageRouter r;

    std::string msg = R"({
        "type":"chat_message",
        "sender":"Jana"
    })";

    std::string result = r.route(msg);

    EXPECT_FALSE(result.empty());
}
