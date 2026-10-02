#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <string>
#include "Models.h"
#include "PricingStrategy.h"
#include "MatchingEngine.h"
#include "Ride.h"

using namespace std;

// ==========================================
// File I/O: Data Loader for CSV Parsing
// ==========================================
class DataLoader
{
public:
    static vector<shared_ptr<Driver>> loadDriversFromCSV(const string &filename)
    {
        vector<shared_ptr<Driver>> drivers;
        ifstream file(filename);

        if (!file.is_open())
        {
            cerr << "[ERROR] Could not open file: " << filename << "\n";
            return drivers;
        }

        string line;
        getline(file, line); // Skip CSV Header line

        while (getline(file, line))
        {
            if (line.empty())
                continue;

            stringstream ss(line);
            string id, name, phone, vType, plate, model, xStr, yStr;

            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, phone, ',');
            getline(ss, vType, ',');
            getline(ss, plate, ',');
            getline(ss, model, ',');
            getline(ss, xStr, ',');
            getline(ss, yStr, ',');

            // Polymorphic Vehicle instantiation
            shared_ptr<Vehicle> vehicle = nullptr;
            if (vType == "Economy")
            {
                vehicle = make_shared<EconomyCar>(plate, model);
            }
            else if (vType == "Premium")
            {
                vehicle = make_shared<PremiumCar>(plate, model);
            }
            else if (vType == "Scooter")
            {
                vehicle = make_shared<Scooter>(plate, model);
            }

            if (vehicle != nullptr)
            {
                auto driver = make_shared<Driver>(id, name, phone, vehicle);
                driver->updateLocation(stod(xStr), stod(yStr));
                drivers.push_back(driver);
            }
        }

        file.close();
        return drivers;
    }

    static vector<shared_ptr<Rider>> loadRidersFromCSV(const string &filename)
    {
        vector<shared_ptr<Rider>> riders;
        ifstream file(filename);

        if (!file.is_open())
        {
            cerr << "[ERROR] Could not open file: " << filename << "\n";
            return riders;
        }

        string line;
        getline(file, line); // Skip CSV Header line

        while (getline(file, line))
        {
            if (line.empty())
                continue;

            stringstream ss(line);
            string id, name, phone, balanceStr, xStr, yStr;

            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, phone, ',');
            getline(ss, balanceStr, ',');
            getline(ss, xStr, ',');
            getline(ss, yStr, ',');

            auto rider = make_shared<Rider>(id, name, phone, stod(balanceStr));
            riders.push_back(rider);
        }

        file.close();
        return riders;
    }
};

// ==========================================
// MAIN SIMULATION
// ==========================================
int main()
{
    cout << "===========================\n";
    cout << "  RIDE-SHARING SYSTEM \n";
    cout << "===========================\n\n";

    // 1. Load Data dynamically from CSV Files
    auto systemDrivers = DataLoader::loadDriversFromCSV("drivers.csv");
    auto systemRiders = DataLoader::loadRidersFromCSV("riders.csv");

    if (systemDrivers.empty() || systemRiders.empty())
    {
        cout << "[WARNING] Failed to load initial data. Please check drivers.csv and riders.csv.\n";
        return 1;
    }

    cout << "--> Successfully loaded " << systemDrivers.size() << " drivers and "
         << systemRiders.size() << " riders from CSV files.\n\n";

    // 2. Select First Rider from CSV
    auto currentRider = systemRiders[0];
    pair<double, double> riderLocation = {0.0, 0.0};
    pair<double, double> destinationLocation = {6.0, 8.0};
    double estimatedDistanceKm = 10.0;

    cout << "--> Rider Selected: " << currentRider->getName() << " (ID: " << currentRider->getId() << ")\n";
    cout << "--> Initial Wallet Balance: " << currentRider->getBalance() << " EGP\n";
    cout << "--> Requesting Category: Economy | Estimated Distance: " << estimatedDistanceKm << " km\n\n";

    // 3. Match Nearest Driver
    auto matchedDriver = MatchingEngine::findNearestDriver(riderLocation, VehicleType::Economy, systemDrivers);

    if (matchedDriver != nullptr)
    {
        cout << "[MATCH SUCCESS] Nearest Driver Found:\n";
        cout << "    - Name: " << matchedDriver->getName() << " (ID: " << matchedDriver->getId() << ")\n";
        cout << "    - Vehicle: " << (matchedDriver->getVehicle()->getType() == VehicleType::Economy ? "Economy" : "Other") << "\n";
        cout << "    - Location: (" << matchedDriver->getLocation().first << ", " << matchedDriver->getLocation().second << ")\n\n";

        // 4. Create Ride Transaction
        Ride currentRide("RIDE-9901", currentRider, riderLocation, destinationLocation, estimatedDistanceKm);
        currentRide.assignDriver(matchedDriver);

        // 5. Apply Surge Pricing Strategy
        SurgePricing surgePricing(1.25);
        currentRide.calculateFare(surgePricing);

        cout << "--> Total Calculated Fare (Surge 1.25x): " << currentRide.getFare() << " EGP\n";

        // 6. Execute Ride Lifecycle
        currentRide.startRide();
        cout << "--> Ride Status: IN PROGRESS...\n";

        if (currentRide.completeRide())
        {
            cout << "\n[RIDE COMPLETED SUCCESSFULLY]\n";
            cout << "--> Remaining Balance for " << currentRider->getName() << ": " << currentRider->getBalance() << " EGP\n";
            cout << "--> Driver Availability Status: " << (matchedDriver->getAvailability() ? "Available" : "Busy") << "\n";
        }
    }
    else
    {
        cout << "[MATCH FAILED] No available drivers found for the requested category.\n";
    }

    cout << "\n=================================================\n";
    return 0;
}