#ifndef SIMULATOR_LOGIC_H
#define SIMULATOR_LOGIC_H

#include <string>
#include <vector>

#include "GeneralTravelOffice.h"
#include "Championship.h"

#define PILOT_CHARACTER "p"
#define RACE_TRACK_CHARACTER "r"
#define CAR_CHARACTER "c"

using namespace std;

class ErrorException : public exception
{
    private:
        string error_information;

    public:
        ErrorException(string error_information);
        const char* what() const throw ();
};

class SimulatorLogic
{
    private:
        int mode;
        bool championship_is_configured;

        vector<GeneralTravelOffice*> general_travel_office_backups;
        GeneralTravelOffice* actual_general_travel_office;

        Championship *championship;

        vector<string> message_log;

        void writeToLog(string message);

        int convertStringToInt(string number) const;
        bool isInteger(string number) const;
        vector<string> tokenizeString(const string &information) const;

    public:
        SimulatorLogic();
        SimulatorLogic(const SimulatorLogic &simulator_logic);
        ~SimulatorLogic();

        enum modes
        {
            MODE_1 = 1,
            MODE_2 = 2,
            EXIT_SIMULATION = 0
        };

        int getMode();

        vector<string> getMap() const;
        vector<string> getLeaderboard() const;
        vector<string> getRaceInformation() const;

        bool validateCreateFunctionInput(vector<string> tokenized_information) const;

        void exitSimulation();

        void loadPilots(string file_name);
        void loadCars(string file_name);
        void loadRaceTracks(string file_name);
        void create(vector<string> tokenized_information);
        void eliminate(string type_character, string id);
        void enterInCar(string car_character, vector<string> tokenized_pilot_name);
        void getOutOfCar(string car_character);
        string list();
        void saveGeneralTravelOffice(string name);
        void loadGeneralTravelOffice(string name);
        void deleteGeneralTravelOffice(string name);
        void startChampionship();

        void associateRacetracksToChampionship(vector<string> race_tracks);
        string listCars();
        void chargeBattery(string car_character, string mAh);
        void chargeAllBatteries();
        void startRace();
        void provoceAccident(string car_character);
        void stopPilot(string pilot_name);
        void destroiCar(string car_character);
        void passTime();
        vector<string> log();
        void leaveChampionship();
        bool raceInCourse() const;
        bool championshipInCourse() const;
};

#endif