#include "MessageRouter.h"
#include <iostream>

std::string MessageRouter::route(std::string msg) {
    std::cout << "Routing message: " << msg << std::endl;

    if (msg.find("chat_message") != std::string::npos) {
        return "chat routed";
    }

    if (msg.find("login") != std::string::npos) {
        return "login routed";
    }

    return "unknown message type";
}
