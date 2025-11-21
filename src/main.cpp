#include "vex.h"
using namespace vex;
brain Brain;

int main()
{
    // // Initialize grid for DFS
    Drivetrain drive(PORT7,PORT12,PORT1,PORT6,PORT9);
    Pump PumpMotor(PORT10);
    int grid[3][3] = {0};
    bool visit_Array[3][3] = {false};
    int current_x = 0;
    int current_y = 0;

    // Run DFS to map the grid
    drive.dfs(grid, current_x, current_y, visit_Array);

    // Mapping variables
    bool finalcheck = true;
    bool verify[3][3] = {false};
    int movement[50] = {0};
    int dead[50] = {0};
    int going[50] = {0};
    int coming[50] = {0};
    int x_pos = 0;
    int y_pos = 0;
    int new_x = 0;
    int new_y = 0;
    int cur_x = 0;
    int cur_y = 0;
    int verifycnt = 0;
    int numcnt = 0;
    int wanted_x = 0;
    int wanted_y = 0;
    int secondcnt = 0;
    int finalcnt = 0;
    int r = 0;
    for (int i = 0; i < 50; i++)
    {
        movement[i] = 0;
        dead[i] = 0;
    }

    // Loop through all plant colors: 1=yellow, 2=green, 3=purple, 4=orange/red
    for (int color_to_find = 1; color_to_find <= 4; color_to_find++)
    {
        // Reset state variables for each plant
        finalcheck = true;
        numcnt = 0;
        x_pos = 0;
        y_pos = 0;
        new_x = 0;
        new_y = 0;
        cur_x = 0;
        cur_y = 0;
        verifycnt = 0;
        for (int i = 0; i < 50; i++)
        {
            movement[i] = 0;
            dead[i] = 0;
            going[i] = 0;
            coming[i] = 0;
        }
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                verify[i][j] = false;
            }
        }
        
        drive.index_finder(wanted_x, wanted_y, grid, color_to_find);

        if (wanted_x != -1 && wanted_y != -1)
        {
            Brain.Screen.clearScreen();
            Brain.Screen.printAt(10, 30, "Searching color: %d", color_to_find);
            Brain.Screen.printAt(10, 50, "Found at [%d][%d]", wanted_x, wanted_y);
            wait(2, seconds);

            drive.mapping(grid, numcnt, finalcheck, x_pos, y_pos, new_x, new_y, verify, verifycnt, cur_x, cur_y, movement, dead, wanted_x, wanted_y);

            secondcnt = 1;
            r = 0;
            for (int i = 0; i < 50; i++)
            {
                if (dead[i] == 1)
                {
                    r++;
                }
            }

            while (movement[secondcnt - 1] != 0)
            {
                Brain.Screen.printAt(10, 70 + secondcnt * 15, "Move: %d", movement[secondcnt - 1]);
                secondcnt++;
            }
            secondcnt--;

            finalcnt = secondcnt - r;

            // Create going path (without dead ends)
            int going_index = 0;
            for (int j = 0; j < secondcnt; j++)
            {
                if (dead[j] != 1)
                {
                    going[going_index] = movement[j];
                    going_index++;
                }
            }

            // Create return path (reversed and flipped directions)
            int index = 0;
            for (int i = finalcnt - 1; i >= 0; i--)
            {
                int direction = going[i];

                // Flip the direction: 1<->2, 3<->4
                if (direction == 1)
                {
                    coming[index] = 2; // right -> left
                }
                else if (direction == 2)
                {
                    coming[index] = 1; // left -> right
                }
                else if (direction == 3)
                {
                    coming[index] = 4; // down -> up
                }
                else if (direction == 4)
                {
                    coming[index] = 3; // up -> down
                }
                else
                {
                    coming[index] = 0;
                }
                index++;
            }

            Brain.Screen.clearScreen();
            Brain.Screen.printAt(10, 50, "Path calculated!");
            Brain.Screen.printAt(10, 70, "Steps: %d", finalcnt);
            wait(2, seconds);

            // Navigate to plant
            drive.GoToPos(going, finalcnt);

            // Water the plant
            int plant_color = grid[wanted_x][wanted_y];
            int water_time = drive.colourtotime(plant_color);
            Brain.Screen.clearScreen();
            Brain.Screen.printAt(10, 50, "Watering plant...");
            Brain.Screen.printAt(10, 70, "Color: %d Time: %d", plant_color, water_time);
            PumpMotor.PourWater(water_time);
            wait(1, seconds);

            // Return home
            Brain.Screen.clearScreen();
            Brain.Screen.printAt(10, 50, "Returning home...");
            wait(1, seconds);
            drive.GoToPos(coming, finalcnt);
            drive.PIDturn(0);
        }
        else
        {
            Brain.Screen.clearScreen();
            Brain.Screen.printAt(10, 50, "Color %d not found!", color_to_find);
            wait(1, seconds);
        }
    }
    
    Brain.Screen.clearScreen();
    Brain.Screen.printAt(10, 50, "All plants watered!");
    wait(2, seconds);
    while (1)
    {
        this_thread::sleep_for(10);
    }
}
// This PID constants are good
// To see updates for the code MAKE SURE TO SAVE AND BUILD THE main.spp FILE
// drive.PIDmove(0.4, 0.000008, 0.01, 200);