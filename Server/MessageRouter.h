#ifndef MESSAGEROUTER_H
#define MESSAGEROUTER_H

#include <string>
#include <unordered_map>

class ClientSession;   // forward declare only
class UserManager;

class MessageRouter {
public:
    MessageRouter(UserManager& um);

    void broadcast(const std::string& msg);

private:
    UserManager& userManager_;
};

#endif

