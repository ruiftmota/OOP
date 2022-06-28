#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

#include "UserInterface.h"
#include "SimulatorLogic.h"
#include "Consola.h"

#define DEFAULT_BACKGROUND_COLOR Consola::PRETO
#define BORDER_COLOR Consola::BRANCO
#define TRACK_BORDER_COLOR Consola::VERDE

#define PRINT_LOG_X 90
#define PRINT_LOG_Y 2

#define PRINT_MAP_X 90
#define PRINT_MAP_Y 25

#define PRINT_LEADERBOARD_X 90
#define PRINT_LEADERBOARD_Y 10

#define PRINT_RACE_INFORMATION_X 90
#define PRINT_RACE_INFORMATION_Y 10

#define PRINT_LIST_X 3
#define PRINT_LIST_Y 2

#define PRINT_COMMANDS_X 3
#define PRINT_COMMANDS_Y 2

#define MAP_SCALE 50

UserInterface::UserInterface(SimulatorLogic &simulator_logic) : simulator_logic(simulator_logic)
{
    fillMode1CommandsVector();
    fillMode2CommandsVector();
}

UserInterface::~UserInterface()
{
    for(int i = 0; i < mode1_commands.size(); i++)
    {
        delete mode1_commands[i];
    }

    for(int i = 0; i < mode2_commands.size(); i++)
    {
        delete mode2_commands[i];
    }
}

void UserInterface::run()
{
    Consola::setBackgroundColor(DEFAULT_BACKGROUND_COLOR);

    while(simulator_logic.getMode() != SimulatorLogic::EXIT_SIMULATION)
    {
        Consola::clrscr();

        if(simulator_logic.getMode() == SimulatorLogic::MODE_1)
        {
            runMode1();
        }
        else if(simulator_logic.getMode() == SimulatorLogic::MODE_2)
        {
            printLeaderboard(PRINT_LEADERBOARD_X, PRINT_LEADERBOARD_Y);
            printMap(PRINT_MAP_X, PRINT_MAP_Y);
            printRaceInformation(PRINT_RACE_INFORMATION_X, PRINT_RACE_INFORMATION_Y);
            runMode2();
        }
    }
}

void UserInterface::fillMode1CommandsVector()
{
    mode1_commands.push_back(new Command("carregaP", "<nomeFicheiro>"));
    mode1_commands.push_back(new Command("carregaC", "<nomeFicheiro>"));
    mode1_commands.push_back(new Command("carregaA", "<nomeFicheiro>"));
    mode1_commands.push_back(new Command("cria", "<letraTipo> <dadosDoObjeto>"));
    mode1_commands.push_back(new Command("apaga", "<letraTipo> <identificador>"));
    mode1_commands.push_back(new Command("entranocarro", "<letraCarro> <nomePiloto>"));
    mode1_commands.push_back(new Command("saidocarro", "<letraCarro>"));
    mode1_commands.push_back(new Command("lista"));
    mode1_commands.push_back(new Command("savedgv", "<nome>"));
    mode1_commands.push_back(new Command("loaddgv", "<nome>"));
    mode1_commands.push_back(new Command("deldgv", "<nome>"));
    mode1_commands.push_back(new Command("campeonato"));
    mode1_commands.push_back(new Command("log"));
    mode1_commands.push_back(new Command("sair"));
}
void UserInterface::fillMode2CommandsVector()
{
    mode2_commands.push_back(new Command("campeonato", "<A1> <A2> <An>"));
    mode2_commands.push_back(new Command("listacarros"));
    mode2_commands.push_back(new Command("carragabat", "<letraCarro> <Q>"));
    mode2_commands.push_back(new Command("carregatudo"));
    mode2_commands.push_back(new Command("corrida"));
    mode2_commands.push_back(new Command("acidente", "<letraCarro>"));
    mode2_commands.push_back(new Command("stop", "<nomePiloto>"));
    mode2_commands.push_back(new Command("destroi", "<letraCarro>"));
    mode2_commands.push_back(new Command("passatempo", "<n>"));
    mode2_commands.push_back(new Command("log"));
    mode2_commands.push_back(new Command("sair"));
}

void UserInterface::runMode1()
{
    string introduced_command;
    vector<string> tokenized_command;

    introduced_command = printModeCommandsAndGetInput(mode1_commands, PRINT_COMMANDS_Y, PRINT_COMMANDS_X);
    tokenized_command = tokenizeCommand(introduced_command);

    try
    {
        if(tokenized_command[0] == "sair" && tokenized_command.size() == 1)
        {
            simulator_logic.exitSimulation();
        }
        else if(tokenized_command[0] == "carregaP" && tokenized_command.size() == 2)
        {
            simulator_logic.loadPilots(tokenized_command[1]);
        }
        else if(tokenized_command[0] == "carregaC" && tokenized_command.size() == 2)
        {
            simulator_logic.loadCars(tokenized_command[1]);
        }
        else if(tokenized_command[0] == "carregaA" && tokenized_command.size() == 2)
        {
            simulator_logic.loadRaceTracks(tokenized_command[1]);
        }
        else if(tokenized_command[0] == "cria" && tokenized_command.size() >= 3)
        {
            tokenized_command.erase(tokenized_command.begin());

            if(simulator_logic.validateCreateFunctionInput(tokenized_command))
            {
                simulator_logic.create(tokenized_command);
            }
            else
            {
                last_error = "'" + introduced_command + "' is not a valid input. Try again!";
            }
        }
        else if(tokenized_command[0] == "apaga" && tokenized_command.size() == 3)
        {
            simulator_logic.eliminate(tokenized_command[1], tokenized_command[2]);
        }
        else if(tokenized_command[0] == "entranocarro" && tokenized_command.size() >= 3)
        {
            string car_id = tokenized_command[1];
            tokenized_command.erase(tokenized_command.begin());
            tokenized_command.erase(tokenized_command.begin());

            simulator_logic.enterInCar(car_id, tokenized_command);
        }
        else if(tokenized_command[0] == "saidocarro" && tokenized_command.size() == 2)
        {
            simulator_logic.getOutOfCar(tokenized_command[1]);
        }
        else if(tokenized_command[0] == "lista" && tokenized_command.size() == 1)
        {
            printList(simulator_logic.list(), PRINT_LIST_X, PRINT_LIST_Y);
        }
        else if(tokenized_command[0] == "savedgv" && tokenized_command.size() == 2)
        {
            simulator_logic.saveGeneralTravelOffice(tokenized_command[1]);
        }
        else if(tokenized_command[0] == "loaddgv" && tokenized_command.size() == 2)
        {
            simulator_logic.loadGeneralTravelOffice(tokenized_command[1]);
        }
        else if(tokenized_command[0] == "deldgv" && tokenized_command.size() == 2)
        {
            simulator_logic.deleteGeneralTravelOffice(tokenized_command[1]);
        }
        else if(tokenized_command[0] == "campeonato" && tokenized_command.size() == 1)
        {
            simulator_logic.startChampionship();
        }
        else if(tokenized_command[0] == "log" && tokenized_command.size() == 1)
        {
            printLog(simulator_logic.log(), PRINT_LOG_X, PRINT_LOG_Y);
        }
        else
        {
            last_error = "'" + introduced_command + "' is not a valid input. Try again!";
        }
    }
    catch(const ErrorException& e)
    {
        last_error = e.what();
    }
}
void UserInterface::runMode2()
{
    string introduced_command;
    vector<string> tokenized_command;

    introduced_command = printModeCommandsAndGetInput(mode2_commands, PRINT_COMMANDS_Y, PRINT_COMMANDS_X);
    tokenized_command = tokenizeCommand(introduced_command);

    try
    {
        if(tokenized_command[0] == "sair" && tokenized_command.size() == 1)
        {
            simulator_logic.leaveChampionship();
        }
        else if(tokenized_command[0] == "campeonato" && tokenized_command.size() >= 2)
        {
            tokenized_command.erase(tokenized_command.begin());
            
            simulator_logic.associateRacetracksToChampionship(tokenized_command);
        }
        else if(tokenized_command[0] == "listacarros" && tokenized_command.size() == 1)
        {
            printMap(PRINT_MAP_X, PRINT_MAP_Y);
            printList(simulator_logic.listCars(), PRINT_LIST_X, PRINT_LIST_Y);
        }
        else if(tokenized_command[0] == "carregabat" && tokenized_command.size() == 3)
        {
            simulator_logic.chargeBattery(tokenized_command[1], tokenized_command[2]);
        }
        else if(tokenized_command[0] == "carregatudo" && tokenized_command.size() == 1)
        {
            simulator_logic.chargeAllBatteries();
        }
        else if(tokenized_command[0] == "corrida" && tokenized_command.size() == 1)
        {
            simulator_logic.startRace();
        }
        else if(tokenized_command[0] == "acidente" && tokenized_command.size() == 2)
        {
            simulator_logic.provoceAccident(tokenized_command[1]);
        }
        else if(tokenized_command[0] == "stop" && tokenized_command.size() == 2)
        {
            simulator_logic.stopPilot(tokenized_command[1]);
        }
        else if(tokenized_command[0] == "destroi" && tokenized_command.size() == 2)
        {
            simulator_logic.destroiCar(tokenized_command[1]);
        }
        else if(tokenized_command[0] == "passatempo" && tokenized_command.size() == 2)
        {
            if(isInteger(tokenized_command[1]) == true)
            {
                int pass_time = convertStringToInt(tokenized_command[1]);

                for(int i = 0; i < pass_time; i++)
                {
                    Consola::clrscr();

                    simulator_logic.passTime();
                    printMap(PRINT_MAP_X, PRINT_MAP_Y);
                    printRaceInformation(PRINT_RACE_INFORMATION_X, PRINT_RACE_INFORMATION_Y);
                    while(Consola::getch() != Consola::ENTER);
                }
            }
        }
        else if(tokenized_command[0] == "log" && tokenized_command.size() == 1)
        {
            printMap(PRINT_MAP_X, PRINT_MAP_Y);
            printLog(simulator_logic.log(), PRINT_LOG_X, PRINT_COMMANDS_Y);
        }
        else
        {
            last_error = "'" + introduced_command + "' is not a valid input. Try again!";
        }
        
    }
    catch(const ErrorException& e)
    {
        last_error = e.what();   
    }
}

string UserInterface::printModeCommandsAndGetInput(const vector<Command*> &mode_commands, int y, int x)
{
    string introduced_command;

    printBox(x - 2, y - 2, 40, 60);

    Consola::gotoxy(x, y);
    if(simulator_logic.getMode() == SimulatorLogic::MODE_1)
    {
        cout << "--------------- MODE 1 ------------------";
    }
    else
    {
        cout << "--------------- MODE 2 ------------------";
    }

    y = y + 2;

    for(int i = 0; i < mode_commands.size(); i++)
    {
        Consola::gotoxy(x, y);
        cout << mode_commands[i]->toString() << endl;
        y++;
    }

    y++;
    Consola::gotoxy(x, y);
    string temp = "Introduce your command: ";
    cout << temp;
    printLastErrors(x, y + 2);
    last_error.clear();
    
    int input_position_x = x + temp.size();
    int input_position_y = y;

    Consola::gotoxy(input_position_x, input_position_y);
    cin >> ws;
    getline(cin, introduced_command);

    return introduced_command;
}

void UserInterface::printMap(int x, int y) const
{
    if(simulator_logic.raceInCourse() == false)
    {
        return;
    }

    printBox(x - 2, y - 2, 24, 60);

    Consola::gotoxy(x, y);
    cout << "--------------- MAP ------------------";

    y = y + 2;

    vector<string> race_informations = simulator_logic.getMap();

    int track_length = convertStringToInt(race_informations[0]);

    for(int i = 1; i < race_informations.size(); i++)
    {
        for(int j = 0; j < MAP_SCALE; j++)
        {
            Consola::setBackgroundColor(TRACK_BORDER_COLOR);
            Consola::gotoxy(x + j, y);
            cout << " ";
        }
        y++;

        istringstream informations_stream(race_informations[i]);

        char car_id;
        int car_distance;
        informations_stream >> car_id >> car_distance;

        Consola::setBackgroundColor(DEFAULT_BACKGROUND_COLOR);
        int position_in_screen = car_distance / (track_length / MAP_SCALE);
        Consola::gotoxy(x + position_in_screen, y);
        cout << car_id;
        y++;
    }

    for(int j = 0; j < MAP_SCALE; j++)
    {
        Consola::setBackgroundColor(TRACK_BORDER_COLOR);
        Consola::gotoxy(x + j, y);
        cout << " ";
    }

    Consola::setBackgroundColor(DEFAULT_BACKGROUND_COLOR);
}

void UserInterface::printLeaderboard(int x, int y) const
{
    if(simulator_logic.championshipInCourse() == true && simulator_logic.raceInCourse() == false)
    {
        printBox(x - 2, y - 2, 15, 60);

        Consola::gotoxy(x, y);
        cout << "--------------- LEADERBOARD ------------------";

        y = y + 2;

        vector<string> leaderboard = simulator_logic.getLeaderboard();

        for(int i = 0; i < leaderboard.size(); i++)
        {
            istringstream leaderboard_stream(leaderboard[i]);

            int position;
            int points;
            string pilot_name;

            leaderboard_stream >> position >> points;
            leaderboard_stream >> ws;
            getline(leaderboard_stream, pilot_name);

            Consola::gotoxy(x, y);
            cout << position << ". " << pilot_name << " : " << points << " points";
            y++;
        }
    }
}

void UserInterface::printRaceInformation(int x, int y) const
{
    if(simulator_logic.championshipInCourse() == true && simulator_logic.raceInCourse() == true)
    {
        printBox(x - 2, y - 2, 15, 60);

        Consola::gotoxy(x, y);
        cout << "--------------- RACE INFORMATION ------------------";

        y = y + 2;

        vector<string> race_information = simulator_logic.getRaceInformation();

        Consola::gotoxy(x, y);
        cout << "Racetrack: " << race_information[0];

        y = y + 2;

        for(int i = 1; i < race_information.size(); i++)
        {
            istringstream race_information_stream(race_information[i]);

            string car_brand;
            string car_id;
            string pilot_name;
            string pilot_type;
            float battery_consumption;
            float battery;
            int traveled_distance;
            int speed;

            race_information_stream >> car_id >> car_brand >> pilot_name >> pilot_type;
            race_information_stream >> battery >> battery_consumption >> traveled_distance >> speed;
            
            Consola::gotoxy(x, y);

            cout << i + 1 << ". " << car_id << " " << car_brand << " / " << pilot_name << " (" << pilot_type << ") - " << battery_consumption << " mAs, ";
            cout << battery << " mAh - " << traveled_distance << "m - " << speed << " m/s";

            y++;
        }
    }
}

void UserInterface::printLog(const vector<string> message_log, int x, int y) const
{
    printBox(x - 2, y - 2, 15, 60);

    Consola::gotoxy(x, y);
    cout << "--------------- LOG ------------------";

    y = y + 2;

    for(int i = 0; i < message_log.size(); i++)
    {
        Consola::gotoxy(x, y);
        cout << message_log[i];
        y++;
    }

    Consola::gotoxy(x, y);
    cout << "Prima ENTER para continuar";
    while(Consola::getch() != Consola::ENTER);
}
void UserInterface::printList(const string list, int x, int y) const
{
    istringstream list_stream(list);
    string temp;
    
    Consola::clrscr();
    printBox(x - 2, y - 2, 24, 60);

    Consola::gotoxy(x, y);
    cout << "--------------- LIST ------------------";

    y = y + 2;

    while(list_stream.eof() == false)
    {
        getline(list_stream, temp);
        Consola::gotoxy(x, y++);
        cout << temp;
    }

    Consola::gotoxy(x, ++y);
    cout << "Prima ENTER para continuar";
    while(Consola::getch() != Consola::ENTER){}
}

void UserInterface::printBox(int x, int y, int height, int width) const
{
    Consola::setBackgroundColor(BORDER_COLOR); 

    for(int i = 0; i < height + 1; i++)
    {
        Consola::gotoxy(x, y);
        cout << " ";
        Consola::gotoxy(x-1, y);
        cout << " ";
        Consola::gotoxy(x + width, y);
        cout << " ";
        Consola::gotoxy(x + width + 1, y);
        cout << " ";
        y++;
    }
    y = y - height - 1;
    for(int i = 0; i < width + 1; i++)
    {   
        Consola::gotoxy(x, y);
        cout << " ";
        Consola::gotoxy(x, y + height);
        cout << " ";
        x++;
    }

    Consola::setBackgroundColor(DEFAULT_BACKGROUND_COLOR);
}

void UserInterface::printLastErrors(int x, int y) const
{
    istringstream last_error_stream(last_error);
    string temp;

    while(last_error_stream.eof() == false)
    {
        Consola::gotoxy(x, y++);
        getline(last_error_stream, temp);
        cout << temp;
    }
}

vector<string> UserInterface::tokenizeCommand(const string &command) const
{
    vector<string> tokenized_command;

    istringstream input_stream(command);
    string temp;

    while(input_stream.eof() == false)
    {
        temp.clear();
        input_stream >> ws;
        input_stream >> temp;
        if(temp.size() > 0)
        {
            tokenized_command.push_back(temp);
        }
    }

    return tokenized_command;
}

Command::Command(string name, string arguments_description) : name(name), arguments_description(arguments_description) {}
Command::Command(string name) : name(name) {}

string Command::toString() const
{
    ostringstream output_stream;
    output_stream << name << " " << arguments_description;
    return output_stream.str();
}

string Command::getName() const { return name; }
int Command::getNumberOfArguments() const { return countWords(arguments_description); }

int Command::countWords(const string &command) const
{
    int i = 0;
    int contador = 0;

    while(command[i] == ' ' && i < command.size())
    {
        i++;
    }

    while(i < command.size())
    {
        while(command[i] != ' '  && i < command.size())
        {
            i++;
        }
        i++;
        contador++;
        while(command[i] == ' '  && i < command.size())
        {
            i++;
        }
    }

    return contador;
}

int UserInterface::convertStringToInt(string number) const
{
    istringstream number_stream(number);
    int int_value;

    number_stream >> int_value;

    return int_value;
}

bool UserInterface::isInteger(string number) const
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