#include "Race.h"

#include <vector>
#include <sstream>
#include <iostream>

using namespace std;

//---------------------------------------------------------RACEINFO---------------------------------------------------


RaceInfo::RaceInfo(Pilot* pilot, int trackLength):pilot(pilot), 
                                                distance(0), 
                                                position(1),
                                                trackLength(trackLength) {}


void RaceInfo::addDistance(int distance){ 
    this->distance += distance;
    if(this->distance > trackLength)
        this->distance = trackLength;
}
void RaceInfo::setPosition(int position){ this->position = position; }
int RaceInfo::getDistance() const { return distance; }
int RaceInfo::getPosition() const { return position; }
Pilot* RaceInfo::getPilot() const { return pilot; }

string RaceInfo::toString() const{ 
    ostringstream oSS;
    if(pilot->getCar() != nullptr){
        oSS << pilot->getCar()->getId() << " ";
        oSS << distance << endl;
    }
    
    return oSS.str();
}





//---------------------------------------------------------Race--------------------------------------------------------

Race::Race(vector<Pilot*> pilots, int trackLength):pilots(pilots),
                                                   time(0),
                                                   trackLength(trackLength),
                                                   isFinished(false)
{
    discardPilotsWithDamagedCars();
    readyInfoRaces();
    if(pilots.size()<2)
        throw exception();

    for(int i = 0; i < this->pilots.size(); i++)
    {
        this->pilotsByOrder.push_back(this->pilots[i]);
    }
}

void Race::discardPilotsWithDamagedCars(){
    for(int i=0 ; i<pilots.size() ; i++){
        if(pilots[i] != nullptr){
            if(pilots[i]->getCar() != nullptr){
                if(pilots[i]->getCar()->getDamagedState() == true){
                    pilots.erase(pilots.begin()+i);
                }
            }else if(pilots[i]->getCar() == nullptr){
                pilots.erase(pilots.begin()+i);
            }
        }
    }
}

void Race::readyInfoRaces(){
    for(int i=0 ; i<pilots.size() ; i++){
        RaceInfo aux(pilots[i], trackLength);
        raceInfos.push_back(aux);
    }    
}      

bool Race::getIsFinised() const { return isFinished; }

int Race::getTime() const { return time; }

bool Race::passTime(){
    time++;
    int distance_aux;
    int position_aux;
    string logP;
    for(int i=0 ; i<pilots.size() ; i++){
        if(pilots[i]->getCar() != nullptr){
            logP = pilots[i]->act(time,raceInfos[i].getPosition(), pilots.size());
            raceInfos[i].addDistance(pilots[i]->getCar()->getActualSpeed());
            distance_aux = raceInfos[i].getDistance();
            position_aux = 1;
            for(int j=0 ; j<raceInfos.size() ; j++){
                if(raceInfos[j].getDistance() > distance_aux){
                    position_aux++;
                }
            }
            raceInfos[i].setPosition(position_aux);
            if(logP != "0"){
                log.push_back(logP);
            }
        }
    }
    seeIfAnyoneFinished();

    bool alreadyEnded= !seeIfEveryoneFinished();
    if(alreadyEnded == false){
        isFinished = true;
        
    }

    

    return alreadyEnded;
}


bool Race::pilotExists(vector<Pilot*> pilots, Pilot* p){
    for(int i=0 ; i< pilots.size() ; i++){
        if(pilots[i]->getName() == p->getName()){
            return true;
        }
    }
    return false;
}


//ADICIONA QUE TIVER TERMINADO A CORRIDA COM SUCESSO
void Race::seeIfAnyoneFinished(){
    for(int i=0 ; i<raceInfos.size() ; i++){
        if(raceInfos[i].getDistance() == trackLength){
            if(pilotExists(pilotsThatFinished, raceInfos[i].getPilot()) == false){
                pilotsThatFinished.push_back(raceInfos[i].getPilot());
                if(raceInfos[i].getPilot()->getCar() != nullptr)
                    raceInfos[i].getPilot()->getCar()->resetSpeed();
            }
        }
    }
}


//VERIFICA SE TODA A GENTE TERMINOU COM OU SEM SUCESSO
bool Race::seeIfEveryoneFinished()
{
    int number_of_finished = 0;

    for(int i=0; i<raceInfos.size() ; i++)
    {
        if(raceInfos[i].getPilot()->getCar() == nullptr){
            number_of_finished++;
        }
        else if(raceInfos[i].getDistance() == trackLength || raceInfos[i].getPilot()->getStop() == true || 
            raceInfos[i].getPilot()->getCar()->getDamagedState() == true || 
            raceInfos[i].getPilot()->getCar()->getEmergencyState() == true)
        {
            number_of_finished++;
        }
    }

    if(number_of_finished == raceInfos.size())
    {
        return true;
    }
    else
    {
        return false;
    }
}


const vector<Pilot*> Race::getPilotsByOrder(){
    pilotsByOrder.clear();
    int pilots_pos[raceInfos.size()];
    int pilots_distance[raceInfos.size()];

    for(int i=0 ; i<raceInfos.size() ; i++){

        pilots_pos[i] = i;
        pilots_distance[i] = raceInfos[i].getDistance();
    }
    int aux;
    for( int i=0 ; i < raceInfos.size() ; i++){
        for(int j=0 ; j < raceInfos.size()-1 ; j++){
            if(pilots_distance[pilots_pos[j]] < pilots_distance[pilots_pos[j+1]]){
                aux = pilots_distance[j];
                pilots_distance[j] = pilots_distance[j+1];
                pilots_distance[j+1] = aux;
                pilots_pos[j]++;
                pilots_pos[j+1]--;
            }
        }
    }

    for(int i=0; i<raceInfos.size() ; i++){
        if(raceInfos[pilots_pos[i]].getPilot() != nullptr)
            pilotsByOrder.push_back(raceInfos[pilots_pos[i]].getPilot());
    }


    //-----------------------------------------------------------------//



    //CODIGO CRITICO QUE DEFINE POSICOES
    for(int i=0 ; i<raceInfos.size() ; i++){
        raceInfos[pilots_pos[i]].setPosition(i+1);
    }
    //CODIGO CRITICO QUE DEFINE POSICOES
    


    //-------------------------------------------------------------------//


    return pilotsByOrder;
}

const vector<Pilot*> Race::getPilotsThatFinished(){ return pilotsThatFinished; }

vector<string> Race::getMap() const{
    vector<string> infoVector;
    ostringstream oSS;
    oSS << trackLength;

    infoVector.push_back(oSS.str());
    
    for(int i=0 ; i<raceInfos.size() ; i++){
        for(int j=0; j<raceInfos.size() ; j++){
            if(pilotsByOrder[i]->getName() == raceInfos[j].getPilot()->getName()){
                infoVector.push_back(raceInfos[j].toString());
            }   
        }
        
    }
    return infoVector;
}


vector<string> Race::getRaceInformation() const
{
    vector<string> race_information_strings;

    for(int i = 0; i < raceInfos.size(); i++)
    {
        if(raceInfos[i].getPilot()->getCar() != nullptr){
            ostringstream race_information_stream;
            race_information_stream << raceInfos[i].getPilot()->getCar()->getId() << " ";
            race_information_stream << raceInfos[i].getPilot()->getCar()->getBrand() << " ";
            race_information_stream << raceInfos[i].getPilot()->getName() << " ";
            race_information_stream << raceInfos[i].getPilot()->getType() << " ";
            race_information_stream << raceInfos[i].getPilot()->getCar()->getActualSpeed() / 10 << " ";
            race_information_stream << raceInfos[i].getPilot()->getCar()->getEnergy() << " ";
            race_information_stream << raceInfos[i].getDistance() << " ";
            race_information_stream << raceInfos[i].getPilot()->getCar()->getActualSpeed();

            race_information_strings.push_back(race_information_stream.str());
        }
        
    }

    return race_information_strings;
}


bool Race::removeCarFromRace(char letter){
    for(int i=0 ; i<pilots.size() ; i++){
        if(pilots[i]->getCar() != nullptr){
            if(pilots[i]->getCar()->getId() == letter){
                pilots[i]->setCar(nullptr);
            }
        }
    }
    for(int i=0 ; i<pilotsThatFinished.size() ; i++){
        if(pilotsThatFinished[i]->getCar() != nullptr){
            if(pilotsThatFinished[i]->getCar()->getId() == letter){
                pilotsThatFinished[i]->setCar(nullptr);
                //ADICIONAR ERASE SE ESTOURAR
            }
        }
    }

    for(int i=0 ; i<pilotsByOrder.size() ; i++){
        if(pilotsByOrder[i]->getCar() != nullptr){
            if(pilotsByOrder[i]->getCar()->getId() == letter){
                pilotsByOrder[i]->setCar(nullptr);
                pilotsByOrder.erase(pilotsByOrder.begin() + i );//ADICIONAR ERASE SE ESTOURAR
            }
        }
        
    }

    for(int i=0 ; i< raceInfos.size() ; i++){
        if(raceInfos[i].getPilot()->getCar() != nullptr){
            if(raceInfos[i].getPilot()->getCar()->getId() == letter){
                raceInfos.erase(raceInfos.begin()+i);
            }
        }
    }

    return true;
}



vector<string> Race::getLog(){
    vector<string> aux = log;
    log.clear();
    return aux;
}
