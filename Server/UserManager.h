#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <unordered_map>
#include <string>
#include <memory>

class ClientSession;

class UserManager {
public:
    bool addUser(const std::string& name, std::shared_ptr<ClientSession> session) {
        if (users_.count(name)) return false;
        users_[name] = session;
        return true;
    }

    void removeUser(const std::string& name) {
        users_.erase(name);
    }

    bool exists(const std::string& name) {
        return users_.count(name) > 0;
    }

    std::unordered_map<std::string, std::shared_ptr<ClientSession>>& getUsers() {
        return users_;
    }

private:
    std::unordered_map<std::string, std::shared_ptr<ClientSession>> users_;
};

#endif

