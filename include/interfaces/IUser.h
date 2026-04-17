#ifndef IUSER_H
#define IUSER_H

#include <string>


class IUser {
protected:
    int userID;
    std::string name, contact, email;

public:
    IUser(int id = 0, std::string n = "NA", std::string c = "NA", std::string e = "NA");

    // Pure virtual functions
    virtual void displayInfo() const = 0;
    virtual std::string getRole() const = 0;
    virtual void login() const = 0;

    int getID() const;
    std::string getName() const;

    virtual ~IUser();
};

#endif
