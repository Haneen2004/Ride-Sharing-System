#pragma once

#include <iostream>
#include <string>
#include <utility>
#include <memory>

using namespace std;

enum class VehicleType
{
    Economy,
    Premium,
    Scooter
};

// ==========================================
// Base Abstract Class: Vehicle
// ==========================================
class Vehicle
{
protected:
    string licensePlate;
    string model;

public:
    Vehicle(string plate, string mdl) : licensePlate(plate), model(mdl) {}
    virtual ~Vehicle() = default;

    virtual VehicleType getType() const = 0;
    virtual double getBaseFarePerKm() const = 0;

    string getLicensePlate() const { return licensePlate; }
    string getModel() const { return model; }
};

// Concrete Vehicles
class EconomyCar : public Vehicle
{
public:
    EconomyCar(string plate, string mdl) : Vehicle(plate, mdl) {}
    VehicleType getType() const override { return VehicleType::Economy; }
    double getBaseFarePerKm() const override { return 10.0; } // 10 EGP/km
};

class PremiumCar : public Vehicle
{
public:
    PremiumCar(string plate, string mdl) : Vehicle(plate, mdl) {}
    VehicleType getType() const override { return VehicleType::Premium; }
    double getBaseFarePerKm() const override { return 20.0; } // 20 EGP/km
};

class Scooter : public Vehicle
{
public:
    Scooter(string plate, string mdl) : Vehicle(plate, mdl) {}
    VehicleType getType() const override { return VehicleType::Scooter; }
    double getBaseFarePerKm() const override { return 5.0; } // 5 EGP/km
};

// ==========================================
// Base Abstract Class: User
// ==========================================
class User
{
protected:
    string id;
    string name;
    string phone;

public:
    User(string uId, string uName, string uPhone)
        : id(uId), name(uName), phone(uPhone) {}
    virtual ~User() = default;

    string getId() const { return id; }
    string getName() const { return name; }
    string getPhone() const { return phone; }
};

// Derived Class: Rider
class Rider : public User
{
private:
    double walletBalance;
    pair<double, double> currentLocation;

public:
    Rider(string id, string name, string phone, double initialBalance, pair<double, double> loc = {0.0, 0.0})
        : User(id, name, phone), walletBalance(initialBalance), currentLocation(loc) {}

    double getBalance() const { return walletBalance; }

    bool deductFunds(double amount)
    {
        if (walletBalance >= amount)
        {
            walletBalance -= amount;
            return true;
        }
        return false;
    }

    void addFunds(double amount)
    {
        walletBalance += amount;
    }

    void setLocation(double x, double y) { currentLocation = {x, y}; }
    pair<double, double> getLocation() const { return currentLocation; }
};

// Derived Class: Driver
class Driver : public User
{
private:
    bool isAvailable;
    pair<double, double> currentLocation;
    shared_ptr<Vehicle> vehicle;

public:
    Driver(string id, string name, string phone, shared_ptr<Vehicle> v)
        : User(id, name, phone), vehicle(v), isAvailable(true), currentLocation({0.0, 0.0}) {}

    void updateLocation(double x, double y)
    {
        currentLocation = {x, y};
    }

    pair<double, double> getLocation() const { return currentLocation; }
    bool getAvailability() const { return isAvailable; }
    void setAvailability(bool status) { isAvailable = status; }
    shared_ptr<Vehicle> getVehicle() const { return vehicle; }
};
