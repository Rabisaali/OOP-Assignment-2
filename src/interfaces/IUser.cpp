#include "../../include/interfaces/IUser.h"

IUser::IUser(int id, std::string n, std::string c, std::string e)
    : userID(id), name(n), contact(c), email(e) {}

int IUser::getID() const {
    return userID;
}

std::string IUser::getName() const {
    return name;
}

IUser::~IUser() {}
