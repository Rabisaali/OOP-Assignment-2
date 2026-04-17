#ifndef IVEHICLE_H
#define IVEHICLE_H

#include <string>


class IVehicle {
public:
    // Pure virtual functions — must be overridden by every concrete vehicle
    virtual void displayDetails() const = 0;
    virtual double calculateDepreciation() const = 0;
    virtual std::string getType() const = 0;
    virtual double getPrice() const = 0;
    virtual std::string getBrand() const = 0;
    virtual std::string getModel() const = 0;
    virtual int getYear() const = 0;

    // Virtual destructor 
    virtual ~IVehicle();
};

#endif
