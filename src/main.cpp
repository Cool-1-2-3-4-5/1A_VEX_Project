#include "vex.h"
// #include "drivetrain.hpp"
using namespace vex;
brain Brain;
<<<<<<< HEAD
Drivetrain drive(PORT7,PORT12);

int main() {
    // ELIL COMMITED
    // wait(1,seconds);
    // float angle = 0;
    // int x_grid = 5;
    // int y_grid = 5; 
    // int x_pos = 1;
    // int y_pos = 1;s
    // MY NAME IS BEN
    while(true){
    // bool plant_check = drive.bumpMove(200);
    drive.move();
    // while(1)
    // {}
    // drive.PIDmove(0.4, 0.000008, 0.01, 200);
    // Debug: Confirm bumpMove completed
    Brain.Screen.clearScreen();
    Brain.Screen.printAt(10, 50, "bumpMove completed!");
    Brain.Screen.printAt(10, 70, "Main function continuing...");
    wait(1, seconds);
    
    }
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
=======
Drivetrain drive(PORT7,PORT12, PORT1, PORT6, PORT10);

int main() {
    drive.move();
    // int grid[3][3] = {};
    // bool visit_Array[3][3] = {};
    // int current_x_pos = 0;
    // int current_y_pos = 0;
    // // Set grid size
    // drive.setGrid(3,3);
    
    // // Calibrate IMU
    // drive.IMUcalibrate();
    
    // // Display start position
    // Brain.Screen.printAt(10, 50, "Start: [%d][%d]", current_x_pos, current_y_pos);
    // wait(3, seconds);
    // Brain.Screen.clearScreen();
    
    // // Start DFS from position [0][0]
    // drive.dfs(grid, current_x_pos, current_y_pos, visit_Array);
    // drive.PIDturn(0);
>>>>>>> e5ec1bed03a2e78864d0e1182c6efcdfdacdda18
    while(1) {
        this_thread::sleep_for(10);
    }
}
// This PID constants are good
// To see updates for the code MAKE SURE TO SAVE AND BUILD THE main.spp FILE
// drive.PIDmove(0.4, 0.000008, 0.01, 200);