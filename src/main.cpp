#include "vex.h"
// #include "drivetrain.hpp"
using namespace vex;
brain Brain;
Drivetrain drive(PORT7,PORT12, PORT1, PORT6, PORT10);

int main() {
    // int grid[4][4] = {};
    // bool visit_Array[4][4] = {};
    // int posible_movement[4][2] = {};
    // int current_x_pos = 0;
    // int current_y_pos = 0;
    // ELIL COMMITED
    // wait(1,seconds);
    // float angle = 0;
    // int x_grid = 5;
    // int y_grid = 5; 
    // int x_pos = 1;
    // int y_pos = 1;s
    // MY NAME IS BEN
    // drive.IMUcalibrate();
    drive.PIDmove(200);
    // drive.moveToPlant();
    // drive.PIDturn(10);  // Then turn
    // drive.stop();
    // drive.PIDturn(350);  // Then turn
    // drive.stop();
    // drive.PIDturn(270);  // Then turn
    // drive.stop();s
    // drive.PIDturn(180);  // Then turn
    // drive.stop();
    // drive.dfs(grid, current_x_pos, current_y_pos, visit_Array, posible_movement);
    while(1) {
        this_thread::sleep_for(10);
    }
}
// This PID constants are good
// To see updates for the code MAKE SURE TO SAVE AND BUILD THE main.spp FILE
// drive.PIDmove(0.4, 0.000008, 0.01, 200);