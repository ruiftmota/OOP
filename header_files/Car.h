#ifndef CAR_H
#define CAR_H

#include <string>

using namespace std;

class Car
{
    private:
        static char idSerie;
        const char id;

        const string brand;
        const string model;

        float energy;
        const float max_energy;

        
        bool emergency_state;
        bool damaged_state;

        int actual_speed;
        const int max_speed;

        static char returnActualLetter();

    public:
        Car(string brand, float initial_energy, float max_energy, int max_speed, string model = "modelo base");
        Car(const Car &car);

        Car* clone() const;

        bool chargeCar(float chargingValue);
        void maxChargeCar();

        void accelarate();
        void brake();
        void move();

        void resetSpeed();
        void setEmergencyState(bool emergency_state);
        void setDamagedstate();

        char getId() const;
        float getEnergy() const;
        float getMaxEnergy() const;
        int getActualSpeed() const;
        int getMaxSpeed() const;
        bool getEmergencyState() const;
        bool getDamagedState() const;
        bool isStoped() const;
        string getBrand() const;

        string toString() const;

        string toStringEverything() const;
};

#endif