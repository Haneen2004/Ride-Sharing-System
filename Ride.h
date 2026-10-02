#pragma once

#include <string>
#include <memory>
#include <iostream>
#include "Models.h"
#include "PricingStrategy.h"

using namespace std;

enum class RideStatus
{
    Requested,
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
    pair<double, double> pickupLocation;
    pair<double, double> destinationLocation;
    double distanceKm;
    double fare;
    RideStatus status;

public:
    Ride(string id, shared_ptr<Rider> r, pair<double, double> pickup, pair<double, double> dest, double dist)
        : rideId(id), rider(r), pickupLocation(pickup), destinationLocation(dest),
          distanceKm(dist), driver(nullptr), fare(0.0), status(RideStatus::Requested) {}

    void assignDriver(shared_ptr<Driver> d)
    {
        driver = d;
        if (driver)
        {
            driver->setAvailability(false); // Driver becomes busy
        }
    }

    void calculateFare(const PricingStrategy &strategy)
    {
        if (driver && driver->getVehicle())
        {
            double baseRate = driver->getVehicle()->getBaseFarePerKm();
            fare = strategy.calculateFare(distanceKm, baseRate);
        }
    }

    double getFare() const { return fare; }

    void startRide()
    {
        if (driver != nullptr)
        {
            status = RideStatus::InProgress;
        }
    }

    bool completeRide()
    {
        if (status == RideStatus::InProgress && rider != nullptr && driver != nullptr)
        {
            if (rider->deductFunds(fare))
            {
                status = RideStatus::Completed;
                driver->setAvailability(true); // Driver is available again
                driver->updateLocation(destinationLocation.first, destinationLocation.second);
                return true;
            }
            else
            {
                status = RideStatus::Cancelled;
                driver->setAvailability(true);
                return false;
            }
        }
        return false;
    }
};
