#pragma once

#include <iostream>

using namespace std;

// Strategy Interface
class PricingStrategy
{
public:
    virtual ~PricingStrategy() = default;
    virtual double calculateFare(double distanceKm, double baseRatePerKm) const = 0;
};

// Standard Pricing Implementation
class StandardPricing : public PricingStrategy
{
public:
    double calculateFare(double distanceKm, double baseRatePerKm) const override
    {
        return distanceKm * baseRatePerKm;
    }
};

// Surge Pricing Implementation
class SurgePricing : public PricingStrategy
{
private:
    double surgeMultiplier;

public:
    SurgePricing(double multiplier) : surgeMultiplier(multiplier) {}

    double calculateFare(double distanceKm, double baseRatePerKm) const override
    {
        return distanceKm * baseRatePerKm * surgeMultiplier;
    }
};
