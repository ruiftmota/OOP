#include "RaceTrack.h"

#include <string>
#include <sstream>
#include <iostream>

#include "Race.h"

using namespace std;

RaceTrack::RaceTrack(int max_number_of_cars, int length, string name) : max_number_of_cars(max_number_of_cars),
                                                                        length(length),
                                                                        name(name),
                                                                        race(nullptr) {}
RaceTrack::RaceTrack(const RaceTrack &race_track_copy) : max_number_of_cars(race_track_copy.max_number_of_cars),
                                                         length(race_track_copy.length),
                                                         name(race_track_copy.name),
                                                         race(nullptr) {}

RaceTrack* RaceTrack::clone() const
{
    return new RaceTrack(*this);
}

string RaceTrack::getName() const
{
    return name;
}

string RaceTrack::toString() const
{
    ostringstream output_string_stream;

    output_string_stream << "Name:" << name << " MaxCars:" << max_number_of_cars << " Length:" << length;

    return output_string_stream.str();
}


const Race* RaceTrack::getRace() const{
    return race;
}



void RaceTrack::startRace(vector<Pilot*> pilots){

    vector<Pilot*> pilotsToRace;
    for(int i=0 ; i<this->max_number_of_cars && i<pilots.size() ; i++)
    {
        pilotsToRace.push_back(pilots[i]);
        if(pilots[i]->getCar() != nullptr)
        {
            pilots[i]->getCar()->resetSpeed();
        }
    }
    for(int i=this->max_number_of_cars ; i< pilots.size() ; i++)
    {
        pilotsInGarage.push_back(pilots[i]);
        if(pilots[i]->getCar() != nullptr)
        {
            pilots[i]->getCar()->resetSpeed();
        }
    }
    race = new Race(pilotsToRace, length);
    
    
}


void RaceTrack::deleteRace(){
    if(race != nullptr)
    {
        delete race;
        race = nullptr;
    }
    for(int i=0; i<pilots.size() ; i++){
        if(pilots[i]!= nullptr){
            if(pilots[i]->getCar() != nullptr){
                pilots[i]->getCar()->resetSpeed();
            }
        }
    }
}

bool RaceTrack::passTime()
{
    bool alreadyFinished = race->passTime();
    if(alreadyFinished == false){
        deleteRace();
    }
    return alreadyFinished;
}

vector<string> RaceTrack::getMap(){
    if(race!=nullptr)
        return race->getMap();
    else
    throw exception();
}

bool RaceTrack::getRaceIsFinished() const { return race->getIsFinised(); }

vector<string> RaceTrack::getRaceInformation() const
{
    vector<string> race_information = race->getRaceInformation();

    race_information.insert(race_information.begin(), this->name);

    return race_information;
}

const vector<Pilot*> RaceTrack::getPilotsByOrder(){
    if(race != nullptr)
        return race->getPilotsByOrder();
    else 
        throw exception();
}

const vector<Pilot*> RaceTrack::getPilotsThatFinished(){
    if(race != nullptr)
        return race->getPilotsThatFinished();
    else 
        throw exception();
}


bool RaceTrack::removeCarFromRaceTrack(char letter)
{
    for(int i=0 ; i<pilots.size() ; i++){
        if(pilots[i]->getCar() != nullptr){
            if(pilots[i]->getCar()->getId() == letter){
                pilots[i]->setCar(nullptr);
            }
        }
    }
    for(int i=0 ; i<pilotsInGarage.size() ; i++){
        if(pilotsInGarage[i]->getCar() != nullptr){
            if(pilotsInGarage[i]->getCar()->getId() == letter){
                pilotsInGarage[i]->setCar(nullptr);
                pilotsInGarage.erase(pilotsInGarage.begin() + i);
            }
        }
    }

    for(int i=0 ; i<carsInGarage.size() ; i++){
        if(carsInGarage[i]->getId() == letter){
            carsInGarage.erase(carsInGarage.begin()+i);
        }
    }

    race->removeCarFromRace(letter);

    return true;
}

vector<string> RaceTrack::getLogProbabilities(){
    if(race != nullptr){
        return race->getLog();
    }
    throw exception();
}