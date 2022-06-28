#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <vector>
#include <string>

#include "SimulatorLogic.h"
#include "Consola.h"

using namespace std;

class Command
{
    private:
        string name;
        string arguments_description;

        int countWords(const string &command) const;

    public:
        Command(string name, string arguments_description);
        Command(string name);
        string toString() const;
        int getNumberOfArguments() const;
        string getName() const;
};

class UserInterface
{
    private:
        SimulatorLogic &simulator_logic;

        vector<Command*> mode1_commands;
        vector<Command*> mode2_commands;

        string last_error;

        void fillMode1CommandsVector();
        void fillMode2CommandsVector();

        void runMode1();
        void runMode2();

        string printModeCommandsAndGetInput(const vector<Command*> &mode_commands, int y, int x);

        void printMap(int x, int y) const;
        void printLeaderboard(int x, int y) const;
        void printRaceInformation(int x, int y) const;
        void printLog(const vector<string> message_log, int x, int y) const;
        void printList(const string list, int x, int y) const;
        void printBox(int x, int y, int height, int width) const;
        void printLastErrors(int x, int y) const;

        vector<string> tokenizeCommand(const string &command) const;
        int convertStringToInt(string number) const;
        bool isInteger(string number) const;

    public:
        UserInterface(SimulatorLogic &simulator_logic);
        ~UserInterface();

        void run();
};

#endif