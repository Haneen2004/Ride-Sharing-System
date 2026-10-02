#pragma once

#include <vector>
#include <memory>
#include <cmath>
#include "Models.h"

using namespace std;

class MatchingEngine
{
private:
    static double calculateDistance(pair<double, double> loc1, pair<double, double> loc2)
    {
        double dx = loc1.first - loc2.first;
        double dy = loc1.second - loc2.second;
        return sqrt(dx * dx + dy * dy);
    }

public:
    static shared_ptr<Driver> findNearestDriver(
        pair<double, double> riderLocation,
        VehicleType requestedType,
        const vector<shared_ptr<Driver>> &drivers)
    {
        shared_ptr<Driver> nearestDriver = nullptr;
        double minDistance = 1e9; // infinity

        for (const auto &driver : drivers)
        {
            if (driver->getAvailability() && driver->getVehicle()->getType() == requestedType)
            {
                double dist = calculateDistance(riderLocation, driver->getLocation());
                if (dist < minDistance)
                {
                    minDistance = dist;
                    nearestDriver = driver;
                }
            }
        }
        return nearestDriver;
    }
};
