#include "UserManager.h"

void UserManager::addUser(std::string username) {
    users[username] = true;
}

void UserManager::removeUser(std::string username) {
    users.erase(username);
}

bool UserManager::isOnline(std::string username) {
    return users.find(username) != users.end();
}
