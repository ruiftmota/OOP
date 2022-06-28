#include "Car.h"

#include <string>
#include <sstream>

using namespace std;


char Car::idSerie = 'a' - 1;

Car::Car(string brand, float initial_energy, float max_energy, int max_speed, string model) : brand(brand), 
                                                                                              model(model), 
                                                                                              id(Car::returnActualLetter()), 
                                                                                              max_energy(max_energy),
                                                                                              max_speed(max_speed),
                                                                                              emergency_state(false),
                                                                                              damaged_state(false),
                                                                                              actual_speed(0),
                                                                                              energy(initial_energy)
{
    if(this->energy > this->max_energy)
    {
        this-> energy = this-> max_energy;
    }
}

Car::Car(const Car &car) : brand(car.brand), 
                           model(car.model), 
                           id(car.id), 
                           max_energy(car.max_energy),
                           max_speed(car.max_speed),
                           emergency_state(car.emergency_state),
                           damaged_state(car.damaged_state),
                           actual_speed(car.actual_speed),
                           energy(car.energy) {}

Car* Car::clone() const
{
    return new Car(*this);
}

char Car::returnActualLetter()
{
    if(Car::idSerie >= 'z')
    {
        return '?';
    }
    else
    {
        Car::idSerie++;
        return idSerie;
    }
}

bool Car::chargeCar(float chargingValue){
    if(isStoped()){
        if(damaged_state == false && chargingValue > 0){
            this->energy += chargingValue; 
            if(this->energy > this->max_energy)
                energy = max_energy;
            return true;
        }
        return false;
    } 
    return false;
}

void Car::maxChargeCar(){
    if(isStoped()  && damaged_state == false)
        this->energy = this->max_energy;
}

void Car::accelarate(){ 
    if(emergency_state == false && damaged_state == false && energy > 0)
        if(actual_speed < max_speed) 
            actual_speed++; 
}

void Car::brake(){ 
    if(damaged_state == false)
        if(actual_speed > 0) 
            actual_speed--; 
}

void Car::move(){
    if(damaged_state == false){
        energy -= actual_speed*0.1;
        if(energy <= 0){
            energy = 0;
            brake();
        }
    }
}   

void Car::resetSpeed(){ this->actual_speed = 0; }
void Car::setEmergencyState(bool emergency_state){ this->emergency_state = emergency_state; }
void Car::setDamagedstate(){ 
    this->damaged_state = true; 
    this->actual_speed = 0;
}

char Car::getId() const { return id; }
float Car::getEnergy() const { return energy; }
float Car::getMaxEnergy() const { return max_energy; }
int Car::getActualSpeed() const { return actual_speed; }
int Car::getMaxSpeed() const { return max_speed; }
bool Car::getEmergencyState() const { return emergency_state; }
bool Car::getDamagedState() const { return damaged_state; }
bool Car::isStoped() const { return (actual_speed == 0); }

string Car::toString() const
{
    ostringstream output_string_stream;

    output_string_stream << "ID:" << id << " Brand:" << brand << " Model:" << model; 
    output_string_stream << " Energy:"<< energy << " Max Energy:" << max_energy << " Max Speed:" << max_speed; 

    return output_string_stream.str();
}

string Car::getBrand() const { return brand; }

string Car::toStringEverything() const {
    ostringstream output_string_stream;

    output_string_stream << toString() << " Emergency State:";
    if(emergency_state == false)
        output_string_stream << " false" ; 
    else
        output_string_stream << " true";

    output_string_stream << "Damaged State:";
    if(damaged_state == false)
        output_string_stream << " false" ; 
    else
        output_string_stream << " true";     

    output_string_stream << " Actual Speed:" << actual_speed;


    return output_string_stream.str();
}