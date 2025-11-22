#include "pump.hpp"
Pump::Pump(char PumpMotor_port) : PumpMotor(PumpMotor_port, false)
{
    Brain.Screen.printAt(10, 50, "Pump Initialized!");
    wait(1, seconds);
    PumpMotor.setStopping(brakeType::hold);
    PumpMotor.setVelocity(0, percent);
}
Pump::~Pump()
{
}
void Pump::stop()
{
    PumpMotor.stop();
}
void Pump::PourWater(int seconds)
{
    const int adjustment = 3;
    PumpMotor.setVelocity(100, percent);
    PumpMotor.spin(reverse);
    timeout.reset();
    while (timeout.time(sec) < (seconds * adjustment))
    {
    }
    stop();
    Brain.Screen.printAt(10, 50, "Watered!");
    wait(1,sec);
    Brain.Screen.clearScreen();
}