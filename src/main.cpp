#include "vex.h"
using namespace vex;
brain Brain;
Drivetrain drive(PORT7, PORT12, PORT1, PORT8, PORT9);

int main()
{
    // Set grid dimensions FIRST
    drive.setGrid(3, 3);
    
    // Wait for IMU calibration from constructor
    wait(3, seconds);
    
    // Test movement
    
    // Initialize arrays
    int grid[3][3] = {};
    bool visit_Array[3][3] = {};
    int current_x_pos = 0;
    int current_y_pos = 0;
    
    // Start DFS exploration
    drive.dfs(grid, current_x_pos, current_y_pos, visit_Array);
    
    while (1)
    {
        this_thread::sleep_for(10);
    }
}
// This PID constants are good
// To see updates for the code MAKE SURE TO SAVE AND BUILD THE main.spp FILE
// drive.PIDmove(0.4, 0.000008, 0.01, 200);