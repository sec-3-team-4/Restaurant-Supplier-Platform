#include "ClientSession.h"
#include "JsonValidator.h"
#include "MessageRouter.h"

void ClientSession::handle(const std::string& raw) {
    if (!JsonValidator::isValid(raw)) {
        send("Invalid JSON");
        return;
    }

    auto j = nlohmann::json::parse(raw);
    std::string type = j["type"];

    if (type == "login") {
        username_ = j["payload"]["username"];
        userManager_.addUser(username_, shared_from_this());
        router_.broadcast(username_ + " joined");
    }
    else if (type == "message") {
        router_.broadcast(username_ + ": " + j["payload"]["text"]);
    }
}

