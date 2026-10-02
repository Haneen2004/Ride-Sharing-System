#pragma once
#include <iostream>
#include <memory>
#include <string>
#include "Models.h"
#include "PricingStrategy.h"

enum class RideStatus
{
    Requested,
    Accepted,
    InProgress,
    Completed,
    Cancelled
};

class Ride
{
private:
    string rideId;
    shared_ptr<Rider> rider;
    shared_ptr<Driver> driver;
    pair<double, double> source;
    pair<double, double> destination;
    double distanceKm;
    double fare;
    RideStatus status;

public:
    Ride(string id, shared_ptr<Rider> r, pair<double, double> src, pair<double, double> dest, double distance)
        : rideId(id), rider(r), driver(nullptr), source(src), destination(dest),
          distanceKm(distance), fare(0.0), status(RideStatus::Requested) {}

    // Assign a driver to the ride and update the status
    void assignDriver(shared_ptr<Driver> d)
    {
        driver = d;
        status = RideStatus::Accepted;
        driver->setAvailability(false); // Mark the driver as unavailable
    }

    // Calculate the fare for the ride based on the selected strategy
    void calculateFare(const PricingStrategy &strategy)
    {
        if (driver && driver->getVehicle())
        {
            double baseRate = driver->getVehicle()->getBaseFarePerKm();
            fare = strategy.calculateFare(distanceKm, baseRate);
        }
    }

    // Start the ride
    void startRide()
    {
        if (status == RideStatus::Accepted)
        {
            status = RideStatus::InProgress;
        }
    }

    // Complete the ride and process payment
    bool completeRide()
    {
        if (status == RideStatus::InProgress)
        {
            if (rider->deductFunds(fare))
            { // Deduct from the Rider's wallet
                status = RideStatus::Completed;
                driver->setAvailability(true); // Mark the driver as available
                return true;
            }
            else
            {
                cout << "Error: Payment failed due to insufficient funds!\n";
                return false;
            }
        }
        return false;
    }

    // Getters
    double getFare() const { return fare; }
    RideStatus getStatus() const { return status; }
    shared_ptr<Driver> getDriver() const { return driver; }
};