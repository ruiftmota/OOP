#include "SimulatorLogic.h"
#include "UserInterface.h"

int main(void)
{
    SimulatorLogic simulator_logic;
    UserInterface user_interface(simulator_logic);

    user_interface.run();

    return 0;
}