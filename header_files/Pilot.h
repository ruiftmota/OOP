#ifndef PILOT_H
#define PILOT_H

#include <string>
#include <vector>

#include "Car.h"

using namespace std;

#define CRAZY_DRIVER "crazy"
#define FAST_PILOT "fast"
#define SURPRISE_PILOT "surprise"

class Pilot
{
    private:
        const string name;
        Car* car;
        int last_instant_position;
        bool stop;
        int count_accelarations;

        static vector<string> names_used;

        static string aproveName(string pilot_name);
        static string changeName(string pilot_name, int number_to_add);

        static bool randInitializer;

        vector<string> logProbabilities;

    public:
        static Pilot* pilotsFactory(string pilot_type, string pilot_name);

        Pilot(string name);
        string getName() const;
        Car* getCar() const;
        void setCar(Car* car);
        int getLastInstantPosition() const;
        void setLastInstantPosition(int position);
        virtual string toString() const;

        bool getStop() const;
        void setStop(bool stoped);

        int getCountAccelarations() const;
        void addCountAccelarations();
        void decreaseCountAccelarations();
        
        
        int generateRandomNumber(int min, int max);

        virtual string getType() const = 0;

        virtual Pilot* clone() const = 0;

        virtual string act(int time, int position, int total_racers) = 0;
};

class CrazyDriver : public Pilot
{
    private:
        int x_segundos_atraso;
    public:
        CrazyDriver(string name);
        CrazyDriver(const CrazyDriver& crazy_driver);

        string getType() const override;
        string toString() const override;

        Pilot* clone() const override;
        string act(int time, int position, int total_racers) override;
};

class FastPilot : public Pilot
{
    private:
        int count_acelerations_after_half_energy;

    public:
        FastPilot(string name);
        FastPilot(const FastPilot& fast_pilot);

        string getType() const override;
        string toString() const override;

        Pilot* clone() const override;
        string act(int time, int position, int total_racers) override;
};

class SurprisePilot : public Pilot
{
    public:
        SurprisePilot(string name);
        SurprisePilot(const SurprisePilot& surprise_pilot);

        string getType() const override;
        string toString() const override;

        Pilot* clone() const override;
        string act(int time, int position, int total_racers) override;
};

#endif