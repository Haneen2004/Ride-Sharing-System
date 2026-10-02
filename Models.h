#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
using namespace std;

// 1. Vehicles
enum class VehicleType
{
    Economy,
    Premium,
    Scooter
};

class Vehicle
{
protected:
    string licensePlate, model;
    VehicleType type;

public:
    Vehicle(string plate, string mdl, VehicleType t)
        : licensePlate(plate), model(mdl), type(t) {}

    virtual ~Vehicle() = default;

    // Pure Virtual Function
    virtual double getBaseFarePerKm() const = 0;
    VehicleType getType() const { return type; }
};

class Scooter : public Vehicle
{
public:
    Scooter(string plate, string mdl)
        : Vehicle(plate, mdl, VehicleType::Scooter) {}

    double getBaseFarePerKm() const override { return 15.0; }
};

class EconomyCar : public Vehicle
{
public:
    EconomyCar(string plate, string mdl)
        : Vehicle(plate, mdl, VehicleType::Economy) {}

    double getBaseFarePerKm() const override { return 10.0; } // EGP per km
};

class PremiumCar : public Vehicle
{
public:
    PremiumCar(string plate, string mdl)
        : Vehicle(plate, mdl, VehicleType::Premium) {}

    double getBaseFarePerKm() const override { return 20.0; }
};

// 2. Users

class User
{
protected:
    string id, name, phone;
    double rating;

public:
    User(string userId, string userName, string userPhone)
        : id(userId), name(userName), phone(userPhone), rating(5.0) {}

    virtual ~User() = default;

    string getId() const { return id; }
    string getName() const { return name; }
};

class Rider : public User
{
private:
    double walletBalance;

public:
    Rider(string id, string name, string phone, double initialBalance)
        : User(id, name, phone), walletBalance(initialBalance) {}

    void addFunds(double amount) { walletBalance += amount; }
    bool deductFunds(double amount)
    {
        if (walletBalance >= amount)
        {
            walletBalance -= amount;
            return true;
        }
        return false;
    }
    double getBalance() const { return walletBalance; }
};

class Driver : public User
{
private:
    shared_ptr<Vehicle> vehicle;
    bool isAvailable;
    pair<double, double> currentLocation; // (x, y) coordinates

public:
    Driver(string id, string name, string phone, shared_ptr<Vehicle> v)
        : User(id, name, phone), vehicle(v), isAvailable(true), currentLocation({0.0, 0.0}) {}

    bool getAvailability() const { return isAvailable; }
    void setAvailability(bool status) { isAvailable = status; }

    void updateLocation(double x, double y) { currentLocation = {x, y}; }
    pair<double, double> getLocation() const { return currentLocation; }
    shared_ptr<Vehicle> getVehicle() const { return vehicle; }
};