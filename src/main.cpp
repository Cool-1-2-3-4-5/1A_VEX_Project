#include "vex.h"
// #include <cmath>
using namespace vex;
vex::brain Brain;
Drivetrain drive(PORT1,PORT6,PORT7);

int main() {
    // wait(1,seconds);
    // float angle = 0;
    // int x_grid = 5;
    // int y_grid = 5; 
    // int x_pos = 1;
    // int y_pos = 1;
    // drive.PIDmove(0.4, 0.000008, 0.01, 200);
    bool plant_check = drive.bumpMove(200);
    
    // Debug: Confirm bumpMove completed
    Brain.Screen.clearScreen();
    Brain.Screen.printAt(10, 50, "bumpMove completed!");
    Brain.Screen.printAt(10, 70, "Main function continuing...");
    wait(3, seconds);
    // if (x_pos==0){
    //     if(angle == 0){
    //         y_pos++;
    //         //append x_pos,y_pos to array
    //         y_pos--;
    //     }
    //     else if(angle == 90){
    //         x_pos++;
    //         //append x_pos,y_pos to array
    //         x_pos--;
    //     }
    //     else if(angle == 180){
    //         y_pos--;
    //         //append x_pos,y_pos to array
    //         y_pos++;
    //     }
    //     else{
    //         x_pos--;
    //         //append x_pos,y_pos to array
    //         x_pos++;
    //     }
    // }
    // Brain.programStop();
    while(1) {
        this_thread::sleep_for(10);
    }
}
// This PID constants are good
// To see updates for the code MAKE SURE TO SAVE AND BUILD THE main.spp FILE
// drive.PIDmove(0.4, 0.000008, 0.01, 200);