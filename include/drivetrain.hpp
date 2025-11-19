#pragma once
#include "iq2_cpp.h"
// #include <string>
using namespace vex;

class Drivetrain
{
private:
    motor left_;
    motor right_;
    inertial BrainInertial;
    brain Brain;
    distance DistanceSensor;
    colorsensor ColourSensor;
    touchled TouchSensor;
    motor PumpMotor;
    timer timeout;
    int grid_rows;
    int grid_cols;

public:
    Drivetrain(char left_Port, char right_Port, char distanceSensor_port, char colourSensor_port, char touchSensor_port);
    ~Drivetrain();
    void setGrid(int x, int y);
    void IMUcalibrate();
    void stop();
    void PIDmove(float distance, float kp = 0.3, float ki = 0.000016, float kd = 0.01);
    void PIDturn(float angle, float kp = 0.4, float ki = 0.00000035, float kd = 0.01);
    bool checkForPlant();
    int moveToPlant();
    int colourtotime(int colourValue);
    void pourwater(int time);
    void move();
    void touchandgo();
    void dfs(int grid[][3], int &current_x_pos, int &current_y_pos, bool visit_Array[][3]);
    void index_finder(int &x_pos, int &y_pos, int grid[][3], int colour_num);
    void BFS(int grid[][3], int colour_x, int colour_y);
};

/*
// safety2:

#pragma once
#include "iq2_cpp.h"
// #include <string>
using namespace vex;

class Drivetrain
{
private:
    motor left_;
    motor right_;
    inertial BrainInertial;
    brain Brain;
    distance DistanceSensor;
    colorsensor ColourSensor;
    touchled TouchSensor;
    motor_group PumpMotor; // Check this is right
    timer timeout;
    int grid_rows = 0;
    int grid_cols = 0;

public:
    Drivetrain(char left_Port, char right_Port, char distanceSensor_port, char colourSensor_port, char touchSensor_port) : left_(left_Port, false), right_(right_Port, true),DistanceSensor(distanceSensor_port),ColourSensor(colourSensor_port), TouchSensor(touchSensor_port)
    {
        left_.setStopping(brakeType::hold);
        right_.setStopping(brakeType::hold);
        left_.setVelocity(0, percent);
        right_.setVelocity(0, percent);
        BrainInertial.calibrate();
        ColourSensor.brightness(100);
    }
    void setGrid(int x = 3, int y = 3)
    {
        grid_rows = x;
        grid_cols = y;
    }
    void IMUcalibrate()
    {
        BrainInertial.calibrate();
        BrainInertial.setRotation(0, degrees);
        BrainInertial.setHeading(0, degrees);
        wait(3,seconds);
    }
    void stop()
    {
        left_.stop();
        right_.stop();
    }

    void move()
    {
        right_.setVelocity(100,percent);
        left_.setVelocity(100,percent);
        left_.spin(forward);
        right_.spin(forward);
    }
    void PIDmove(float distance, float kp = 0.3, float ki =  0.000016, float kd = 0.01)
    {
        left_.setPosition(0, degrees);
        right_.setPosition(0, degrees);
        float average_dist = 0;
        float derivate_difference;
        float cumerror = 0;
        bool check = true;
        float error = 0;
        float speed;
        float prev_error = 0;
        timeout.reset();
        left_.spin(forward);
        right_.spin(forward);
        while (check)
        {
            average_dist = ((left_.position(degrees) + right_.position(degrees)) / 2) * 200.0/360;
            Brain.Screen.printAt(10, 50, "dist: %.1f", error);
            // potential
            error = distance - average_dist;
            // integral
            cumerror += error;
            // derivative
            derivate_difference = error - prev_error;
            speed = kp * (error) + ki * cumerror + kd * derivate_difference;
            left_.setVelocity(speed, percent);
            right_.setVelocity(speed, percent);
            prev_error = error;
            if (timeout.time(sec) > 3.0)
            {
                check = false;
            }
        }
        left_.stop();
        right_.stop();
    }
    void PIDturn(float angle, float kp = 0.4, float ki = 0.00000035, float kd = 0.01)
    {
        float average_dist = 0;
        float derivate_difference;
        float cumerror = 0;
        bool check = true;
        float error;
        float speed;
        float prev_error = 0;
        timeout.reset();
        left_.spin(forward);
        right_.spin(forward);
        if(angle > 180){
            angle = angle -360;
        }
        while (check)
        {
            // potential
            error = angle - BrainInertial.rotation();
            // integral
            cumerror += error;
            // derivative
            derivate_difference = error - prev_error;
            speed = kp * (error) + ki * cumerror + kd * derivate_difference;
            left_.setVelocity(1 * speed, percent);
            right_.setVelocity(-1 * speed, percent);
            prev_error = error;
            if (timeout.time(sec) > 5.0)
            {
                check = false;
            }
        }
        left_.stop();
        right_.stop();
    }
    bool checkForPlant()
    {

        Brain.Screen.printAt(10, 50, "DisPlant: %.1f", DistanceSensor.objectDistance(mm));
        wait(1,seconds);
        if (DistanceSensor.objectDistance(mm) < 200)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    int moveToPlant()
    {
        int colourVal = 0;
        float distance_initial = 0;
        while(timeout.time(sec) < 5.0){
            Brain.Screen.printAt(10, 50, "RUN: %.1f", DistanceSensor.objectDistance(mm));
        }
        distance_initial = DistanceSensor.objectDistance(mm);
        distance_initial =  distance_initial - 50.0;
        Brain.Screen.printAt(10, 50, "final: %.1f", distance_initial);
        timeout.reset();
        PIDmove(distance_initial);
        if (ColourSensor.color() == cyan)
        {
            colourVal = 1;
        }
        else if (ColourSensor.color() == green)
        {
            colourVal = 2;
        }
        else if (ColourSensor.color() == yellow)
        {
            colourVal = 3;
        }
        else if (ColourSensor.color() == violet)
        {
            colourVal = 4;
        }
        wait(1,seconds);
        PIDmove(-1*distance_initial);
        wait(1,seconds);
        return colourVal;
    }
    int colourtotime (int colourValue)
    int timetowater = 0;
    {
        if (colourValue == 1)
        {
            time = 3;
        }
        if (colourValue == 2)
        {
            time = 6;
        }
        if (colourValue == 3)
        {
            time = 9;
        }
        if (colourValue == 4)
        {
            time = 11;
        }
        return time;
    }
    void pourwater (int time)
    {
        int adjustment = 1 // change adjustment upon testing
        timeout.reset()
        PumpMotor.spin(forwards)
        while (timeout.value() < time*adjustment)
        {}
        PumpMotor.stop()
    }

    void move()
    {
<<<<<<< HEAD
        right_.setVelocity(100,percent);
        right_.spin(forward);
    }
    void touchandgo ()
    {
        while (TouchSensor.pressing() = 0)
        {}
        while (TouchSensor.pressing() = 1)
        {}
    }
=======
       //
<<<<<<< HEAD

>>>>>>> 361e9c9384259890134860ab4083af20e295f836
=======
>>>>>>> ea23f0a5f3235c1653658adda22c825d054cea7a
=======
>>>>>>> bbb57b28618af9904f729a0c16d66e5cb75b73f5
    }
    // void touchAndGo()
    // {
    //     while (!TouchSensor.pressing())
    //     {
    //     }
    //     while (TouchSensor.pressing())
    //     {
    //     }
    //     int distance_initial = 0;
    //     int distance_final = 0;
    //     int distance_togo = 0;
    //     string color_ind = " ";
    //     int colour_ind = 0;
    //     distance_initial = DistanceSensor.objectDistance(mm);
    //     Left_.setPosition(0, turns);
    //     Left_.setVelocity(10, percent);
    //     Right_.setVelocity(10, percent);
    //     Left_.spin(forwards);
    //     Right_.spin(forwards);
    //     while (DistanceSensor(mm) < 30;
    //     {}
    //     Left_.stop();
    //     Right_.stop();
    //     distance_final = DistanceSensor.objectDistance(mm);
    //     distance_togo = distance_initial - distance_final;
    //     if (colour
    // }

    void dfs(int grid[][3], int current_x_pos, int current_y_pos, bool visit_Array[][3])
    {
        // Check if current position is out of bounds or already visited
        if (current_x_pos < 0 || current_x_pos >= grid_rows || current_y_pos < 0 || current_y_pos >= grid_cols || visit_Array[current_x_pos][current_y_pos])
        {
            return;
        }

        // Mark current position as visited
        visit_Array[current_x_pos][current_y_pos] = true;

        // Define directions: Up, Right, Down, Left
        // [0][0] is top-left corner
        // Up: y-1, Right: x+1, Down: y+1, Left: x-1
        int directions_change[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
        int posible_movement[4][2] = {};
        int cnt = 0;

        // Check all 4 directions
        for (int i = 0; i < 4; i++)
        {
            int new_pos_x = current_x_pos + directions_change[i][0];
            int new_pos_y = current_y_pos + directions_change[i][1];

            Brain.Screen.clearScreen();
            Brain.Screen.printAt(10, 30, "Checking dir %d", i);
            Brain.Screen.printAt(10, 50, "From [%d][%d]", current_x_pos, current_y_pos);
            Brain.Screen.printAt(10, 70, "To [%d][%d]", new_pos_x, new_pos_y);
            wait(2, seconds);

            // Check if position is valid before turning and checking
            if (new_pos_x < 0 || new_pos_x >= grid_rows || new_pos_y < 0 || new_pos_y >= grid_cols)
            {
                Brain.Screen.clearScreen();
                Brain.Screen.printAt(10, 50, "Out of bounds:");
                Brain.Screen.printAt(10, 70, "[%d][%d]", new_pos_x, new_pos_y);
                wait(2, seconds);
                continue; // Skip this direction
            }

            if (visit_Array[new_pos_x][new_pos_y])
            {
                Brain.Screen.clearScreen();
                Brain.Screen.printAt(10, 50, "Already visited:");
                Brain.Screen.printAt(10, 70, "[%d][%d]", new_pos_x, new_pos_y);
                wait(2, seconds);
                continue; // Skip this direction
            }

            // Turn to face direction i (0=up, 1=right, 2=down, 3=left)
            PIDturn(90 * i);
            wait(0.5, seconds);

            // Check if there's a plant in this direction
            if (checkForPlant())
            {
                // Plant detected - move to it, get color, move back
                int colour = moveToPlant();

                // Store color in grid if position is valid
                if (new_pos_x >= 0 && new_pos_x < grid_rows && new_pos_y >= 0 && new_pos_y < grid_cols)
                {
                    grid[new_pos_x][new_pos_y] = colour;
                    visit_Array[new_pos_x][new_pos_y] = true;
                    Brain.Screen.printAt(10, 70, "Plant at [%d][%d]: %d", new_pos_x, new_pos_y, colour);
                    wait(1, seconds);
                    Brain.Screen.clearScreen();
                }
            }
            else
            {
                // No plant - this is an empty cell, add to possible movements
                posible_movement[cnt][0] = new_pos_x;
                posible_movement[cnt][1] = new_pos_y;
                Brain.Screen.clearScreen();
                Brain.Screen.printAt(10, 50, "Possible move %d:", cnt);
                Brain.Screen.printAt(10, 70, "[%d][%d]", new_pos_x, new_pos_y);
                wait(2, seconds);
                cnt++;
            }
        }

        // Now visit all possible empty cells (DFS recursion)
        for (int i = 0; i < cnt; i++)
        {
            int next_cell_x = posible_movement[i][0];
            int next_cell_y = posible_movement[i][1];

            // Calculate which direction to turn to face the next cell
            int dx = next_cell_x - current_x_pos;
            int dy = next_cell_y - current_y_pos;
            int direction = 0;
            if (dy == -1) direction = 0;      // Up
            else if (dx == 1) direction = 1;  // Right
            else if (dy == 1) direction = 2;  // Down
            else if (dx == -1) direction = 3; // Left

            // Turn to face the direction and move to next cell
            PIDturn(90 * direction);
            wait(0.5, seconds);
            PIDmove(300); // Move 100mm to next cell
            wait(0.5, seconds);

            // Recursively explore from new position
            dfs(grid, next_cell_x, next_cell_y, visit_Array);

            // Backtrack: turn around, move back, turn around again
            Brain.Screen.clearScreen();
            Brain.Screen.printAt(10, 50, "DEADEND %d:", cnt);
            Brain.Screen.printAt(10, 70, "[%d][%d]", dx, dy);
            wait(1,seconds);
            Brain.Screen.clearScreen();
            PIDturn(90 * ((direction + 2) % 4)); // Turn 180 degrees
            wait(0.5, seconds);
            PIDmove(300); // Move back
            wait(0.5, seconds);
            PIDturn(90 * direction); // Face original direction
            wait(0.5, seconds);
        }
        Brain.Screen.clearScreen();
        Brain.Screen.printAt(10, 50, "DFS DONE!!!");
    }
};

// void PIDturn2(float angle, float kp = 0.4, float ki = 0.000026, float kd = 0.01)
//     {
//         float average_dist = 0;
//         float derivate_difference;
//         float cumerror = 0;
//         bool check = true;
//         float error;
//         float speed;
//         float prev_error = 0;
//         timeout.reset();
//         left_.spin(forward);
//         right_.spin(forward);
//         int changer = 1;
//         int convert = 1;
//         // if(angle > 180){
//         //     angle -= 360;
//         // }
//         if(BrainInertial.rotation() < 0){
//             convert = 1;
//         }
//         if ((BrainInertial.rotation() - angle) < ((BrainInertial.rotation()) + angle))
//         {
//             changer = -1;
//         }
//         while (check)
//         {
//             // potential
//             error = angle - (convert * BrainInertial.rotation());
//             // integral
//             cumerror += error;
//             // derivative
//             derivate_difference = error - prev_error;
//             speed = kp * (error) + ki * cumerror + kd * derivate_difference;
//             left_.setVelocity(-1 * speed * changer, percent);
//             right_.setVelocity(speed * changer, percent);
//             prev_error = error;
//             if (timeout.time(sec) > 4.0)
//             {
//                 check = false;
//             }
//             Brain.Screen.printAt(10, 50, "Rotation: %.1f", BrainInertial.rotation());
//             Brain.Screen.printAt(10, 70, "Target: %.1f", angle);
//             Brain.Screen.printAt(10, 90, "Error: %.1f", error);
//         }
//         left_.stop();
//         right_.stop();
//     }


















/*
#pragma once
#include "iq2_cpp.h"
// #include <string>
using namespace vex;

class Drivetrain
{
private:
    motor left_;
    motor right_;
    inertial BrainInertial;
    brain Brain;
    distance DistanceSensor;
    colorsensor ColourSensor;
    touchled TouchSensor;
    motor_group PumpMotor; // Check this is right
    timer timeout;
    int grid_rows = 0;
    int grid_cols = 0;

public:
    Drivetrain(char left_Port, char right_Port, char distanceSensor_port, char colourSensor_port, char touchSensor_port) : left_(left_Port, false), right_(right_Port, true),DistanceSensor(distanceSensor_port),ColourSensor(colourSensor_port), TouchSensor(touchSensor_port)
    {
        left_.setStopping(brakeType::hold);
        right_.setStopping(brakeType::hold);
        left_.setVelocity(0, percent);
        right_.setVelocity(0, percent);
        BrainInertial.calibrate();
        ColourSensor.brightness(100);
    }
    void setGrid(int x = 3, int y = 3)
    {
        grid_rows = x;
        grid_cols = y;
    }
    void IMUcalibrate()
    {
        BrainInertial.calibrate();
        BrainInertial.setRotation(0, degrees);
        BrainInertial.setHeading(0, degrees);
        wait(3,seconds);
    }
    void stop()
    {
        left_.stop();
        right_.stop();
    }

    void move()
    {
        right_.setVelocity(100,percent);
        left_.setVelocity(100,percent);
        left_.spin(forward);
        right_.spin(forward);
    }
    void PIDmove(float distance, float kp = 0.3, float ki =  0.000016, float kd = 0.01)
    {
        left_.setPosition(0, degrees);
        right_.setPosition(0, degrees);
        float average_dist = 0;
        float derivate_difference;
        float cumerror = 0;
        bool check = true;
        float error = 0;
        float speed;
        float prev_error = 0;
        timeout.reset();
        left_.spin(forward);
        right_.spin(forward);
        while (check)
        {
            average_dist = ((left_.position(degrees) + right_.position(degrees)) / 2) * 200.0/360;
            Brain.Screen.printAt(10, 50, "dist: %.1f", error);
            // potential
            error = distance - average_dist;
            // integral
            cumerror += error;
            // derivative
            derivate_difference = error - prev_error;
            speed = kp * (error) + ki * cumerror + kd * derivate_difference;
            left_.setVelocity(speed, percent);
            right_.setVelocity(speed, percent);
            prev_error = error;
            if (timeout.time(sec) > 3.0)
            {
                check = false;
            }
        }
        left_.stop();
        right_.stop();
    }
    void PIDturn(float angle, float kp = 0.4, float ki = 0.00000035, float kd = 0.01)
    {
        float average_dist = 0;
        float derivate_difference;
        float cumerror = 0;
        bool check = true;
        float error;
        float speed;
        float prev_error = 0;
        timeout.reset();
        left_.spin(forward);
        right_.spin(forward);
        if(angle > 180){
            angle = angle -360;
        }
        while (check)
        {
            // potential
            error = angle - BrainInertial.rotation();
            // integral
            cumerror += error;
            // derivative
            derivate_difference = error - prev_error;
            speed = kp * (error) + ki * cumerror + kd * derivate_difference;
            left_.setVelocity(1 * speed, percent);
            right_.setVelocity(-1 * speed, percent);
            prev_error = error;
            if (timeout.time(sec) > 5.0)
            {
                check = false;
            }
        }
        left_.stop();
        right_.stop();
    }
    bool checkForPlant()
    {

        Brain.Screen.printAt(10, 50, "DisPlant: %.1f", DistanceSensor.objectDistance(mm));
        wait(1,seconds);
        if (DistanceSensor.objectDistance(mm) < 200)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    int moveToPlant()
    {
        int colourVal = 0;
        float distance_initial = 0;
        while(timeout.time(sec) < 5.0){
            Brain.Screen.printAt(10, 50, "RUN: %.1f", DistanceSensor.objectDistance(mm));
        }
        distance_initial = DistanceSensor.objectDistance(mm);
        distance_initial =  distance_initial - 50.0;
        Brain.Screen.printAt(10, 50, "final: %.1f", distance_initial);
        timeout.reset();
        PIDmove(distance_initial);
        if (ColourSensor.color() == cyan)
        {
            colourVal = 1;
        }
        else if (ColourSensor.color() == green)
        {
            colourVal = 2;
        }
        else if (ColourSensor.color() == yellow)
        {
            colourVal = 3;
        }
        else if (ColourSensor.color() == violet)
        {
            colourVal = 4;
        }
        wait(1,seconds);
        PIDmove(-1*distance_initial);
        wait(1,seconds);
        return colourVal;
<<<<<<< HEAD
     }
    int colourtotime (int colourValue)
    int timetowater = 0;
    {
        if (colourValue == 1)
        {
            time = 3;
        }
        if (colourValue == 2)
        {
            time = 6;
        }
        if (colourValue == 3)
        {
            time = 9;
        }
        if (colourValue == 4)
        {
            time = 11;
        }
        return time;
    }
    void pourwater (int time)
    {
        int adjustment = 1 // change adjustment upon testing
        timeout.reset()
        PumpMotor.spin(forwards)
        while (timeout.value() < time*adjustment)
        {}
        PumpMotor.stop()
    }

    void move()
    {
<<<<<<< HEAD
        right_.setVelocity(100,percent);
        right_.spin(forward);
    }
    void touchandgo ()
    {
        while (TouchSensor.pressing() = 0)
        {}
        while (TouchSensor.pressing() = 1)
        {}
    }
=======
       //
<<<<<<< HEAD

>>>>>>> 361e9c9384259890134860ab4083af20e295f836
=======
>>>>>>> ea23f0a5f3235c1653658adda22c825d054cea7a
=======
>>>>>>> bbb57b28618af9904f729a0c16d66e5cb75b73f5
    }
    // void touchAndGo()
    // {
    //     while (!TouchSensor.pressing())
    //     {
    //     }
    //     while (TouchSensor.pressing())
    //     {
    //     }
    //     int distance_initial = 0;
    //     int distance_final = 0;
    //     int distance_togo = 0;
    //     string color_ind = " ";
    //     int colour_ind = 0;
    //     distance_initial = DistanceSensor.objectDistance(mm);
    //     Left_.setPosition(0, turns);
    //     Left_.setVelocity(10, percent);
    //     Right_.setVelocity(10, percent);
    //     Left_.spin(forwards);
    //     Right_.spin(forwards);
    //     while (DistanceSensor(mm) < 30;
    //     {}
    //     Left_.stop();
    //     Right_.stop();
    //     distance_final = DistanceSensor.objectDistance(mm);
    //     distance_togo = distance_initial - distance_final;
    //     if (colour
    // }

    void dfs(int grid[][3], int current_x_pos, int current_y_pos, bool visit_Array[][3])
    {
        // Check if current position is out of bounds or already visited
        if (current_x_pos < 0 || current_x_pos >= grid_rows || current_y_pos < 0 || current_y_pos >= grid_cols || visit_Array[current_x_pos][current_y_pos])
        {
            return;
        }

        // Mark current position as visited
        visit_Array[current_x_pos][current_y_pos] = true;

        // Define directions: Up, Right, Down, Left
        // [0][0] is top-left corner
        // Up: y-1, Right: x+1, Down: y+1, Left: x-1
        int directions_change[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
        int posible_movement[4][2] = {};
        int cnt = 0;

        // Check all 4 directions
        for (int i = 0; i < 4; i++)
        {
            int new_pos_x = current_x_pos + directions_change[i][0];
            int new_pos_y = current_y_pos + directions_change[i][1];

            Brain.Screen.clearScreen();
            Brain.Screen.printAt(10, 30, "Checking dir %d", i);
            Brain.Screen.printAt(10, 50, "From [%d][%d]", current_x_pos, current_y_pos);
            Brain.Screen.printAt(10, 70, "To [%d][%d]", new_pos_x, new_pos_y);
            wait(2, seconds);

            // Check if position is valid before turning and checking
            if (new_pos_x < 0 || new_pos_x >= grid_rows || new_pos_y < 0 || new_pos_y >= grid_cols)
            {
                Brain.Screen.clearScreen();
                Brain.Screen.printAt(10, 50, "Out of bounds:");
                Brain.Screen.printAt(10, 70, "[%d][%d]", new_pos_x, new_pos_y);
                wait(2, seconds);
                continue; // Skip this direction
            }

            if (visit_Array[new_pos_x][new_pos_y])
            {
                Brain.Screen.clearScreen();
                Brain.Screen.printAt(10, 50, "Already visited:");
                Brain.Screen.printAt(10, 70, "[%d][%d]", new_pos_x, new_pos_y);
                wait(2, seconds);
                continue; // Skip this direction
            }

            // Turn to face direction i (0=up, 1=right, 2=down, 3=left)
            PIDturn(90 * i);
            wait(0.5, seconds);

            // Check if there's a plant in this direction
            if (checkForPlant())
            {
                // Plant detected - move to it, get color, move back
                int colour = moveToPlant();

                // Store color in grid if position is valid
                if (new_pos_x >= 0 && new_pos_x < grid_rows && new_pos_y >= 0 && new_pos_y < grid_cols)
                {
                    grid[new_pos_x][new_pos_y] = colour;
                    visit_Array[new_pos_x][new_pos_y] = true;
                    Brain.Screen.printAt(10, 70, "Plant at [%d][%d]: %d", new_pos_x, new_pos_y, colour);
                    wait(1, seconds);
                    Brain.Screen.clearScreen();
                }
            }
            else
            {
                // No plant - this is an empty cell, add to possible movements
                posible_movement[cnt][0] = new_pos_x;
                posible_movement[cnt][1] = new_pos_y;
                Brain.Screen.clearScreen();
                Brain.Screen.printAt(10, 50, "Possible move %d:", cnt);
                Brain.Screen.printAt(10, 70, "[%d][%d]", new_pos_x, new_pos_y);
                wait(2, seconds);
                cnt++;
            }
        }

        // Now visit all possible empty cells (DFS recursion)
        for (int i = 0; i < cnt; i++)
        {
            int next_cell_x = posible_movement[i][0];
            int next_cell_y = posible_movement[i][1];

            // Calculate which direction to turn to face the next cell
            int dx = next_cell_x - current_x_pos;
            int dy = next_cell_y - current_y_pos;
            int direction = 0;
            if (dy == -1) direction = 0;      // Up
            else if (dx == 1) direction = 1;  // Right
            else if (dy == 1) direction = 2;  // Down
            else if (dx == -1) direction = 3; // Left

            // Turn to face the direction and move to next cell
            PIDturn(90 * direction);
            wait(0.5, seconds);
            PIDmove(300); // Move 100mm to next cell
            wait(0.5, seconds);

            // Recursively explore from new position
            dfs(grid, next_cell_x, next_cell_y, visit_Array);

            // Backtrack: turn around, move back, turn around again
            Brain.Screen.clearScreen();
            Brain.Screen.printAt(10, 50, "DEADEND %d:", cnt);
            Brain.Screen.printAt(10, 70, "[%d][%d]", dx, dy);
            wait(1,seconds);
            Brain.Screen.clearScreen();
            PIDturn(90 * ((direction + 2) % 4)); // Turn 180 degrees
            wait(0.5, seconds);
            PIDmove(300); // Move back
            wait(0.5, seconds);
            PIDturn(90 * direction); // Face original direction
            wait(0.5, seconds);
        }
        Brain.Screen.clearScreen();
        Brain.Screen.printAt(10, 50, "DFS DONE!!!");
    }
};

// void PIDturn2(float angle, float kp = 0.4, float ki = 0.000026, float kd = 0.01)
//     {
//         float average_dist = 0;
//         float derivate_difference;
//         float cumerror = 0;
//         bool check = true;
//         float error;
//         float speed;
//         float prev_error = 0;
//         timeout.reset();
//         left_.spin(forward);
//         right_.spin(forward);
//         int changer = 1;
//         int convert = 1;
//         // if(angle > 180){
//         //     angle -= 360;
//         // }
//         if(BrainInertial.rotation() < 0){
//             convert = 1;
//         }
//         if ((BrainInertial.rotation() - angle) < ((BrainInertial.rotation()) + angle))
//         {
//             changer = -1;
//         }
//         while (check)
//         {
//             // potential
//             error = angle - (convert * BrainInertial.rotation());
//             // integral
//             cumerror += error;
//             // derivative
//             derivate_difference = error - prev_error;
//             speed = kp * (error) + ki * cumerror + kd * derivate_difference;
//             left_.setVelocity(-1 * speed * changer, percent);
//             right_.setVelocity(speed * changer, percent);
//             prev_error = error;
//             if (timeout.time(sec) > 4.0)
//             {
//                 check = false;
//             }
//             Brain.Screen.printAt(10, 50, "Rotation: %.1f", BrainInertial.rotation());
//             Brain.Screen.printAt(10, 70, "Target: %.1f", angle);
//             Brain.Screen.printAt(10, 90, "Error: %.1f", error);
//         }
//         left_.stop();
//         right_.stop();
//     }


// safety2:

#pragma once
#include "iq2_cpp.h"
// #include <string>
using namespace vex;

class Drivetrain
{
private:
    motor left_;
    motor right_;
    inertial BrainInertial;
    brain Brain;
    distance DistanceSensor;
    colorsensor ColourSensor;
    touchled TouchSensor;
    motor_group PumpMotor; // Check this is right
    timer timeout;
    int grid_rows = 0;
    int grid_cols = 0;

public:
    Drivetrain(char left_Port, char right_Port, char distanceSensor_port, char colourSensor_port, char touchSensor_port) : left_(left_Port, false), right_(right_Port, true),DistanceSensor(distanceSensor_port),ColourSensor(colourSensor_port), TouchSensor(touchSensor_port)
    {
        left_.setStopping(brakeType::hold);
        right_.setStopping(brakeType::hold);
        left_.setVelocity(0, percent);
        right_.setVelocity(0, percent);
        BrainInertial.calibrate();
        ColourSensor.brightness(100);
    }
    void setGrid(int x = 3, int y = 3)
    {
        grid_rows = x;
        grid_cols = y;
    }
    void IMUcalibrate()
    {
        BrainInertial.calibrate();
        BrainInertial.setRotation(0, degrees);
        BrainInertial.setHeading(0, degrees);
        wait(3,seconds);
    }
    void stop()
    {
        left_.stop();
        right_.stop();
    }

    void move()
    {
        right_.setVelocity(100,percent);
        left_.setVelocity(100,percent);
        left_.spin(forward);
        right_.spin(forward);
    }
    void PIDmove(float distance, float kp = 0.3, float ki =  0.000016, float kd = 0.01)
    {
        left_.setPosition(0, degrees);
        right_.setPosition(0, degrees);
        float average_dist = 0;
        float derivate_difference;
        float cumerror = 0;
        bool check = true;
        float error = 0;
        float speed;
        float prev_error = 0;
        timeout.reset();
        left_.spin(forward);
        right_.spin(forward);
        while (check)
        {
            average_dist = ((left_.position(degrees) + right_.position(degrees)) / 2) * 200.0/360;
            Brain.Screen.printAt(10, 50, "dist: %.1f", error);
            // potential
            error = distance - average_dist;
            // integral
            cumerror += error;
            // derivative
            derivate_difference = error - prev_error;
            speed = kp * (error) + ki * cumerror + kd * derivate_difference;
            left_.setVelocity(speed, percent);
            right_.setVelocity(speed, percent);
            prev_error = error;
            if (timeout.time(sec) > 3.0)
            {
                check = false;
            }
        }
        left_.stop();
        right_.stop();
    }
    void PIDturn(float angle, float kp = 0.4, float ki = 0.00000035, float kd = 0.01)
    {
        float average_dist = 0;
        float derivate_difference;
        float cumerror = 0;
        bool check = true;
        float error;
        float speed;
        float prev_error = 0;
        timeout.reset();
        left_.spin(forward);
        right_.spin(forward);
        if(angle > 180){
            angle = angle -360;
        }
        while (check)
        {
            // potential
            error = angle - BrainInertial.rotation();
            // integral
            cumerror += error;
            // derivative
            derivate_difference = error - prev_error;
            speed = kp * (error) + ki * cumerror + kd * derivate_difference;
            left_.setVelocity(1 * speed, percent);
            right_.setVelocity(-1 * speed, percent);
            prev_error = error;
            if (timeout.time(sec) > 5.0)
            {
                check = false;
            }
        }
        left_.stop();
        right_.stop();
    }
    bool checkForPlant()
    {

        Brain.Screen.printAt(10, 50, "DisPlant: %.1f", DistanceSensor.objectDistance(mm));
        wait(1,seconds);
        if (DistanceSensor.objectDistance(mm) < 200)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    int moveToPlant()
    {
        int colourVal = 0;
        float distance_initial = 0;
        while(timeout.time(sec) < 5.0){
            Brain.Screen.printAt(10, 50, "RUN: %.1f", DistanceSensor.objectDistance(mm));
        }
        distance_initial = DistanceSensor.objectDistance(mm);
        distance_initial =  distance_initial - 50.0;
        Brain.Screen.printAt(10, 50, "final: %.1f", distance_initial);
        timeout.reset();
        PIDmove(distance_initial);
        if (ColourSensor.color() == cyan)
        {
            colourVal = 1;
        }
        else if (ColourSensor.color() == green)
        {
            colourVal = 2;
        }
        else if (ColourSensor.color() == yellow)
        {
            colourVal = 3;
        }
        else if (ColourSensor.color() == violet)
        {
            colourVal = 4;
        }
        wait(1,seconds);
        PIDmove(-1*distance_initial);
        wait(1,seconds);
        return colourVal;
<<<<<<< HEAD
     }
    int colourtotime (int colourValue)
    int timetowater = 0;
    {
        if (colourValue == 1)
        {
            time = 3;
        }
        if (colourValue == 2)
        {
            time = 6;
        }
        if (colourValue == 3)
        {
            time = 9;
        }
        if (colourValue == 4)
        {
            time = 11;
        }
        return time;
    }
    void pourwater (int time)
    {
        int adjustment = 1 // change adjustment upon testing
        timeout.reset()
        PumpMotor.spin(forwards)
        while (timeout.value() < time*adjustment)
        {}
        PumpMotor.stop()
    }

    void move()
    {
<<<<<<< HEAD
        right_.setVelocity(100,percent);
        right_.spin(forward);
    }
    void touchandgo ()
    {
        while (TouchSensor.pressing() = 0)
        {}
        while (TouchSensor.pressing() = 1)
        {}
    }
=======
       //
<<<<<<< HEAD

>>>>>>> 361e9c9384259890134860ab4083af20e295f836
=======
>>>>>>> ea23f0a5f3235c1653658adda22c825d054cea7a
=======
>>>>>>> bbb57b28618af9904f729a0c16d66e5cb75b73f5
    }
    // void touchAndGo()
    // {
    //     while (!TouchSensor.pressing())
    //     {
    //     }
    //     while (TouchSensor.pressing())
    //     {
    //     }
    //     int distance_initial = 0;
    //     int distance_final = 0;
    //     int distance_togo = 0;
    //     string color_ind = " ";
    //     int colour_ind = 0;
    //     distance_initial = DistanceSensor.objectDistance(mm);
    //     Left_.setPosition(0, turns);
    //     Left_.setVelocity(10, percent);
    //     Right_.setVelocity(10, percent);
    //     Left_.spin(forwards);
    //     Right_.spin(forwards);
    //     while (DistanceSensor(mm) < 30;
    //     {}
    //     Left_.stop();
    //     Right_.stop();
    //     distance_final = DistanceSensor.objectDistance(mm);
    //     distance_togo = distance_initial - distance_final;
    //     if (colour
    // }

    void dfs(int grid[][3], int current_x_pos, int current_y_pos, bool visit_Array[][3])
    {
        // Check if current position is out of bounds or already visited
        if (current_x_pos < 0 || current_x_pos >= grid_rows || current_y_pos < 0 || current_y_pos >= grid_cols || visit_Array[current_x_pos][current_y_pos])
        {
            return;
        }

        // Mark current position as visited
        visit_Array[current_x_pos][current_y_pos] = true;

        // Define directions: Up, Right, Down, Left
        // [0][0] is top-left corner
        // Up: y-1, Right: x+1, Down: y+1, Left: x-1
        int directions_change[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
        int posible_movement[4][2] = {};
        int cnt = 0;

        // Check all 4 directions
        for (int i = 0; i < 4; i++)
        {
            int new_pos_x = current_x_pos + directions_change[i][0];
            int new_pos_y = current_y_pos + directions_change[i][1];

            Brain.Screen.clearScreen();
            Brain.Screen.printAt(10, 30, "Checking dir %d", i);
            Brain.Screen.printAt(10, 50, "From [%d][%d]", current_x_pos, current_y_pos);
            Brain.Screen.printAt(10, 70, "To [%d][%d]", new_pos_x, new_pos_y);
            wait(2, seconds);

            // Check if position is valid before turning and checking
            if (new_pos_x < 0 || new_pos_x >= grid_rows || new_pos_y < 0 || new_pos_y >= grid_cols)
            {
                Brain.Screen.clearScreen();
                Brain.Screen.printAt(10, 50, "Out of bounds:");
                Brain.Screen.printAt(10, 70, "[%d][%d]", new_pos_x, new_pos_y);
                wait(2, seconds);
                continue; // Skip this direction
            }

            if (visit_Array[new_pos_x][new_pos_y])
            {
                Brain.Screen.clearScreen();
                Brain.Screen.printAt(10, 50, "Already visited:");
                Brain.Screen.printAt(10, 70, "[%d][%d]", new_pos_x, new_pos_y);
                wait(2, seconds);
                continue; // Skip this direction
            }

            // Turn to face direction i (0=up, 1=right, 2=down, 3=left)
            PIDturn(90 * i);
            wait(0.5, seconds);

            // Check if there's a plant in this direction
            if (checkForPlant())
            {
                // Plant detected - move to it, get color, move back
                int colour = moveToPlant();

                // Store color in grid if position is valid
                if (new_pos_x >= 0 && new_pos_x < grid_rows && new_pos_y >= 0 && new_pos_y < grid_cols)
                {
                    grid[new_pos_x][new_pos_y] = colour;
                    visit_Array[new_pos_x][new_pos_y] = true;
                    Brain.Screen.printAt(10, 70, "Plant at [%d][%d]: %d", new_pos_x, new_pos_y, colour);
                    wait(1, seconds);
                    Brain.Screen.clearScreen();
                }
            }
            else
            {
                // No plant - this is an empty cell, add to possible movements
                posible_movement[cnt][0] = new_pos_x;
                posible_movement[cnt][1] = new_pos_y;
                Brain.Screen.clearScreen();
                Brain.Screen.printAt(10, 50, "Possible move %d:", cnt);
                Brain.Screen.printAt(10, 70, "[%d][%d]", new_pos_x, new_pos_y);
                wait(2, seconds);
                cnt++;
            }
        }

        // Now visit all possible empty cells (DFS recursion)
        for (int i = 0; i < cnt; i++)
        {
            int next_cell_x = posible_movement[i][0];
            int next_cell_y = posible_movement[i][1];

            // Calculate which direction to turn to face the next cell
            int dx = next_cell_x - current_x_pos;
            int dy = next_cell_y - current_y_pos;
            int direction = 0;
            if (dy == -1) direction = 0;      // Up
            else if (dx == 1) direction = 1;  // Right
            else if (dy == 1) direction = 2;  // Down
            else if (dx == -1) direction = 3; // Left

            // Turn to face the direction and move to next cell
            PIDturn(90 * direction);
            wait(0.5, seconds);
            PIDmove(300); // Move 100mm to next cell
            wait(0.5, seconds);

            // Recursively explore from new position
            dfs(grid, next_cell_x, next_cell_y, visit_Array);

            // Backtrack: turn around, move back, turn around again
            Brain.Screen.clearScreen();
            Brain.Screen.printAt(10, 50, "DEADEND %d:", cnt);
            Brain.Screen.printAt(10, 70, "[%d][%d]", dx, dy);
            wait(1,seconds);
            Brain.Screen.clearScreen();
            PIDturn(90 * ((direction + 2) % 4)); // Turn 180 degrees
            wait(0.5, seconds);
            PIDmove(300); // Move back
            wait(0.5, seconds);
            PIDturn(90 * direction); // Face original direction
            wait(0.5, seconds);
        }
        Brain.Screen.clearScreen();
        Brain.Screen.printAt(10, 50, "DFS DONE!!!");
    }
};

// void PIDturn2(float angle, float kp = 0.4, float ki = 0.000026, float kd = 0.01)
//     {
//         float average_dist = 0;
//         float derivate_difference;
//         float cumerror = 0;
//         bool check = true;
//         float error;
//         float speed;
//         float prev_error = 0;
//         timeout.reset();
//         left_.spin(forward);
//         right_.spin(forward);
//         int changer = 1;
//         int convert = 1;
//         // if(angle > 180){
//         //     angle -= 360;
//         // }
//         if(BrainInertial.rotation() < 0){
//             convert = 1;
//         }
//         if ((BrainInertial.rotation() - angle) < ((BrainInertial.rotation()) + angle))
//         {
//             changer = -1;
//         }
//         while (check)
//         {
//             // potential
//             error = angle - (convert * BrainInertial.rotation());
//             // integral
//             cumerror += error;
//             // derivative
//             derivate_difference = error - prev_error;
//             speed = kp * (error) + ki * cumerror + kd * derivate_difference;
//             left_.setVelocity(-1 * speed * changer, percent);
//             right_.setVelocity(speed * changer, percent);
//             prev_error = error;
//             if (timeout.time(sec) > 4.0)
//             {
//                 check = false;
//             }
//             Brain.Screen.printAt(10, 50, "Rotation: %.1f", BrainInertial.rotation());
//             Brain.Screen.printAt(10, 70, "Target: %.1f", angle);
//             Brain.Screen.printAt(10, 90, "Error: %.1f", error);
//         }
//         left_.stop();
//         right_.stop();
//     }



*/