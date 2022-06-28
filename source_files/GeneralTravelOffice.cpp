#include "GeneralTravelOffice.h"

#include <string>
#include <sstream>

using namespace std;

#include "Pilot.h"
#include "Car.h"
#include "RaceTrack.h"

GeneralTravelOffice::GeneralTravelOffice() : name("original") {}

GeneralTravelOffice::GeneralTravelOffice(const GeneralTravelOffice& general_travel_office_copy)
{
    copyCars(general_travel_office_copy.cars);
    copyPilots(general_travel_office_copy.pilots);
    copyRaceTracks(general_travel_office_copy.race_tracks);
}

GeneralTravelOffice::~GeneralTravelOffice()
{
    for(int i = 0; i < pilots.size(); i++)
    {
        delete pilots[i];
    }
    for(int i = 0; i < cars.size(); i++)
    {
        delete cars[i];
    }
    for(int i = 0; i < race_tracks.size(); i++)
    {
        delete race_tracks[i];
    }
}

void GeneralTravelOffice::operator=(const GeneralTravelOffice& general_travel_office_copy)
{
    for(int i = 0; i < pilots.size(); i++)
    {
        delete pilots[i];
    }
    pilots.clear();
    for(int i = 0; i < cars.size(); i++)
    {
        delete cars[i];
    }
    cars.clear();
    for(int i = 0; i < race_tracks.size(); i++)
    {
        delete race_tracks[i];
    }
    race_tracks.clear();

    copyCars(general_travel_office_copy.cars);
    copyPilots(general_travel_office_copy.pilots);
    copyRaceTracks(general_travel_office_copy.race_tracks);
}

void GeneralTravelOffice::copyPilots(const vector<Pilot*> &pilots_copy)
{
    for(int i = 0; i < pilots_copy.size(); i++)
    {
        /* if(pilots_copy[i]->getType() == CRAZY_DRIVER)
        {
            this->pilots.push_back(new CrazyDriver(*((CrazyDriver*)(pilots_copy[i]))));
        }
        if(pilots_copy[i]->getType() == FAST_PILOT)
        {
            this->pilots.push_back(new FastPilot(*((FastPilot*)(pilots_copy[i]))));
        }
        if(pilots_copy[i]->getType() == SURPRISE_PILOT)
        {
            this->pilots.push_back(new SurprisePilot(*((SurprisePilot*)(pilots_copy[i]))));
        } */

        this->pilots.push_back(pilots_copy[i]->clone());
    }

    for(int i = 0; i < pilots_copy.size(); i++)
    {
        const Car* car_temp = pilots_copy[i]->getCar();

        if(car_temp != nullptr)
        {
            for(int j = 0; j < cars.size(); j++)
            {
                if(car_temp->getId() == cars[j]->getId())
                {
                    pilots[i]->setCar(cars[j]);
                }
            }
        }
    }
}

void GeneralTravelOffice::copyCars(const vector<Car*> &cars_copy)
{
    for(int i = 0; i < cars_copy.size(); i++)
    {
        this->cars.push_back(cars_copy[i]->clone());
    }
}

void GeneralTravelOffice::copyRaceTracks(const vector<RaceTrack*> &race_tracks_copy)
{
    for(int i = 0; i < race_tracks_copy.size(); i++)
    {
        this->race_tracks.push_back(race_tracks_copy[i]->clone());
    }
}

string GeneralTravelOffice::getName() const
{
    return name;
}

void GeneralTravelOffice::setName(string name)
{
    this->name = name;
}

string GeneralTravelOffice::toString() const
{
    ostringstream output_string_stream;

    output_string_stream << "GTO name: " << name << endl;

    output_string_stream << "----- PILOTS -----" << endl;
    for(int i = 0; i < pilots.size(); i++)
    {
        output_string_stream << pilots[i]->toString() << endl;
    }

    output_string_stream << "----- CARS -----" << endl;
    for(int i = 0; i < cars.size(); i++)
    {
        output_string_stream << cars[i]->toString() << endl;
    }

    output_string_stream << "----- RACE TRACKS -----" << endl;
    for(int i = 0; i < race_tracks.size(); i++)
    {
        output_string_stream << race_tracks[i]->toString() << endl;
    }

    return output_string_stream.str();
}

bool GeneralTravelOffice::addPilot(string pilot_type, string pilot_name)
{
    Pilot* pilot =  Pilot::pilotsFactory(pilot_type, pilot_name);

    if(pilot != nullptr)
    {
        pilots.push_back(pilot);
        return true;
    }
    else
    {
        return false;
    }
}

void GeneralTravelOffice::addCar(int initial_energy, int max_energy, int max_speed, string brand, string model)
{
    if(model == "")
    {
        cars.push_back(new Car(brand, initial_energy, max_energy, max_speed));
    }
    else
    {
        cars.push_back(new Car(brand, initial_energy, max_energy, max_speed, model));
    }
    
}

void GeneralTravelOffice::addRaceTrack(int max_number_of_cars, int length, string name)
{
    race_tracks.push_back(new RaceTrack(max_number_of_cars, length, name));
}

bool GeneralTravelOffice::deletePilot(string pilot_name)
{
    for(auto pilot_iterator = pilots.begin(); pilot_iterator != pilots.end(); ++pilot_iterator)
    {
        if((*pilot_iterator)->getName() == pilot_name)
        {
            delete (*pilot_iterator);
            pilots.erase(pilot_iterator);
            return true;
        }
    }

    return false;
}

bool GeneralTravelOffice::deleteCar(string id)
{
    if(id.size() == 1)
    {
        for(auto car_iterator = cars.begin(); car_iterator != cars.end(); ++car_iterator)
        {
            if((*car_iterator)->getId() == id[0])
            {
                for(int i = 0; i < pilots.size(); i++)
                {
                    if(pilots[i]->getCar() != nullptr)
                    {
                        if(pilots[i]->getCar()->getId() == id[0])
                        {
                            pilots[i]->setCar(nullptr);
                            break;
                        }
                    }
                }

                delete (*car_iterator);
                cars.erase(car_iterator);

                return true;
            }
        }
    }
    
    return false;
}

bool GeneralTravelOffice::deleteRaceTrack(string race_track_name)
{
    for(auto race_track_iterator = race_tracks.begin(); race_track_iterator != race_tracks.end(); ++race_track_iterator)
    {
        if((*race_track_iterator)->getName() == race_track_name)
        {
            delete (*race_track_iterator);
            race_tracks.erase(race_track_iterator);

            return true;
        }
    }

    return false;
}

bool GeneralTravelOffice::pilotExists(string name) const
{
    for(int i = 0; i < pilots.size(); i++)
    {
        if(pilots[i]->getName() == name)
        {
            return true;
        }
    }

    return false;
}

bool GeneralTravelOffice::carExists(string id) const
{
    for(int i = 0; i < cars.size(); i++)
    {
        if(cars[i]->getId() == id[0])
        {
            return true;
        }
    }

    return false;
}

bool GeneralTravelOffice::raceTrackExists(string name) const
{
    for(int i = 0; i < race_tracks.size(); i++)
    {
        if(race_tracks[i]->getName() == name)
        {
            return true;
        }
    }

    return false;
}

bool GeneralTravelOffice::enterInCar(string car_id, string pilot_name)
{
    Pilot* pilot = nullptr;
    Car* car = nullptr;

    for(int i = 0; i < pilots.size(); i++)
    {
        if(pilots[i]->getName() == pilot_name && pilots[i]->getCar() == nullptr)
        {
            pilot = pilots[i];
            break;
        }
    }

    for(int i = 0; i < cars.size(); i++)
    {
        if(cars[i]->getId() == car_id[0] && cars[i]->getActualSpeed() == 0 && cars[i]->getDamagedState() == false)
        {
            car = cars[i];
        }
    }

    for(int i = 0; i < pilots.size(); i++)
    {
        if(pilots[i]->getCar() == car)
        {
            return false;
        }
    }

    if(pilot != nullptr && car != nullptr)
    {
        pilot->setCar(car);
        return true;
    }
    return false;
}

bool GeneralTravelOffice::getOutOfCar(string car_id)
{
    for(int i = 0; i < cars.size(); i++)
    {
        if(cars[i]->getId() == car_id[0])
        {
            for(int j = 0; j < pilots.size(); j++)
            {
                if(pilots[j]->getCar() == cars[i])
                {
                    pilots[j]->setCar(nullptr);
                    return true;
                }
            }
        }
    }

    return false;
}

vector<Pilot*> GeneralTravelOffice::getPilotsVector() const
{
    return pilots;
}

vector<RaceTrack*> GeneralTravelOffice::getRaceTracksVector() const
{
    return race_tracks;
}

vector<Car*> GeneralTravelOffice::getCarsVector() const
{
    return cars;
}