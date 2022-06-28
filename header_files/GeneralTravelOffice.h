#ifndef GENERAL_TRAVEL_OFFICE_H
#define GENERAL_TRAVEL_OFFICE_H

#include <string>
#include <vector>

#include "Pilot.h"
#include "Car.h"
#include "RaceTrack.h"

using namespace std;

class GeneralTravelOffice
{
    private:
        string name;

        vector<Pilot*> pilots;
        void copyPilots(const vector<Pilot*> &pilots_copy);
        vector<Car*> cars;
        void copyCars(const vector<Car*> &cars_copy);
        vector<RaceTrack*> race_tracks;
        void copyRaceTracks(const vector<RaceTrack*> &race_tracks_copy);

    public:
        GeneralTravelOffice();
        GeneralTravelOffice(const GeneralTravelOffice& general_travel_office_copy);
        ~GeneralTravelOffice();
        void operator=(const GeneralTravelOffice& general_travel_office_copy);

        string getName() const;
        void setName(string name);
        string toString() const;

        bool addPilot(string pilot_type, string pilot_name);
        void addCar(int initial_energy, int max_energy, int max_speed, string brand, string model);
        void addRaceTrack(int max_number_of_cars, int length, string name);

        bool deletePilot(string pilot_name);
        bool deleteCar(string id);
        bool deleteRaceTrack(string race_track_name);

        bool pilotExists(string name) const;
        bool carExists(string id) const;
        bool raceTrackExists(string name) const;

        bool enterInCar(string car_id, string pilot_name);
        bool getOutOfCar(string car_id);

        vector<Pilot*> getPilotsVector() const;
        vector<RaceTrack*> getRaceTracksVector() const;
        vector<Car*> getCarsVector() const;
};

#endif