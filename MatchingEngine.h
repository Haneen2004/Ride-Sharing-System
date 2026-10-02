#pragma once
#include "Models.h"
#include <cmath>

class MatchingEngine
{
private:
    // Euclidean Distance between two points (x1, y1) and (x2, y2)
    static double calculateDistance(pair<double, double> loc1, pair<double, double> loc2)
    {
        double dx = loc1.first - loc2.first;
        double dy = loc1.second - loc2.second;
        return sqrt(dx * dx + dy * dy);
    }

public:
    // Find the nearest available driver of the preferred vehicle type
    static shared_ptr<Driver> findNearestDriver(
        pair<double, double> riderLocation,
        VehicleType preferredType,
        const vector<shared_ptr<Driver>> &drivers)
    {
        shared_ptr<Driver> bestDriver = nullptr;
        double minDistance = 1e9; // Initialize with a large number

        for (const auto &driver : drivers)
        {
            // Conditions: the driver is available + the type of their vehicle matches the preferred type
            if (driver->getAvailability() && driver->getVehicle()->getType() == preferredType)
            {
                double dist = calculateDistance(riderLocation, driver->getLocation());
                if (dist < minDistance)
                {
                    minDistance = dist;
                    bestDriver = driver;
                }
            }
        }
        return bestDriver; // Returns the nearest available driver or nullptr if none is available
    }
};