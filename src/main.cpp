#include "vex.h"
#include <cmath>
using namespace vex;
vex::brain Brain;
Drivetrain drive(PORT1,PORT6);

int main() {
    // wait(1,seconds);
    float angle = 0;
    int x_grid = 5;
    int y_grid = 5; 
    int x_pos = 1;
    int y_pos = 1;
    int plant = int[x_grid][y_grid];
    if (drive.bumpMove()){
        if(angle == 0){
            y_pos++;
            //append x_pos,y_pos to array
            y_pos--;
        }
        else if(angle == 90){
            x_pos++;
            //append x_pos,y_pos to array
            x_pos--;
        }
        else if(angle == 180){
            y_pos--;
            //append x_pos,y_pos to array
            y_pos++;
        }
        else{
            x_pos--;
            //append x_pos,y_pos to array
            x_pos++;
        }
    }
    Brain.programStop();
    while(1) {
        this_thread::sleep_for(10);
    }
}
// This PID constants are good
// drive.PIDmove(0.4, 0.000008, 0.01, 200);