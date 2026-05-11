#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <string>
#include <map>

class UserManager {
public:
    void addUser(std::string username);
    void removeUser(std::string username);
    bool isOnline(std::string username);

private:
    std::map<std::string, bool> users;
};

#endif
