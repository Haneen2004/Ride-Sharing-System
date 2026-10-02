# Ride-Sharing System Simulation (C++ OOP)

A modular, Object-Oriented Ride-Sharing System Simulation built in **C++17**. The project models real-world ride-hailing services (like Uber/Careem) applying core software engineering design patterns, object-oriented principles, and dynamic data loading via file I/O (`CSV`).

---

## Key Features

- **Object-Oriented Architecture:** Pure implementation of Inheritance, Polymorphism, Encapsulation, and Abstraction.
- **Factory & Strategy Patterns:** Dynamic pricing calculation (Standard vs. Surge) and polymorphic vehicle instantiation.
- **Data Persistence (CSV Parsing):** Separates system state/configuration from logic by loading `Rider` and `Driver` records directly from external `.csv` files.
- **Nearest Driver Matching:** Spatial location-based matching algorithm filtering available drivers by vehicle category.
- **Wallet & Transaction Management:** Safe ride execution with automatic wallet deduction and driver availability resets.

---

## Class Diagram (UML)

```mermaid
classDiagram
    class Vehicle {
        <<abstract>>
        #string licensePlate
        #string model
        #VehicleType type
        +getBaseFarePerKm()* double
        +getType() VehicleType
    }

    class EconomyCar {
        +getBaseFarePerKm() double
    }
    class PremiumCar {
        +getBaseFarePerKm() double
    }
    class Scooter {
        +getBaseFarePerKm() double
    }

    Vehicle <|-- EconomyCar
    Vehicle <|-- PremiumCar
    Vehicle <|-- Scooter

    class User {
        <<abstract>>
        #string id
        #string name
        #string phone
        +getId() string
        +getName() string
    }

    class Rider {
        -double walletBalance
        +deductFunds(amount) bool
        +addFunds(amount)
        +getBalance() double
    }

    class Driver {
        -bool isAvailable
        -pair~double,double~ currentLocation
        -shared_ptr~Vehicle~ vehicle
        +updateLocation(x, y)
        +getAvailability() bool
        +setAvailability(status)
    }

    User <|-- Rider
    User <|-- Driver
    Driver "1" o-- "1" Vehicle : owns

    class PricingStrategy {
        <<interface>>
        +calculateFare(distanceKm, baseRate)* double
    }

    class StandardPricing {
        +calculateFare(distanceKm, baseRate) double
    }

    class SurgePricing {
        -double surgeMultiplier
        +calculateFare(distanceKm, baseRate) double
    }

    PricingStrategy <|.. StandardPricing
    PricingStrategy <|.. SurgePricing

    class Ride {
        -string rideId
        -shared_ptr~Rider~ rider
        -shared_ptr~Driver~ driver
        -double fare
        -RideStatus status
        +assignDriver(driver)
        +calculateFare(strategy)
        +startRide()
        +completeRide() bool
    }

    Ride "1" --> "1" Rider
    Ride "1" --> "1" Driver
    Ride ..> PricingStrategy
```

---

## Build and Run Instructions

### Compilation Commands

```powershell
g++ main.cpp -o main -lmingw32
.\main
```
