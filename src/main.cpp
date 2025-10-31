#include "vex.h"

using namespace vex;
vex::brain Brain;
Drivetrain drive(PORT1,PORT6);

int main() {
    // wait(1,seconds);
    Brain.programStop();
    while(1) {
        this_thread::sleep_for(10);
    }
}
// This PID constants are good
// drive.PIDmove(0.4, 0.000008, 0.01, 200);