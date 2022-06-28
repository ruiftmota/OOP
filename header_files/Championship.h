#ifndef CHAMPIONSHIP_H
#define CHAMPIONSHIP_H

#include <vector>
#include <string>

using namespace std;

#include "Pilot.h"
#include "RaceTrack.h"

class Score{
    private:
        Pilot *pilot;
        int score;

    public:
        Score(Pilot *pilot);

        Pilot* getPilot() const;
        int getScore() const;
        void addScore(int value);

        string toString();


};

class Championship
{
    private:
        vector<Pilot*> pilots;
        vector<RaceTrack*> raceTracks;
        vector<Car*> cars;
        
        vector<Score> scores;

        int race_number;

        void discardPilotsWithDamagedCars();
        void readyScores();
        void assignScores();

        vector<Score> getOrderedScores() const;
        

    public:
        Championship(vector<Pilot*> pilots, vector<RaceTrack*> raceTracks, vector<Car*> cars);
        ~Championship();
        
        vector<string> getLeaderboard() const;

        vector<string> getRaceInformation() const;

        bool raceInCourse() const;

        void selectRaceTracks(vector<string> raceTracks);

        string getListCars();

        bool chargeBattery(char letter, int quantity);

        void chargeAllCars();

        string nextRace();

        bool accident(char letter);

        bool removeCarFromChampionship(char letter);

        bool stopPilot(string pilot_name);

        void endRace();

        bool passTime();

        vector<string> getMap() const;

        vector<string> getLog();


        int getActualRaceNumber() const;
        int getTotalRaceNumber() const;


        //Associacao pilotos carros antes de campeonato começa
        // 10,5,2 pontos

        
};



#endif