#include "SimulatorLogic.h"

#include "Championship.h"

#include <sstream>
#include <fstream>

using namespace std;

ErrorException::ErrorException(string error_information) : error_information(error_information), exception() {}

const char* ErrorException::what() const throw()
{
    return error_information.c_str();
}

/* -----------------------------------------------------------------------------------------------------------------------------------------------------*/

SimulatorLogic::SimulatorLogic() : mode(MODE_1), championship_is_configured(false)
{
    this->general_travel_office_backups.push_back(new GeneralTravelOffice());
    this->actual_general_travel_office = general_travel_office_backups[0];
}
SimulatorLogic::SimulatorLogic(const SimulatorLogic &simulator_logic) : championship(nullptr) {}
SimulatorLogic::~SimulatorLogic()
{
    for(int i = 0; i < general_travel_office_backups.size(); i++)
    {
        delete general_travel_office_backups[i];
    }
}

void SimulatorLogic::writeToLog(string message)
{
    message_log.push_back(message);

    if(message_log.size() > 10)
    {
        message_log.erase(message_log.begin());
    }
}

int SimulatorLogic::convertStringToInt(string number) const
{
    istringstream number_stream(number);
    int int_value;

    number_stream >> int_value;

    return int_value;
}

bool SimulatorLogic::isInteger(string number) const
{
    for(int i = 0; i < number.size(); i++)
    {
        if(number[i] < '0' || number[i] > '9')
        {
            return false;
        }
    }

    return true;
}

bool SimulatorLogic::validateCreateFunctionInput(vector<string> tokenized_information) const
{
    if(tokenized_information[0] == PILOT_CHARACTER)
    {
        if(tokenized_information.size() - 1 >= 2)
        {
            return true;
        }
    }
    else if(tokenized_information[0] == CAR_CHARACTER)
    {
        if(tokenized_information.size() - 1 >= 4)
        {
            if(isInteger(tokenized_information[1]) && isInteger(tokenized_information[2]) && isInteger(tokenized_information[3]))
            {
                return true;
            }
        }
    }
    else if(tokenized_information[0] == RACE_TRACK_CHARACTER)
    {
        if(tokenized_information.size() - 1 == 3)
        {
            if(isInteger(tokenized_information[1]) && isInteger(tokenized_information[2]))
            {
                return true;
            }
        }
    }

    return false;
}

vector<string> SimulatorLogic::tokenizeString(const string &information) const
{
    vector<string> tokenized_information;

    istringstream input_stream(information);
    string temp;

    while(input_stream.eof() == false)
    {
        temp.clear();
        input_stream >> ws;
        input_stream >> temp;
        if(temp.size() > 0)
        {
            tokenized_information.push_back(temp);
        }
    }

    return tokenized_information;
}

int SimulatorLogic::getMode()
{
    return mode;
}

vector<string> SimulatorLogic::getMap() const
{
    if(raceInCourse() == false)
    {
        throw ErrorException("There is no race in course to return the map!");
    }

    return championship->getMap();
}

vector<string> SimulatorLogic::getLeaderboard() const
{
    if(championship_is_configured != false && raceInCourse() == false)
    {
        return championship->getLeaderboard();
    }

    throw ErrorException("There is no championship in course to return the leaderboard!");
}

vector<string> SimulatorLogic::getRaceInformation() const
{
    if(championship_is_configured != false && raceInCourse() == true)
    {
        try{
            return championship->getRaceInformation();
        }catch(const exception &e){

        }
        
    }

    throw ErrorException("There is no race in course to return the race information!");
}

void SimulatorLogic::exitSimulation()
{
    this->mode = EXIT_SIMULATION;
}

void SimulatorLogic::loadPilots(string file_name)
{
    fstream file_stream(file_name);
    ostringstream error_stream;
    int number_of_errors = 0;

    string line;
    string pilot_type;
    string pilot_name;

    if(file_stream.is_open())
    {
        for(int i = 0; getline(file_stream, line); i++)
        {
            vector<string> tokenized_information = tokenizeString(line);
            tokenized_information.insert(tokenized_information.begin(), "p");

            if(validateCreateFunctionInput(tokenized_information) != true)
            {
                error_stream << "The pilot in the line " << i + 1 << " is not in a valid format." << endl;
                number_of_errors++;

                continue;
            }

            pilot_type = tokenized_information[1];
            pilot_name = tokenized_information[2];
            for(int j = 3; j < tokenized_information.size(); j++)
            {
                pilot_name = pilot_name + " " + tokenized_information[j];
            }

            try
            {
                if(actual_general_travel_office->addPilot(pilot_type, pilot_name) == true)
                {
                    writeToLog("Pilot of type " + pilot_type + " named " + pilot_name + " created successfully.");
                }
                else
                {
                    error_stream << "Pilot type " + pilot_type + " doesn't exist." << endl;
                    number_of_errors++;
                }
            }
            catch(const bad_alloc& e)
            {
                error_stream << "Error allocating memory for pilot of type " + pilot_type + " called " + pilot_name + " " << endl;
                number_of_errors++;
            }
        }



        file_stream.close();
    }
    else
    {
        error_stream << "Error opening " + file_name + " file." << endl;
        number_of_errors++;
    }

    if(number_of_errors > 0)
    {
        throw ErrorException(error_stream.str());
    }
}

void SimulatorLogic::loadCars(string file_name)
{
    fstream file_stream(file_name);
    ostringstream error_stream;
    int number_of_errors = 0;

    string line;
    string brand;
    string model;
    int max_energy;
    int initial_energy;
    int max_speed;

    if(file_stream.is_open())
    {
        for(int i = 0; getline(file_stream, line); i++)
        {
            vector<string> tokenized_information = tokenizeString(line);
            tokenized_information.insert(tokenized_information.begin(), "c");

            if(validateCreateFunctionInput(tokenized_information) != true)
            {
                error_stream << "The car in the line " << i + 1 << " is not in a valid format." << endl;
                number_of_errors++;

                continue;
            }

            max_energy = convertStringToInt(tokenized_information[1]);
            initial_energy = convertStringToInt(tokenized_information[2]);
            max_speed = convertStringToInt(tokenized_information[3]);
            brand = tokenized_information[4];
            if(tokenized_information.size() > 4) model = tokenized_information[5];
            for(int j = 6; j < tokenized_information.size(); j++)
            {
                model = model + " " + tokenized_information[j];
            }

            try
            {
                actual_general_travel_office->addCar(initial_energy, max_energy, max_speed, brand, model);

                writeToLog("Car of brand " + brand + " and model " + model + " created successfully");
            }
            catch(const bad_alloc& e)
            {
                error_stream << "Error allocating memory for car of brand " + brand + " and model " + model << endl;
                number_of_errors++;
            }
        }

        file_stream.close();
    }
    else
    {
        error_stream << "Error opening " + file_name + " file." << endl;
        number_of_errors++;
    }

    if(number_of_errors > 0)
    {
        throw ErrorException(error_stream.str());
    }
}

void SimulatorLogic::loadRaceTracks(string file_name)
{
    fstream file_stream(file_name);
    ostringstream error_stream;
    int number_of_errors = 0;

    string line;

    string name;
    int max_number_of_cars;
    int length;

    if(file_stream.is_open())
    {
        for(int i = 0; getline(file_stream, line); i++)
        {
            vector<string> tokenized_information = tokenizeString(line);
            tokenized_information.insert(tokenized_information.begin(), "r");

            if(validateCreateFunctionInput(tokenized_information) != true)
            {
                error_stream << "The race track in the line " << i + 1 << " is not in a valid format." << endl;
                number_of_errors++;

                continue;
            }

            max_number_of_cars = convertStringToInt(tokenized_information[1]);
            length = convertStringToInt(tokenized_information[2]);
            name = tokenized_information[3];

            try
            {
                actual_general_travel_office->addRaceTrack(max_number_of_cars, length, name);

                error_stream << "Racetrack named " + name + " was created successfully" << endl;
            }
            catch(const bad_alloc& e)
            {
                error_stream << "Error allocating memory for racetrack named " + name << endl;
                number_of_errors++;
            }
        }

        file_stream.close();
    }
    else
    {
        error_stream << "Error opening " + file_name + " file." << endl;
        number_of_errors++;
    }

    if(number_of_errors > 0)
    {
        throw ErrorException(error_stream.str());
    }
}

void SimulatorLogic::create(vector<string> tokenized_information)
{
    string type_character = tokenized_information[0];

    if(type_character == PILOT_CHARACTER)
    {
        string pilot_type;
        string pilot_name;

        pilot_type = tokenized_information[1];
        pilot_name = tokenized_information[2];
        for(int i = 3; i < tokenized_information.size(); i++)
        {
            pilot_name = pilot_name + " " + tokenized_information[i];
        }

        try
        {
            if(actual_general_travel_office->addPilot(pilot_type, pilot_name) == true)
            {
                writeToLog("Pilot of type " + pilot_type + " named " + pilot_name + " created successfully.");
            }
            else
            {
                throw ErrorException("Pilot type " + pilot_type + " doesn't exist.");
            }
        }
        catch(const bad_alloc& e)
        {
            throw ErrorException("Error allocating memory for pilot of type " + pilot_type + " called " + pilot_name + " ");
        }
    }
    else if(type_character == CAR_CHARACTER)
    {
        string brand;
        string model;
        int max_energy;
        int initial_energy;
        int max_speed;

        max_energy = convertStringToInt(tokenized_information[1]);
        initial_energy = convertStringToInt(tokenized_information[2]);
        max_speed = convertStringToInt(tokenized_information[3]);
        brand = tokenized_information[4];
        for(int i = 5; i < tokenized_information.size(); i++)
        {
            model = model + " " + tokenized_information[i];
        }

        try
        {
            actual_general_travel_office->addCar(initial_energy, max_energy, max_speed, brand, model);

            writeToLog("Car of brand " + brand + " and model " + model + " created successfully");
        }
        catch(const bad_alloc& e)
        {
            throw ErrorException("Error allocating memory for car of brand " + brand + " and model " + model);
        }
    }
    else if(type_character == RACE_TRACK_CHARACTER)
    {
        string name;
        int max_number_of_cars;
        int length;

        max_number_of_cars = convertStringToInt(tokenized_information[1]);
        length = convertStringToInt(tokenized_information[2]);
        name = tokenized_information[3];

        try
        {
            actual_general_travel_office->addRaceTrack(max_number_of_cars, length, name);

            writeToLog("Racetrack named " + name + " was created successfully");
        }
        catch(const bad_alloc& e)
        {
            throw ErrorException("Error allocating memory for racetrack named " + name);
        }
    }
    else
    {
        throw ErrorException("The type refered by '" + type_character + "' doesn't extist.");
    }
    
}

void SimulatorLogic::eliminate(string type_character, string id)
{
    if(type_character == PILOT_CHARACTER)
    {
        if(actual_general_travel_office->deletePilot(id) == true)
        {
            writeToLog("The pilot named " + id + " was deleted successfully.");
        }
        else
        {
            throw ErrorException("The pilot named " + id + " was not found to delete.");
        }
    }
    else if(type_character == CAR_CHARACTER)
    {
        if(actual_general_travel_office->deleteCar(id) == true)
        {
            writeToLog("The car with ID " + id + " was deleted successfully.");
        }
        else
        {
            throw ErrorException("The car with ID " + id + " was not found to delete.");
        }
    }
    else if(type_character == RACE_TRACK_CHARACTER)
    {
        if(actual_general_travel_office->deleteRaceTrack(id) == true)
        {
            writeToLog("The race track named " + id + " was deleted successfully.");
        }
        else
        {
            throw ErrorException("The race track named " + id + " was not found to delete.");
        }
    }
    else
    {
        throw ErrorException("The type refered by '" + type_character + "' doesn't extist.");
    }
}

void SimulatorLogic::enterInCar(string car_character, vector<string> tokenized_pilot_name)
{
    string pilot_name = tokenized_pilot_name[0];
    for(int i = 1; i < tokenized_pilot_name.size(); i++)
    {
        pilot_name = pilot_name + " " + tokenized_pilot_name[i];
    }

    if(actual_general_travel_office->pilotExists(pilot_name) == true)
    {
        if(actual_general_travel_office->carExists(car_character) == true)
        {
            if(actual_general_travel_office->enterInCar(car_character, pilot_name) == true)
            {
                writeToLog("The pilot named " + pilot_name + " enterd in car with ID " + car_character + " successfully");
            }
            else
            {
                throw ErrorException("It was not possible to put pilot named " + pilot_name + " in car with ID " + car_character);
            }
        }
        else
        {
            throw ErrorException("The car with ID " + car_character + " was not found to put a pilot inside.");
        }
    }
    else
    {
        throw ErrorException("The pilot named " + pilot_name + " was not found to enter in a car.");
    }
}

void SimulatorLogic::getOutOfCar(string car_character)
{
    if(actual_general_travel_office->carExists(car_character) == true)
    {
        if(actual_general_travel_office->getOutOfCar(car_character) == true)
        {
            writeToLog("Pilot in car with ID " + car_character + " got out successfully.");
        }
        else
        {
            throw ErrorException("Car with ID " + car_character + " didn't have a pilot inside.");
        }
        
    }
    else
    {
        throw ErrorException("The car with ID " + car_character + " was not found to take pilot out.");
    }
    
}

string SimulatorLogic::list()
{
    ostringstream list_stream;

    list_stream << "Saved GTOs: ";
    for(int i = 0; i < general_travel_office_backups.size(); i++)
    {
        list_stream << general_travel_office_backups[i]->getName() << ", ";
    }

    list_stream << endl << endl;

    list_stream << actual_general_travel_office->toString() << endl;

    return list_stream.str();
}

void SimulatorLogic::saveGeneralTravelOffice(string name)
{
    for(int i = 0; i < general_travel_office_backups.size(); i++)
    {
        if(general_travel_office_backups[i]->getName() == name)
        {
            throw ErrorException("There is already a GTO backup named " + name + ", it's not possible to create a new one.");
            return;
        }
    }

    GeneralTravelOffice* new_general_travel_office = new GeneralTravelOffice(*actual_general_travel_office);
    new_general_travel_office->setName(name);
    general_travel_office_backups.push_back(new_general_travel_office);

    writeToLog("GTO backup named " + name + " was saved successfully.");
}

void SimulatorLogic::loadGeneralTravelOffice(string name)
{
    for(int i = 0; i < general_travel_office_backups.size(); i++)
    {
        if(general_travel_office_backups[i]->getName() == name)
        {
            this->actual_general_travel_office = general_travel_office_backups[i];
            writeToLog("GTO backup named " + name + " was loaded successfully.");
            return;
        }
    }

    throw ErrorException("GTO backup named " + name + " was not found.");
}

void SimulatorLogic::deleteGeneralTravelOffice(string name)
{
    for(auto gto_iterator = general_travel_office_backups.begin(); gto_iterator != general_travel_office_backups.end(); ++gto_iterator)
    {
        if((*gto_iterator)->getName() == name && (*gto_iterator) != actual_general_travel_office)
        {
            delete (*gto_iterator);
            general_travel_office_backups.erase(gto_iterator);
            
            writeToLog("GTO backup named " + name + " was deleted successfully");
            return;
        }
    }

    throw ErrorException("It was not possible to delete GTO backup named " + name + ".");
}

void SimulatorLogic::startChampionship()
{
    vector<Pilot*> pilots = actual_general_travel_office->getPilotsVector();
    vector<RaceTrack*> race_tracks = actual_general_travel_office->getRaceTracksVector();
    vector<Car*> cars = actual_general_travel_office->getCarsVector();

    try
    {
        this->championship = new Championship(pilots, race_tracks, cars);
    }
    catch(const bad_alloc& e)
    {
        throw ErrorException("Error allocating memory for the Championship.");
    }

    this->mode = MODE_2;
}

void SimulatorLogic::associateRacetracksToChampionship(vector<string> race_tracks)
{
    if(championship_is_configured == true)
    {
        throw ErrorException("There is already a championship running, finish it first.");
    }

    for(int i = 0; i < race_tracks.size(); i++)
    {
        if(actual_general_travel_office->raceTrackExists(race_tracks[i]) == false)
        {
            throw ErrorException("The race track named " + race_tracks[i] + " doesn't exist.");
        }
    }

    championship->selectRaceTracks(race_tracks);

    string log = "Championship was created with the following tracks:";
    for(int i = 0; i < race_tracks.size(); i++)
    {
        log = log + " " + race_tracks[i];
    }

    writeToLog(log);

    championship_is_configured = true;
}

string SimulatorLogic::listCars()
{
    if(championship_is_configured == false)
    {
        throw ErrorException("The championship isn't configured yet.");
    }

    return championship->getListCars();
}

void SimulatorLogic::chargeBattery(string car_character, string mAh)
{
    if(championship_is_configured == false)
    {
        throw ErrorException("The championship isn't configured yet.");
    }

    if(isInteger(mAh) == false)
    {
        throw ErrorException(mAh + " is not a valid amount for the battery.");
    }

    if(car_character.size() == 1)
    {
        if(championship->chargeBattery(car_character[0], (float)convertStringToInt(mAh)) == true)
        {
            writeToLog("Battery of car with ID " + car_character + " was charged successfully with " + mAh + " mAh.");
        }
        else
        {
            throw ErrorException("The car with ID " + car_character + " is not competing or is not stopped");
        }
    }
    else
    {
        throw ErrorException("The car with ID " + car_character + " doesn't exist to charge battery");
    }
}

void SimulatorLogic::chargeAllBatteries()
{
    if(championship_is_configured == false)
    {
        throw ErrorException("The championship isn't configured yet.");
    }

    championship->chargeAllCars();
}

void SimulatorLogic::startRace()
{
    if(championship_is_configured == false)
    {
        throw ErrorException("The championship isn't configured yet.");
    }

    string race_name;
    if(championship->getActualRaceNumber()+1 < championship->getTotalRaceNumber())
    {
        try
        {
            race_name = championship->nextRace();
        }
        catch(const bad_alloc& e)
        {
            throw ErrorException("Error allocating memory for the next race.");
        }

        if(race_name == "impossible")
        {
            throw ErrorException("There is already a race in course. Can't create a new one!");
        }
        if(race_name == "last_race")
        {
            throw ErrorException("There are no more races to start!");
        }

        writeToLog("Race in track named " + race_name + " started successfully");
    }
    else
    {
        writeToLog("All races in this championship have done");
    }
    
}

void SimulatorLogic::provoceAccident(string car_character)
{
    if(championship_is_configured == false)
    {
        throw ErrorException("The championship isn't configured yet.");
    }

    if(car_character.size() == 1 && actual_general_travel_office->carExists(car_character))
    {
        if(championship->accident(car_character[0]) == true)
        {
            writeToLog("Car with ID " + car_character + " had an accident successfully.");
        }
        else
        {
            throw ErrorException("The car with ID " + car_character + " is not competing");
        }
    }
    else
    {
        throw ErrorException("The car with ID " + car_character + " doesn't exist to provoce an accident.");
    }
}

void SimulatorLogic::stopPilot(string pilot_name)
{
    if(championship_is_configured == false)
    {
        throw ErrorException("The championship isn't configured yet.");
    }

    if(actual_general_travel_office->pilotExists(pilot_name) == false)
    {
        throw ErrorException("The pilot named " + pilot_name + " doesn't exist");
    }

    if((championship->stopPilot(pilot_name)) == true)
    {
        writeToLog("Pilot named " + pilot_name + " was stopped successfully.");
    }
    else
    {
        throw ErrorException("Pilot named " + pilot_name + " is not competing.");
    }
}

void SimulatorLogic::destroiCar(string car_character)
{
    
    if(championship_is_configured == false)
    {
        throw ErrorException("The championship isn't configured yet.");
    }

    if(actual_general_travel_office->carExists(car_character) == true)
    {
        championship->removeCarFromChampionship(car_character[0]);
        actual_general_travel_office->deleteCar(car_character);
        writeToLog("The car with ID " + car_character + " was deleted successfully.");
    }
    else
    {
        throw ErrorException("The car with ID " + car_character + " was not found to delete.");
    }
}

void SimulatorLogic::passTime()
{
    if(championship_is_configured == false)
    {
        throw ErrorException("The championship isn't configured yet.");
    }

    if(championship->passTime() == false)
    {
        throw ErrorException("There is no race in course! Start a race first.");
    }
}

vector<string> SimulatorLogic::log() 
{    
    if(championship_is_configured == true)
    {
        vector<string> logTemp = championship->getLog();

        for(int i = 0; i < logTemp.size(); i++)
        {
            writeToLog(logTemp[i]);
        }
    }

    return message_log;
}

void SimulatorLogic::leaveChampionship()
{
    delete championship;
    championship = nullptr;

    this->mode = MODE_1;
    championship_is_configured = false;
}

bool SimulatorLogic::raceInCourse() const
{
    if(championship_is_configured == false)
    {
        return false;
    }

    return championship->raceInCourse();
}

bool SimulatorLogic::championshipInCourse() const
{
    return (championship_is_configured == true);
}