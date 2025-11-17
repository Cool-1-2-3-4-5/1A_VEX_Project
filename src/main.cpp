#include "vex.h"
// #include "drivetrain.hpp"
using namespace vex;
brain Brain;
Drivetrain drive(PORT7,PORT12, PORT1,PORT8,PORT9);

int main() {

    int grid[3][3] = {};
    bool visit_Array[3][3] = {};
    int current_x_pos = 0;
    int current_y_pos = 0;
    int x_pos = 0;
    int y_pos = 0;
    int colour_num = 0;
    drive.dfs(grid,current_x_pos,current_y_pos,visit_Array);
    while(1) {
        this_thread::sleep_for(10);
    }
}
// This PID constants are good
// To see updates for the code MAKE SURE TO SAVE AND BUILD THE main.spp FILE
// drive.PIDmove(0.4, 0.000008, 0.01, 200);