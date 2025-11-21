#include "pump.hpp"
Pump::Pump(char PumpMotor_port) : PumpMotor(PumpMotor_port, false)
{
    Brain.Screen.printAt(10, 50, "Pum23p!");
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
    int adjustment = 1;
    PumpMotor.setVelocity(100, percent);
    PumpMotor.spin(reverse);
    timeout.reset();
    while (timeout.time(sec) < (seconds * adjustment))
    {
        wait(50, msec);
    }
    // stop();
}