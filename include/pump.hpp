#pragma once
#include "iq2_cpp.h"
using namespace vex;

class Pump
{
private:
    brain Brain;
    motor PumpMotor;
    timer timeout;

public:
    Pump(char PumpMotor_port);
    ~Pump();
    void stop();
    void PourWater(int seconds);
};