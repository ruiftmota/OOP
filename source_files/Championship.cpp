#include <vector>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

#include "Championship.h"
#include "RaceTrack.h"

// -----------------------------------------------------SCORES-----------------------------------------------------

Score::Score(Pilot* pilot) : pilot(pilot),
                             score(0) {}

int Score::getScore() const { return score; }
Pilot* Score::getPilot() const { return pilot; }

void Score::addScore(int value){ this->score += value; }

string Score::toString(){ 
    ostringstream aux; 

    aux << score << " " << pilot->getName();

    return aux.str();
}

//------------------------------------------------------CHAMPIONSHIP---------------------------------------------------

Championship::Championship(vector<Pilot*> pilots, vector<RaceTrack*> raceTracks, vector<Car*> cars): pilots(pilots),
                                                                                                     raceTracks(raceTracks),
                                                                                                     cars(cars),
                                                                                                     race_number(0)
{
    discardPilotsWithDamagedCars();
    readyScores();
}

Championship::~Championship(){
    if(raceTracks.empty() != true)
    {
        if(raceTracks[race_number]->getRace() != nullptr)
        {
            raceTracks[race_number]->deleteRace();
        }
    }
}

void Championship::discardPilotsWithDamagedCars(){
    for(int i=0 ; i<pilots.size() ; i++){
        if(pilots[i]!= nullptr){
            if(pilots[i]->getCar() != nullptr){
                if(pilots[i]->getCar()->getDamagedState() == true){
                    pilots.erase(pilots.begin()+i);
                }
            }else if(pilots[i]->getCar() == nullptr){
                pilots.erase(pilots.begin()+i);
            }
        }else
        {
            pilots.erase(pilots.begin()+i);
        }
        
    }
}

void Championship::readyScores() {
    for(int i=0 ; i<pilots.size() ; i++){
        Score aux(pilots[i]);
        scores.push_back(aux);
    }
}


void Championship::assignScores()
{
    /*try{
        const vector<Pilot*> pilotsThatFinished = raceTracks[race_number]->getPilotsThatFinished();
        if(pilotsThatFinished.size() > 0){
            for(int i=0; i<scores.size() ; i++){
                if(scores[i].getPilot()->getName() == pilotsThatFinished[0]->getName()){
                    scores[i].addScore(10);
                }
            }
        }
        if(pilotsThatFinished.size() > 1){
            for(int i=0; i<scores.size() ; i++){
                if(scores[i].getPilot()->getName() == pilotsThatFinished[1]->getName()){
                    scores[i].addScore(5);
                }
            }
        }
        if(pilotsThatFinished.size() > 2){
            for(int i=0; i<scores.size() ; i++){
                if(scores[i].getPilot()->getName() == pilotsThatFinished[2]->getName()){
                    scores[i].addScore(2);
                }
            }
        }
    }catch(const exception &e){
        cout << "championship assign scores";
    }
    */
        
}


void Championship::selectRaceTracks(vector<string> racetrackNames){
    vector<RaceTrack*> newVector;

    for(int i=0 ; i< racetrackNames.size() ; i++){
        for(int j=0; j<raceTracks.size() ; j++){
            if(racetrackNames[i] == raceTracks[j]->getName()){
                newVector.push_back(raceTracks[j]);
                break;
            }
        }
    }
    raceTracks = newVector;
}

string Championship::getListCars(){ 
    ostringstream output_string_stream;
    for(int i=0; i<pilots.size() ; i++){
        if(pilots[i] != nullptr){
            if(pilots[i]->getCar() != nullptr)
                output_string_stream << pilots[i]->getCar()->toStringEverything() << endl;
        }
    }
    return output_string_stream.str();
}

bool Championship::chargeBattery(char letter, int quantity){
    for(int i=0 ; i<pilots.size() ; i++){
        if(pilots[i] != nullptr){
            if(pilots[i]->getCar() != nullptr){
                if(pilots[i]->getCar()->getId() == letter){
                    return pilots[i]->getCar()->chargeCar(quantity);
                }
            }
        }
    }
    return false;
}

void Championship::chargeAllCars(){
    for(int i=0 ; i<pilots.size() ; i++){
        if(pilots[i] != nullptr){
            if(pilots[i]->getCar() != nullptr){
                pilots[i]->getCar()->maxChargeCar();
            }
        }
    }
}

string Championship::nextRace(){
    if(race_number != raceTracks.size()){
        const Race* raceAux = raceTracks[race_number]->getRace();
        if(raceAux == nullptr){
            try{
                raceTracks[race_number]->startRace(pilots);
                return raceTracks[race_number]->getName();
            }catch(const exception &ex){
                cout << "not enough pilots";
                return "not_enough_pilots";
            }
        }
        return "impossible";
    }
    return "last_race";
}

void Championship::endRace(){
    for(int i=0 ; i<pilots.size() ; i++){
        if(pilots[i]->getStop() == true){
            pilots[i]->setStop(false);
        }
    }
    if(this->getActualRaceNumber()< this->getTotalRaceNumber())
        race_number++;
}

bool Championship::accident(char letter){
    for(int i=0 ; i<pilots.size() ; i++){
        if(pilots[i] != nullptr){
            if(pilots[i]->getCar() != nullptr){
                if(pilots[i]->getCar()->getId() == letter){
                    pilots[i]->getCar()->setDamagedstate();
                    return true;
                }
            }
        }
    }
    return false;
}

bool Championship::removeCarFromChampionship(char letter)
{
    for(int i=0 ; i<cars.size() ; i++){
        if(cars[i]->getId() == letter){
            cars.erase(cars.begin() + i);
        }
    }

    raceTracks[race_number]->removeCarFromRaceTrack(letter);

    return true;
}

bool Championship::stopPilot(string pilot_name){
    for(int i=0 ; i<pilots.size() ; i++){
        if(pilots[i] != nullptr){
            if(pilots[i]->getCar() != nullptr){
                if(pilots[i]->getName() == pilot_name){
                    pilots[i]->setStop(true);
                    return true;
                }
            }
        }
    }
    return false;
}

vector<string> Championship::getLog() {
    vector<string> emptyVector;
    try{
        return raceTracks[race_number]->getLogProbabilities();
    }catch(const exception &e) { return emptyVector; }
}

bool Championship::passTime(){
    const Race* raceAux = raceTracks[race_number]->getRace();
    if(raceAux == nullptr)
        return false;
    
    bool alreadyFinished = raceTracks[race_number]->passTime();
    if(alreadyFinished == false){
        endRace();
        assignScores();
    }
    return alreadyFinished;
}


vector<string> Championship::getMap() const {
    return raceTracks[race_number]->getMap();
}

vector<string> Championship::getLeaderboard() const
{
    vector<Score> ordered_scores = getOrderedScores();

    vector<string> ordered_scores_strings;

    for(int i = 0; i < ordered_scores.size(); i++)
    {
        ostringstream scores_stream;

        scores_stream << i + 1 << " " << ordered_scores[i].toString();

        ordered_scores_strings.push_back(scores_stream.str());
    }

    return ordered_scores_strings;
}

vector<string> Championship::getRaceInformation() const
{
    if(raceTracks[race_number]->getRaceIsFinished() == false)
    {
        return raceTracks[race_number]->getRaceInformation();
    }

    throw exception();
}

bool Championship::raceInCourse() const
{
    if(raceTracks[race_number]->getRace() == nullptr)
        return false;
    return true;
}


vector<Score> Championship::getOrderedScores() const {
    vector<Score> ordered_scores;

    int scores_pos[scores.size()];
    int scores_value[scores.size()];

    for(int i=0 ; i<scores.size() ; i++){

        scores_pos[i] = i;
        scores_value[i] = scores[i].getScore();
    }
    int aux;
    for( int i=0 ; i < scores.size() ; i++){
        for(int j=0 ; j < scores.size()-1 ; j++){
            if(scores_value[scores_pos[j]] < scores_value[scores_pos[j+1]]){
                aux = scores_value[j];
                scores_value[j] = scores_value[j+1];
                scores_value[j+1] = aux;
                scores_pos[j]++;
                scores_pos[j+1]--;
            }
        }
    }

    for(int i=0; i<scores.size() ; i++){
        ordered_scores.push_back(scores[scores_pos[i]]);
    }
    
    return ordered_scores;
}


int Championship::getActualRaceNumber() const{ return race_number; }
int Championship::getTotalRaceNumber() const{ return raceTracks.size(); }