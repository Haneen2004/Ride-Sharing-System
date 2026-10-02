#pragma once
#include <cmath>

// Strategy Interface
class PricingStrategy
{
public:
    virtual ~PricingStrategy() = default;
    virtual double calculateFare(double distanceKm, double baseFarePerKm) const = 0;
};

// 1. Standard Pricing Strategy
class StandardPricing : public PricingStrategy
{
public:
    double calculateFare(double distanceKm, double baseFarePerKm) const override
    {
        double baseBookingFee = 5.0;
        return baseBookingFee + (distanceKm * baseFarePerKm);
    }
};

// 2. Surge Pricing Strategy
class SurgePricing : public PricingStrategy
{
private:
    double surgeMultiplier;

public:
    SurgePricing(double multiplier) : surgeMultiplier(multiplier) {}

    double calculateFare(double distanceKm, double baseFarePerKm) const override
    {
        double baseBookingFee = 5.0;
        return (baseBookingFee + (distanceKm * baseFarePerKm)) * surgeMultiplier;
    }
};