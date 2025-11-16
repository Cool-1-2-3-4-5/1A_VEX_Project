#include "vex.h"
// #include "drivetrain.hpp"
using namespace vex;
brain Brain;
Drivetrain drive(PORT7,PORT12, PORT1, PORT6, PORT10);

int main() {
    int grid[4][4] = {};
    bool visit_Array[4][4] = {};
    int current_x_pos = 0;
    int current_y_pos = 0;
    
    // Set grid size
    drive.setGrid(4, 4);
    
    // Calibrate IMU
    drive.IMUcalibrate();
    
    // Display start position
    Brain.Screen.printAt(10, 50, "Start: [%d][%d]", current_x_pos, current_y_pos);
    wait(3, seconds);
    Brain.Screen.clearScreen();
    
    // Start DFS from position [0][0]
    drive.dfs(grid, current_x_pos, current_y_pos, visit_Array);
    
    while(1) {
        this_thread::sleep_for(10);
    }
}
// This PID constants are good
// To see updates for the code MAKE SURE TO SAVE AND BUILD THE main.spp FILE
// drive.PIDmove(0.4, 0.000008, 0.01, 200);