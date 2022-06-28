#ifndef RACE_TRACK_H
#define RACE_TRACK_H

#include <string>
#include <vector>

using namespace std;

#include "Car.h"
#include "Pilot.h"
#include "Race.h"

class RaceTrack
{
    private:
        const string name;
        const int max_number_of_cars;
        const int length;

        Race *race;
        vector<Pilot*> pilots;

        vector<Pilot*> pilotsInGarage;
        vector<Car*> carsInGarage;


    public:
        RaceTrack(int max_number_of_cars, int length, string name);
        RaceTrack(const RaceTrack &race_track_copy);
        
        RaceTrack* clone() const;

        string getName() const;
        string toString() const;

        const Race* getRace() const;

        bool getRaceIsFinished() const;

        vector<string> getRaceInformation() const;

        void startRace(vector<Pilot*> pilots);

        void deleteRace();

        bool passTime();

        vector<string> getMap();

        const vector<Pilot*> getPilotsByOrder();
        const vector<Pilot*> getPilotsThatFinished();

        bool removeCarFromRaceTrack(char letter);

        vector<string> getLogProbabilities();
};

#endif