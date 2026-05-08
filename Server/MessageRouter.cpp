#include "MessageRouter.h"

#include "ClientSession.h"

MessageRouter::MessageRouter(UserManager& um)
    : userManager_(um) {}

void MessageRouter::broadcast(const std::string& msg) {
    for (auto& pair : userManager_.getUsers()) {
        pair.second->send(msg);
    }
}

