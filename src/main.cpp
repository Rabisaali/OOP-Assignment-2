#include <iostream>
#include <iomanip>
#include <string>
#include "../include/interfaces/IVehicle.h"
#include "../include/interfaces/IUser.h"
#include "../include/interfaces/IListable.h"
#include "../include/interfaces/IPayable.h"
using namespace std;

/*
 * 1)  Vehicle -> IVehicle     : common vehicle interface for polymorphic listings
 * 2)  Car -> Vehicle          : reuse shared vehicle state/behaviour
 * 3)  ElectricCar -> Car      : specialize car for EV-specific details
 * 4)  Bike -> Vehicle         : bike-specific behavior with shared base fields
 * 5)  User -> IUser           : role-independent user abstraction
 * 6)  Buyer -> User           : adds favorites and buyer actions
 * 7)  TrustedBuyer -> Buyer   : extends buyer with verification badge
 * 8)  Seller -> User          : adds inventory/review/rating behavior
 * 9)  PremiumSeller -> Seller : adds premium seller capabilities
 * 10) Admin -> User           : moderation and approval responsibilities
 * 11) Payment -> IPayable     : standard payment contract implementation
 * 12) Listing -> IListable    : standard listing lifecycle operations
 */

//  Forward declarations (for friend declarations)

class Car;
class ElectricCar;
class Bike;
class Buyer;
class TrustedBuyer;
class Seller;
class PremiumSeller;
class Admin;
class Payment;
class Review;
class Listing;

//  REVIEW CLASS
class Review {
private:
    const int reviewID;
    const int reviewerID, sellerID;
    int rating;
    string comment, date;

public:
    Review(int id=0, int r=0, int s=0, int rate=0, string c="NA", string d="NA") : reviewID(id), reviewerID(r), sellerID(s), rating(rate), comment(c), date(d) {}

    void addReview() { 
        cout << "Review added: " << rating << "/5 - " << comment << "\n"; 
    }
    void deleteReview() { 
        cout << "Review deleted.\n"; 
    }
    void editReview(string newC, int newR) { 
        comment = newC; rating = newR; 
    }
    void displayReview() const { 
        cout << "Review: " << rating << "/5 - " << comment << " (" << date << ")\n";
    }
    int getRating() const { 
        return rating; 
    }

    //Operator Overloading 1: == to compare two reviews by rating
    // Reasoning: allows easy equality check between reviews
    bool operator==(const Review& other) const {
        return rating == other.rating;
    }

    //Operator Overloading 2: << to print review
    // Reasoning: cleaner output without calling a function
    friend ostream& operator<<(ostream& os, const Review& rev);
};
ostream& operator<<(ostream& os, const Review& rev) {
    os << "[Review " << rev.reviewID << "] " << rev.rating << "/5 - " << rev.comment;
    return os;
}

// BASE CLASS: Vehicle (inherits IVehicle)
// Inheritance #1: Vehicle -> IVehicle (abstract)
// Provides shared attributes for Car and Bike
class Vehicle : public IVehicle {
protected:
    string brand, model;
    int year;
    double price;
    bool isSold;

public:
    Vehicle() : brand("Unknown"), model("Unknown"), year(0), price(0.0), isSold(false) {}

    Vehicle(string b, string m, int y, double p) : brand(b), model(m), year(y), price(p), isSold(false) {}

    Vehicle(const Vehicle& v) : brand(v.brand), model(v.model), year(v.year), price(v.price), isSold(v.isSold) {}

    Vehicle& operator=(const Vehicle& v) {
        if (this != &v) {
            brand = v.brand;
            model = v.model;
            year = v.year;
            price = v.price;
            isSold = v.isSold;
        }
        return *this;
    }

    string getBrand() const override { 
        return brand; 
    }
    string getModel() const override { 
        return model; 
    }
    int getYear()  const override { 
        return year;  
    }
    double getPrice() const override { 
        return price; 
    }

    void markAsSold() { 
        isSold = true; 
    }

    // Operator Overloading 3: < to compare vehicle price 
    // Reasoning: enables sorting/searching by price
    bool operator<(const Vehicle& other) const { 
        return price < other.price; 
    }

    // Operator Overloading 4: > to compare vehicle price 
    bool operator>(const Vehicle& other) const { 
        return price > other.price; 
    }

    // Operator Overloading 5: == to compare brand+model 
    // Reasoning: check if two vehicles are the "same" model
    bool operator==(const Vehicle& other) const {
        return (brand == other.brand && model == other.model && year == other.year);
    }

    virtual ~Vehicle() {}
};

// DERIVED CLASS: Car (Inheritance #2: Car -> Vehicle) 
class Car : public Vehicle {
private:
    float mileage;
    int numDoors;
    string fuelType;

public:
    Car() : Vehicle(), mileage(0), numDoors(4), fuelType("Petrol") {}

    Car(string b, string m, int y, double p, float mil, int doors = 4, string fuel = "Petrol") : Vehicle(b, m, y, p), mileage(mil), numDoors(doors), fuelType(fuel) {}

    Car(const Car& c) : Vehicle(c), mileage(c.mileage), numDoors(c.numDoors), fuelType(c.fuelType) {}

    Car& operator=(const Car& c) {
        if (this != &c) {
            Vehicle::operator=(c);
            mileage = c.mileage;
            numDoors = c.numDoors;
            fuelType = c.fuelType;
        }
        return *this;
    }

    // Polymorphism: override displayDetails()
    void displayDetails() const override {
        cout << "[CAR]  " << brand << " " << model << " (" << year << ")  Rs." << price << "  Mileage: " << mileage << " km" << "  Doors: " << numDoors << "  Fuel: " << fuelType << "\n";
    }

    // Function overloading — displayDetails with extra context flag
    void displayDetails(bool showFuel) const {
        displayDetails();
        if (showFuel) cout << "       -> Fuel type highlighted: " << fuelType << "\n";
    }

    double calculateDepreciation() const override {
        // Cars depreciate ~15% per year from base price
        return price * 0.85;
    }

    string getType() const override { 
        return "Car"; 
    }
    float  getMileage() const { 
        return mileage; 
    }

    void updateDetails(string b, string m, int y, double p, float mil) {
        brand = b; model = m; year = y; price = p; mileage = mil;
    }

    // Operator Overloading 6: + to combine two cars into a descriptive string
    // Reasoning: quickly produce a combined listing description
    string operator+(const Car& other) const {
        return brand + " " + model + " & " + other.brand + " " + other.model;
    }

    // Operator Overloading 7: >> to read car from stream
    // Reasoning: convenient cin-style input for Car objects
    friend istream& operator>>(istream& is, Car& c);

    // Operator Overloading 8: << to print car 
    friend ostream& operator<<(ostream& os, const Car& c);
};
istream& operator>>(istream& is, Car& c) {
    cout << "  Brand: ";    is >> c.brand;
    cout << "  Model: ";    is >> c.model;
    cout << "  Year: ";     is >> c.year;
    cout << "  Price: ";    is >> c.price;
    cout << "  Mileage: ";  is >> c.mileage;
    return is;
}
ostream& operator<<(ostream& os, const Car& c) {
    os << "[CAR] " << c.brand << " " << c.model << " (" << c.year << ") Rs." << c.price;
    return os;
}

// DERIVED CLASS: ElectricCar (Inheritance #3: ElectricCar -> Car)
class ElectricCar : public Car {
private:
    int batteryRangeKm;

public:
    ElectricCar(string b = "Unknown", string m = "Unknown", int y = 0, double p = 0, float mil = 0, int range = 250) : Car(b, m, y, p, mil, 4, "Electric"), batteryRangeKm(range) {}

    void displayDetails() const override {
        cout << "[EV]   " << getBrand() << " " << getModel() << " (" << getYear() << ")  Rs." << getPrice() << "  Range: " << batteryRangeKm << " km\n";
    }

    double calculateDepreciation() const override {
        return getPrice() * 0.88;
    }

    string getType() const override { return "ElectricCar"; }
};

// DERIVED CLASS: Bike (Inheritance #3: Bike -> Vehicle)
class Bike : public Vehicle {
private:
    string bikeType;   // Road / Off-road / Sport
    int engineCC;

public:
    Bike() : Vehicle(), bikeType("Road"), engineCC(125) {}

    Bike(string b, string m, int y, double p, string bt = "Road", int cc = 125) : Vehicle(b, m, y, p), bikeType(bt), engineCC(cc) {}

    // Polymorphism: override displayDetails() 
    void displayDetails() const override {
        cout << "[BIKE] " << brand << " " << model << " (" << year << ")  Rs." << price << "  Type: " << bikeType << "  Engine: " << engineCC << "cc\n";
    }

    double calculateDepreciation() const override {
        // Bikes depreciate ~20% per year
        return price * 0.80;
    }

    string getType() const override { 
        return "Bike"; 
    }

    // Operator Overloading 9: == for bikes
    bool operator==(const Bike& other) const {
        return (brand == other.brand && engineCC == other.engineCC);
    }

    friend ostream& operator<<(ostream& os, const Bike& bk);
};
ostream& operator<<(ostream& os, const Bike& bk) {
    os << "[BIKE] " << bk.brand << " " << bk.model << " (" << bk.year << ") Rs." << bk.price << " " << bk.engineCC << "cc";
    return os;
}

// BASE CLASS: User (Inheritance #4: User -> IUser)
class User : public IUser {
protected:
    string password;
    bool isLoggedIn;

public:
    User(int id = 0, string n = "NA", string c = "NA", string e = "NA", string pwd = "1234") : IUser(id, n, c, e), password(pwd), isLoggedIn(false) {}

    void login() const override {
        cout << getRole() << " [" << name << "] logged in.\n";
    }

    void logout() { isLoggedIn = false; }

    // Operator Overloading 10: == to compare users by ID 
    // Reasoning: used when checking if a user is already registered
    bool operator==(const User& other) const {
        return userID == other.userID;
    }

    virtual ~User() {}
};

// DERIVED CLASS: Buyer (Inheritance #5: Buyer -> User) 
class Buyer : public User {
private:
    Car favorites[10];
    int favCount;

public:
    Buyer(int id = 0, string n = "NA", string c = "NA", string e = "NA") : User(id, n, c, e), favCount(0) {}

    void displayInfo() const override {
        cout << "Buyer | ID: " << userID << " | Name: " << name << " | Contact: " << contact << "\n";
    }

    string getRole() const override { 
        return "Buyer"; 
    }

    void addFavorite(Car c) {
        if (favCount < 10) favorites[favCount++] = c;
    }

    void removeFavorite(int index) {
        if (index >= 0 && index < favCount) {
            for (int i = index; i < favCount - 1; i++)
                favorites[i] = favorites[i + 1];
            favCount--;
        }
    }

    void displayFavorites() const {
        cout << "=== " << name << "'s Favourites ===\n";
        if (favCount == 0) { cout << "No favourites saved.\n"; return; }
        for (int i = 0; i < favCount; i++) favorites[i].displayDetails();
    }

    void sendMessage(string msg) {
        cout << "Buyer [" << name << "] says: " << msg << "\n";
    }

    // Friend function declared here so it can access favCount
    friend void compareBuyers(const Buyer& a, const Buyer& b);
};

// DERIVED CLASS: Seller (Inheritance #6: Seller -> User)
class Seller : public User {
private:
    double sellerRating;
    Car cars[10];
    int carCount;
    Review sellerReview;

public:
    Seller(int id = 0, string n = "NA", string c = "NA", string e = "NA", double r = 0.0, Review rev = Review()) : User(id, n, c, e), sellerRating(r), carCount(0), sellerReview(rev) {}

    void displayInfo() const override {
        cout << "Seller | ID: " << userID << " | Name: " << name << " | Rating: " << sellerRating << "\n";
    }

    string getRole() const override { 
        return "Seller"; 
    }

    void addCar(Car c) {
        if (carCount < 10) cars[carCount++] = c;
    }

    void removeCar(int index) {
        if (index >= 0 && index < carCount) {
            for (int i = index; i < carCount - 1; i++)
                cars[i] = cars[i + 1];
            carCount--;
        }
    }

    void showCars() const {
        cout << "=== " << name << "'s Listings ===\n";
        for (int i = 0; i < carCount; i++) cars[i].displayDetails();
    }

    void showReview() const { 
        sellerReview.displayReview(); 
    }
    double getRating() const { 
        return sellerRating; 
    }

    // Friend class: Admin needs to access rating for ban decisions
    friend class Admin;

    // Friend function: compare two sellers' ratings
    friend void compareSellerRatings(const Seller& a, const Seller& b);
};

// DERIVED CLASS: TrustedBuyer (Inheritance #7: TrustedBuyer -> Buyer)
class TrustedBuyer : public Buyer {
private:
    string trustBadge;

public:
    TrustedBuyer(int id = 0, string n = "NA", string c = "NA", string e = "NA", string badge = "Verified") : Buyer(id, n, c, e), trustBadge(badge) {}

    void displayInfo() const override {
        cout << "Trusted Buyer | ID: " << userID << " | Name: " << name << " | Badge: " << trustBadge << "\n";
    }

    string getRole() const override { 
        return "TrustedBuyer"; 
    }
};

//  DERIVED CLASS: PremiumSeller (Inheritance #9: PremiumSeller -> Seller) 
class PremiumSeller : public Seller {
private:
    string badgeLevel;

public:
    PremiumSeller(int id = 0, string n = "NA", string c = "NA", string e = "NA", double r = 0.0, Review rev = Review(), string badge = "Gold") : Seller(id, n, c, e, r, rev), badgeLevel(badge) {}

    void displayInfo() const override {
        cout << "Premium Seller | ID: " << userID << " | Name: " << name << " | Rating: " << getRating() << " | Badge: " << badgeLevel << "\n";
    }

    string getRole() const override { 
        return "PremiumSeller"; 
    }

    void featureListing() const {
        cout << "Premium feature enabled for seller [" << name << "] with " << badgeLevel << " badge.\n";
    }
};

// DERIVED CLASS: Admin (Inheritance #10: Admin -> User) 
class Admin : public User {
private:
    string role;
    static int totalAdmins;

public:
    Admin(int id = 0, string n = "NA",  string e = "NA", string r = "Moderator") : User(id, n, "N/A", e), role(r) {
        totalAdmins++;
    }

    void displayInfo() const override {
        cout << "Admin | ID: " << userID << " | Name: " << name  << " | Role: " << role << "\n";
    }

    string getRole() const override { 
        return "Admin"; 
    }

    void approveListing(IVehicle& v) {
        cout << "Admin [" << name << "] approved: ";
        v.displayDetails();
    }

    // Admin uses Seller friendship to read private rating
    void banSeller(Seller& s) {
        cout << "Admin [" << name << "] banned seller [" << s.name << "] with rating " << s.sellerRating << "\n";
    }

    static void displayTotalAdmins() {
        cout << "Total Admins: " << totalAdmins << "\n";
    }
};
int Admin::totalAdmins = 0;

//  FRIEND FUNCTIONS (outside any class)

// Friend Function #1 — accesses Buyer's private favCount
// Reasoning: comparison logic doesn't belong to either Buyer object
void compareBuyers(const Buyer& a, const Buyer& b) {
    cout << "Comparing buyers: " << a.name << " (" << a.favCount << " favs) vs " << b.name << " (" << b.favCount << " favs)\n";
    if (a.favCount > b.favCount)
        cout << a.name << " is more active.\n";
    else
        cout << b.name << " is more active.\n";
}

// Friend Function #2 — accesses Seller's private sellerRating
// Reasoning: rating comparison is a utility that shouldn't expose the rating getter
void compareSellerRatings(const Seller& a, const Seller& b) {
    cout << "Comparing sellers: " << a.name << " (" << a.sellerRating << ") vs " << b.name << " (" << b.sellerRating << ")\n";
    if (a.sellerRating > b.sellerRating)
        cout << a.name << " has the better rating.\n";
    else
        cout << b.name << " has the better rating.\n";
}

// Friend Function #3 — global function that compares any two vehicles by price
// Reasoning: IVehicle provides getPrice(); function is intentionally global so it works for Car OR Bike without extra coupling
void comparePrices(const IVehicle& v1, const IVehicle& v2) {
    cout << "Price comparison: Rs." << v1.getPrice() << " vs Rs." << v2.getPrice() << " -> ";
    if (v1.getPrice() < v2.getPrice())
        cout << v1.getBrand() << " " << v1.getModel() << " is cheaper.\n";
    else
        cout << v2.getBrand() << " " << v2.getModel() << " is cheaper.\n";
}

//  PAYMENT (Inheritance #11: Payment -> IPayable)

class Payment : public IPayable {
private:
    int paymentID;
    double amount;
    int buyerID, sellerID;
    string method;
    bool paid;
    const double TAX_RATE = 0.15;

public:
    Payment(int id = 0, double a = 0, int b = 0, int s = 0, string m = "Cash") : paymentID(id), amount(a), buyerID(b), sellerID(s), method(m), paid(false) {}

    void processPayment() override {
        paid = true;
        cout << "Payment processed: PKR " << amount * (1 + TAX_RATE) << " via " << method << "\n";
    }

    void refundPayment() override {
        paid = false;
        cout << "Payment refunded.\n";
    }

    void generateInvoice() override {
        cout << "Invoice #" << paymentID << "  Amount: PKR " << amount << "  Tax: PKR " << amount * TAX_RATE << "\n";
    }

    bool isPaid() const { return paid; }

    // Operator Overloading (global) : + to add two payments
    // Reasoning: combine payment totals without exposing amount setter
    friend Payment operator+(const Payment& a, const Payment& b) {
        return Payment(0, a.amount + b.amount, a.buyerID, b.sellerID, "Combined");
    }

    friend ostream& operator<<(ostream& os, const Payment& p) ;
};
ostream& operator<<(ostream& os, const Payment& p) {
        os << "Payment #" << p.paymentID << " | Rs." << p.amount << " | " << (p.paid ? "PAID" : "PENDING");
        return os;
    }

//  LISTING (Inheritance #12: Listing -> IListable)

class Listing : public IListable {
private:
    int listingID;
    IVehicle* vehicle;   // pointer to base — supports Car OR Bike
    string datePosted;
    bool active;
    int views;

public:
    Listing(int id, IVehicle* v, string d) : listingID(id), vehicle(v), datePosted(d), active(true), views(0) {}

    void activateListing() override { 
        active = true; 
    }
    void deactivateListing() override { 
        active = false; 
    }
    bool isActive() const override { 
        return active; 
    }

    void showListing() const override {
        cout << "Listing #" << listingID << " [" << (active ? "ACTIVE" : "INACTIVE") << "]" << " Posted: " << datePosted << " Views: " << views << "\n";
        if (vehicle) vehicle->displayDetails();  // Polymorphism
    }

    void incrementViews() { views++; }
};

//  MARKETPLACE 

class Marketplace {
private:
    string systemName;
    IVehicle* allListings[50];
    int listingCount;
    Seller* sellers[20];
    int sellerCount;
    static int totalListings;
    static int totalUsers;

public:
    Marketplace(string name = "Car Marketplace") : systemName(name), listingCount(0), sellerCount(0) {}

    void addListing(IVehicle* v) {
        if (listingCount < 50) {
            allListings[listingCount++] = v;
            totalListings++;
        }
    }

    void deleteListing(int index) {
        if (index >= 0 && index < listingCount) {
            for (int i = index; i < listingCount - 1; i++)
                allListings[i] = allListings[i + 1];
            listingCount--;
            totalListings--;
        }
    }

    // Polymorphic display — each vehicle type calls its own displayDetails()
    void showAllListings() const {
        cout << "=== All Listings in " << systemName << " ===\n";
        for (int i = 0; i < listingCount; i++)
            allListings[i]->displayDetails();
    }

    void searchByBrand(string brand) const {
        cout << "=== Search Results: " << brand << " ===\n";
        for (int i = 0; i < listingCount; i++)
            if (allListings[i]->getBrand() == brand)
                allListings[i]->displayDetails();
    }

    void registerSeller(Seller* s) {
        if (sellerCount < 20) sellers[sellerCount++] = s;
    }

    IVehicle** getListings() {
        return allListings; 
    }
    int getListingCount() const {
        return listingCount; 
    }

    static int getTotalListings() { 
        return totalListings; 
    }
};
int Marketplace::totalListings = 0;
int Marketplace::totalUsers    = 0;

//  MESSAGE CLASS 

class Message {
private:
    const int messageID;
    const int senderID, receiverID;
    string content, timestamp;
    bool status;

public:
    Message(int id=0, int s=0, int r=0, string c="NA", string t="NA") : messageID(id), senderID(s), receiverID(r), content(c), timestamp(t), status(false) {}

    void sendMessage() { 
        status = true;  
        cout << "Message sent: " << content << "\n"; 
    }
    void markAsRead() { 
        cout << "Message marked as read.\n"; 
    }
    void deleteMessage() { 
        status = false; cout << "Message deleted.\n"; 
    }
    void replyMessage(string r) { 
        cout << "Reply: " << r << "\n"; 
    }
};

//  SEARCHFILTER CLASS 

class SearchFilter {
private:
    string brand, model;
    double minPrice, maxPrice;
    int minYear;

public:
    SearchFilter(string b="Any", string m="Any", double minP=0, double maxP=999999, int y=0) : brand(b), model(m), minPrice(minP), maxPrice(maxP), minYear(y) {}

    bool matchVehicle(const IVehicle* v) const {
        return (brand == "Any" || v->getBrand() == brand) && (v->getPrice() >= minPrice && v->getPrice() <= maxPrice) && (v->getYear() >= minYear);
    }

    void applyFilter(IVehicle** vehicles, int size) const {
        cout << "=== Filter Results ===\n";
        for (int i = 0; i < size; i++)
            if (matchVehicle(vehicles[i]))
                vehicles[i]->displayDetails();
    }

    void resetFilter() {
        brand="Any"; model="Any";
        minPrice=0; maxPrice=999999; minYear=0;
    }
};

int main() {
    // Print currency-like numeric values without scientific notation.
    cout << fixed << setprecision(0);

    cout << "========================================\n";
    cout << "   Car & Bike Marketplace - Assignment 2\n";
    cout << "========================================\n\n";

    // Create Marketplace
    Marketplace market("PakWheels Extended");

    // Create Users (Inheritance: User->Buyer/Seller/Admin)
    Buyer  buyer1(101, "Rabisa", "0333-9876543", "rabisa@email.com");
    Buyer  buyer2(102, "Ahmed",  "0321-1112222", "ahmed@email.com");
    TrustedBuyer trustedBuyer(103, "Areeba", "0301-4445556", "areeba@email.com", "Platinum Verified");
    Seller seller1(1,  "Ali",    "0300-1234567", "ali@email.com", 4.8, Review(1, 101, 1, 5, "Great seller!", "2024-01-10"));
    Seller seller2(2,  "Usman",  "0311-5556666", "usman@email.com", 3.5);
    PremiumSeller premiumSeller(3, "Sara", "0322-7778888", "sara@email.com", 4.9, Review(3, 102, 3, 5, "Trusted and responsive", "2025-11-02"), "Platinum");
    Admin  admin1(999, "System Admin", "admin@market.com", "SuperAdmin");

    ElectricCar ev1("Tesla", "Model3", 2024, 12000000, 5000, 420);

    IUser* users[] = { &buyer1, &trustedBuyer, &seller1, &premiumSeller, &admin1 };
    cout << " User Logins (Polymorphism via IUser*) \n";
    for (int i = 0; i < 5; i++) users[i]->login();
    cout << "\n";

    cout << " Additional Inheritance Demo: PremiumSeller -> Seller \n";
    premiumSeller.displayInfo();
    premiumSeller.featureListing();
    cout << "\n";

    // Create Vehicles (Car and Bike)
    Car  c1("Toyota", "Corolla", 2020, 2500000, 30000);
    Car  c2("Honda",  "Civic",   2019, 2200000, 40000);
    Car  c3("Suzuki", "Swift",   2021, 2000000, 15000);
    Bike b1("Yamaha", "YBR125",  2022,  175000, "Road",    125);
    Bike b2("Honda",  "CBR150",  2023,  450000, "Sport",   150);

    // Register seller and add cars
    market.registerSeller(&seller1);
    seller1.addCar(c1);
    seller1.addCar(c2);
    seller2.addCar(c3);

    // Add listings
    cout << " Adding Listings \n";
    market.addListing(&c1);
    market.addListing(&c2);
    market.addListing(&c3);
    market.addListing(&ev1);
    market.addListing(&b1);
    market.addListing(&b2);

    cout << "\n All Listings (Polymorphic displayDetails) \n";
    market.showAllListings();

    cout << "\n Car displayDetails() overload with fuel flag \n";
    c1.displayDetails(true);

    cout << "\n Depreciation via Abstract IVehicle* \n";
    IVehicle* vlist[] = { &c1, &b1 };
    for (int i = 0; i < 2; i++)
        cout << vlist[i]->getBrand() << " " << vlist[i]->getModel() << " depreciated value: Rs." << vlist[i]->calculateDepreciation() << "\n";

    cout << "\n Operator Overloading \n";

    Car c1_copy("Toyota","Corolla",2020,2500000,30000);
    cout << "c1 == c1_copy? " << (c1 == c1_copy ? "Yes" : "No") << "\n";

    cout << "c1 < c2? " << (c1 < c2 ? "Yes" : "No") << "\n";
    cout << "c1 > c2? " << (c1 > c2 ? "Yes" : "No") << "\n";

    string combined = c1 + c2;
    cout << "Combined listing: " << combined << "\n";

    cout << c1 << "\n";
    cout << b2 << "\n";

    Payment p1(1, 2500000, 101, 1, "Bank Transfer");
    Payment p2(2, 175000,  101, 2, "Cash");
    Payment ptotal = p1 + p2;
    cout << "Total payment: ";
    ptotal.generateInvoice();

    cout << p1 << "\n";

    Review r1(1,101,1,5,"Excellent","2024-01-10");
    Review r2(2,102,2,5,"Great","2024-02-01");
    cout << "r1 == r2 (same rating)? " << (r1 == r2 ? "Yes" : "No") << "\n";

    cout << r1 << "\n";

    // Listing (IListable)
    cout << "\n Listings with IListable Interface \n";
    Listing listing1(1001, &c1, "2026-04-16");
    Listing listing2(1002, &b1, "2026-04-16");
    listing1.showListing();
    listing2.showListing();
    listing1.deactivateListing();
    cout << "After deactivate: active=" << listing1.isActive() << "\n";

    cout << "\n Buyer Features \n";
    buyer1.addFavorite(c1);
    buyer1.addFavorite(c3);
    buyer2.addFavorite(c2);
    buyer1.displayFavorites();
    buyer1.sendMessage("Is the Corolla still available?");

    cout << "\n Seller Features \n";
    seller1.showCars();
    seller1.showReview();

    cout << "\n Admin Features \n";
    admin1.approveListing(c1);
    admin1.banSeller(seller2);    
    Admin::displayTotalAdmins();

    cout << "\n Friend Functions \n";
    compareBuyers(buyer1, buyer2);
    compareSellerRatings(seller1, seller2);
    comparePrices(c1, b1);

    // SearchFilter
    cout << "\n Search Filter (Brand: Honda) \n";
    SearchFilter filter("Honda");
    filter.applyFilter(market.getListings(), market.getListingCount());

    cout << "\n Payment Processing \n";
    p1.processPayment();
    p1.generateInvoice();
    p1.refundPayment();

    cout << "\nTotal Listings (Static): " << Marketplace::getTotalListings() << "\n";

    return 0;
}
