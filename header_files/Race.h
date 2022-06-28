#ifndef RACE_H
#define RACE_H

#include <vector>
#include <string>

using namespace std;

#include "Pilot.h"

class RaceInfo{
    private:
        Pilot* pilot;
        int distance;
        int position;
        int trackLength;
    public:
        RaceInfo(Pilot* pilot, int trackLength);
        void addDistance(int distance);
        void setPosition(int position);
        int getDistance() const;
        int getPosition() const ;
        Pilot* getPilot() const;

        string toString() const;
};

class Race{
    private:
        vector<Pilot*> pilots;
        vector<RaceInfo> raceInfos;
        int time;
        int trackLength;
        bool isFinished;

        vector<Pilot*> pilotsThatFinished;
        vector<Pilot*> pilotsByOrder;

        vector<string> log;

        void readyInfoRaces();

        void discardPilotsWithDamagedCars();

    public:
        Race(vector<Pilot*> pilots, int trackLength);


        int getTime() const;
        void increaseTime();
        
        bool getIsFinised() const;

        bool passTime();

        bool pilotExists(vector<Pilot*> pilots, Pilot* p);

        void seeIfAnyoneFinished();

        bool seeIfEveryoneFinished();

        const vector<Pilot*> getPilotsByOrder();
        const vector<Pilot*> getPilotsThatFinished();

        vector<string> getMap() const;

        vector<string> getRaceInformation() const;

        bool removeCarFromRace(char letter);

        vector<string> getLog();
};

#endif