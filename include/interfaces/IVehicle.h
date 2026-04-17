#ifndef IVEHICLE_H
#define IVEHICLE_H

#include <string>

// ==================== ABSTRACT CLASS: IVehicle ====================
// Pure interface for all vehicle types (Car, Bike)
// Enforces polymorphic displayDetails() and depreciation behaviour

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

    // Virtual destructor — good practice for base classes
    virtual ~IVehicle();
};

#endif
