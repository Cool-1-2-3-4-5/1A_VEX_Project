#include "vex.h"
// #include "drivetrain.hpp"
using namespace vex;
brain Brain;
Drivetrain drive(PORT7,PORT12, PORT8, PORT9, PORT10);

int main() {
    // ELIL COMMITED
    // wait(1,seconds);
    // float angle = 0;
    // int x_grid = 5;
    // int y_grid = 5; 
    // int x_pos = 1;
    // int y_pos = 1;s
    // MY NAME IS BEN
    drive.IMUcalibrate();
    drive.PIDturn(90);  // Then turn
    drive.stop();
    drive.PIDturn(180);  // Then turn
    drive.stop();
    drive.PIDturn(270);  // Then turn
    drive.stop();
    drive.PIDturn(180);  // Then turn
    drive.stop();
    while(1) {
        this_thread::sleep_for(10);
    }
}
// This PID constants are good
// To see updates for the code MAKE SURE TO SAVE AND BUILD THE main.spp FILE
// drive.PIDmove(0.4, 0.000008, 0.01, 200);