#include "pump.hpp"
Pump::Pump(char PumpMotor_port) : PumpMotor(PumpMotor_port, false)
{
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
    PumpMotor.spin(forward);
    timeout.reset();
    while (timeout.time() < seconds * adjustment)
    {
    }
    stop();
}