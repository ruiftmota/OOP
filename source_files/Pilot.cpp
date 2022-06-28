#include "Pilot.h"

#include <iostream>
#include <string>
#include <sstream>
#include <ctime>

using namespace std;

#include "Car.h"

Pilot* Pilot::pilotsFactory(string pilot_type, string pilot_name)
{
    pilot_name = Pilot::aproveName(pilot_name);

    if(pilot_type == CRAZY_DRIVER)
    {
        return new CrazyDriver(pilot_name);
    }
    else if(pilot_type == FAST_PILOT)
    {
        return new FastPilot(pilot_name);
    }
    else if(pilot_type == SURPRISE_PILOT)
    {
        return new SurprisePilot(pilot_name);
    }
    else
    {
        return nullptr;
    }
}

Pilot::Pilot(string name) : name(name),
                            car(nullptr),
                            stop(false),
                            last_instant_position(1),
                            count_accelarations(0) {}

vector<string> Pilot::names_used;

string Pilot::aproveName(string pilot_name) {
    string name_aux = pilot_name;
    int count_aux = 1;
    for(int i=0 ; i<names_used.size() ; i++){
        if(names_used[i] == name_aux){
            count_aux++;
            name_aux = changeName(pilot_name, count_aux);
            i=0;
        }
    }
    names_used.push_back(name_aux);
    return name_aux;
}

string Pilot::changeName(string pilot_name, int number_to_add){
    ostringstream name;
    name << pilot_name << number_to_add;
    return name.str();
}




string Pilot::getName() const { return name; }
Car* Pilot::getCar() const { return car; }
void Pilot::setCar(Car* car) { this->car = car; }
void Pilot::setLastInstantPosition(int position){ this->last_instant_position = position; }
int Pilot::getLastInstantPosition() const { return last_instant_position; }
string Pilot::toString() const
{
    ostringstream output_string_stream;

    output_string_stream << "Name:" << name;

    if(car != nullptr) { output_string_stream << " Car:" << car->getId(); }
    else { output_string_stream << " Car:none"; }

    return output_string_stream.str();
}
bool Pilot::getStop() const{ return stop; }
void Pilot::setStop(bool stop){ this->stop = stop; }


int Pilot::getCountAccelarations() const { return count_accelarations; }
void Pilot::addCountAccelarations() { ++count_accelarations;}
void Pilot::decreaseCountAccelarations() { --count_accelarations;}


bool Pilot::randInitializer = false;    
int Pilot::generateRandomNumber(int min, int max){
    if(Pilot::randInitializer==false){
        srand((unsigned)time(NULL));
        Pilot::randInitializer = true;
    }
    return rand()%max + min;

}





/* -------------------------------------------CRAZY DRIVER------------------------------------------ */

CrazyDriver::CrazyDriver(string name) : Pilot(name) {
    x_segundos_atraso = generateRandomNumber(1,5);
}
CrazyDriver::CrazyDriver(const CrazyDriver& crazy_driver) : Pilot(crazy_driver.getName()) {}

string CrazyDriver::getType() const
{
    return CRAZY_DRIVER;
}
string CrazyDriver::toString() const
{
    ostringstream output_string_stream;

    output_string_stream << Pilot::toString() << " Tipo:" << this->getType();

    return output_string_stream.str();
}

Pilot* CrazyDriver::clone() const
{
    return (Pilot*)new CrazyDriver(*this);
}

string CrazyDriver::act(int time, int position, int total_racers)
{
    if(time < x_segundos_atraso){
        this->setLastInstantPosition(position);
        return "0";
    }
    
    if(this->getCar() != nullptr)
    {
        if(this->getCar()->getDamagedState() != true && this->getCar()->getEmergencyState() != true)
        {
            if(this->getStop() == true)
            {
                this->getCar()->brake();
            }
            else if(this->getCountAccelarations() > 0)
            {
                this->getCar()->accelarate();
                this->decreaseCountAccelarations();
            }
            else if(position != 1)
            {
                if(position == total_racers)
                {
                    this->getCar()->brake();
                }
                else if(position > this->getLastInstantPosition())
                {
                    this->getCar()->accelarate();
                    this->addCountAccelarations();
                }
                else
                {
                    this->getCar()->accelarate();
                }
            }

            if(generateRandomNumber(1,100)<=5){
                this->getCar()->resetSpeed();
                this->getCar()->setDamagedstate();
                return "The crazy pilot " + getName() + " has damaged the car";
            }

            this->getCar()->move();
            this->setLastInstantPosition(position);

            if(this->getCar()->getEnergy() <= 0)
            {
                this->getCar()->setEmergencyState(true);
            }
        }
    }

    /* if(this->getCar()==nullptr)
        return;
    if(time < x_segundos_atraso){
        this->setLastInstantPosition(position);
        return;
    }
    if(this->getCar()->getDamagedState() == true){
        return;
    }
    if(this->getStop() == true){
        this->getCar()->brake();
        this->setLastInstantPosition(position);
        this->getCar()->move();
        return;
    }
    if(this->getCountAccelarations()>0){
        this->getCar()->accelarate();
        this->decreaseCountAccelarations();
        this->getCar()->move();
        return;
    }
    if(position < this->getLastInstantPosition()){
        if(this->getCar() != nullptr){
            this->getCar()->accelarate();
            this->addCountAccelarations();
        }
    }
    if(this->getCar() != nullptr){
        if(position == 1){
            this->setLastInstantPosition(position);
        }else if(position == total_racers){
            this->setLastInstantPosition(position);
            this->getCar()->brake();
        }else if(position != 1 && position != total_racers){
            this->setLastInstantPosition(position);
            this->getCar()->accelarate();
        }
    }
    this->getCar()->move(); */

    /* this->getCar()->accelarate();
    this->getCar()->move(); */

    return "0";
}

/* -------------------------------------------FAST PILOT--------------------------------------------- */

FastPilot::FastPilot(string name) : Pilot(name) {}
FastPilot::FastPilot(const FastPilot& fast_pilot) : Pilot(fast_pilot.getName()),
                                                    count_acelerations_after_half_energy(0) {}

string FastPilot::getType() const
{
    return FAST_PILOT;
}
string FastPilot::toString() const
{
    ostringstream output_string_stream;

    output_string_stream << Pilot::toString() << " Tipo:" << this->getType();

    return output_string_stream.str();
}

Pilot* FastPilot::clone() const
{
    return (Pilot*)new FastPilot(*this);
}

string FastPilot::act(int time, int position, int total_racers){
    if(this->getCar() != nullptr)
    {
        if(this->getCar()->getDamagedState() != true && this->getCar()->getEmergencyState() != true)
        {
            if(this->getStop() == true)
            {
                this->getCar()->brake();
            }
            else if(this->getCar()->getEnergy()*2 > this->getCar()->getMaxEnergy() ){
                this->getCar()->accelarate();
            }else{
                count_acelerations_after_half_energy++;
                if(count_acelerations_after_half_energy % 3 == 0){
                    this->getCar()->accelarate();
                }
            }
            

            if(generateRandomNumber(1,100)<=10){
                this->getCar()->resetSpeed();
                this->getCar()->setEmergencyState(true);
                return "The fast pilot " + getName() + " has activated the emergency state.";
            }
            
            this->getCar()->move();
            this->setLastInstantPosition(position);

            if(this->getCar()->getEnergy() <= 0)
            {
                this->getCar()->setEmergencyState(true);
            }
        }
    }
    return "0";
}




/* ----------------------------------------------SURPRISE PILOT----------------------------------- */

SurprisePilot::SurprisePilot(string name) : Pilot(name) {}
SurprisePilot::SurprisePilot(const SurprisePilot& surprise_pilot) : Pilot(surprise_pilot.getName()) {}

string SurprisePilot::getType() const
{
    return SURPRISE_PILOT;
}
string SurprisePilot::toString() const
{
    ostringstream output_string_stream;

    output_string_stream << Pilot::toString() << " Tipo:" << this->getType();

    return output_string_stream.str();
}

Pilot* SurprisePilot::clone() const
{
    return (Pilot*)new SurprisePilot(*this);
}

string SurprisePilot::act(int time, int position, int total_racers)
{
    if(this->getCar() != nullptr)
    {
        if(this->getCar()->getDamagedState() != true && this->getCar()->getEmergencyState() != true)
        {
            if(this->getStop() == true)
            {
                this->getCar()->brake();
            }
            
            if(generateRandomNumber(1,100)<=50)
            {
                this->getCar()->accelarate();
            }

            this->getCar()->move();
            this->setLastInstantPosition(position);
        }
    }

    return "0";
}