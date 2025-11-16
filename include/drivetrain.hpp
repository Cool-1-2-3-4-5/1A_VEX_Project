#pragma once
#include "iq2_cpp.h"
using namespace vex;

class Drivetrain
{
private:
    motor left_;
    motor right_;
    inertial BrainInertial;
    distance DistanceSensor;
    colorsensor ColourSensor;
    touchled TouchSensor;
    motor_group PumpMotor; // Check this is right 
    timer timeout;
    int grid_rows = 0;
    int grid_cols = 0;

public:
    Drivetrain(char left_Port, char right_Port) : left_(left_Port, false), right_(right_Port, true)
    {
        left_.setStopping(brakeType::hold);
        right_.setStopping(brakeType::hold);
        left_.setVelocity(0, percent);
        right_.setVelocity(0, percent);
    }
    void setGrid(int x = 4, int y = 4)
    {
        grid_rows = x;
        grid_cols = y;
    }
    void IMUcalibrate()
    {
        BrainInertial.calibrate();
        BrainInertial.setRotation(0, degrees);
        BrainInertial.setHeading(0, degrees);
    }
    void stop()
    {
        left_.stop();
        right_.stop();
    }
    void PIDmove(float kp, float ki, float kd, float distance)
    {
        distance = distance * (360.0 / 200);
        left_.setPosition(0, degrees);
        right_.setPosition(0, degrees);
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
        while (check)
        {
            average_dist = (left_.position(degrees) + right_.position(degrees)) / 2;
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
    void PIDturn(float kp, float ki, float kd, float angle, float current_angle)
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
        int changer = 1;
        while (check)
        {
            average_dist = (left_.position(degrees) + right_.position(degrees)) / 2;
            // potential
            error = angle - average_dist;
            // integral
            cumerror += error;
            // derivative
            derivate_difference = error - prev_error;
            speed = kp * (error) + ki * cumerror + kd * derivate_difference;
            left_.setVelocity(-1 * speed * changer, percent);
            right_.setVelocity(speed * changer, percent);
            prev_error = error;
            if (timeout.time(sec) > 3.0)
            {
                check = false;
            }
            // wait(20, msec);
        }
        left_.stop();
        right_.stop();
    }

<<<<<<< HEAD
    // bool checkForPlant()
    // {
    //     if(DistanceSensor.objectDistance(mm)  < 120)
    //     {
    //         return true;
    //     }
    //     else{
    //         return false;
    //     }
    // }
=======
    bool checkForPlant()
    {
        if (DistanceSensor.objectDistance(mm) < 120)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
>>>>>>> ea23f0a5f3235c1653658adda22c825d054cea7a

     int moveToPlant()
     {
        int distance_initial = 0;
        distance_initial = DistanceSensor.objectDistance(mm);
        PIDmove(0.4, 0.000008, 0.01, distance_initial - 25);
        if (ColourSensor == orange)
        {
            colourVal = 1;
        }
        if (ColourSensor == green)
        {
            colourVal = 2;
        }
        if (ColourSensor == yellow)
        {
            colourVal = 3;
        }
        if (ColourSensor == violet)
        {
            colourVal = 4;
        }
        PIDmove(0.4, 0.000008, 0.01, -distance_initial + 25);
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
    }

    // void dfs(int grid[][4], int current_x_pos, int current_y_pos, bool visit_Array[][4], int posible_movement[][2])
    // {
    //     int cnt = 0;
    //     int colour = 0;
    //     int max_row_space = grid_rows;
    //     int max_col_space = grid_cols;
    //     if (current_x_pos > max_row_space || current_y_pos > max_col_space || current_x_pos < 0 || current_y_pos < 0 || visit_Array[current_x_pos][current_y_pos])
    //     {
    //     }
    //     else
    //     {
    //         visit_Array[current_x_pos][current_y_pos] = true;
    //         int directions_change[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    //         for (int i = 0; i < 4; i++)
    //         {
    //             int new_pos_x = current_x_pos + directions_change[i][0];
    //             int new_pos_y = current_y_pos + directions_change[i][1];
    //             PIDturn(0.4, 0.000008, 0.01, 90 * i);
    //             if (checkForPlant())
    //             {
    //                 colour = moveToPlant();
    //                 grid[new_pos_x][new_pos_y] = colour;
    //                 visit_Array[new_pos_x][new_pos_y] = true;
    //             }
    //             else
    //             {
    //                 if (new_pos_x < max_row_space && new_pos_y < max_col_space && new_pos_x > 0 && new_pos_y > 0 && !visit_Array[new_pos_x][new_pos_y])
    //                 {
    //                     posible_movement[cnt][0] = new_pos_x;
    //                     posible_movement[cnt][1] = new_pos_y;
    //                 }
    //             }
    //             cnt++;
    //         }
    //         for (int i = 0; i < cnt; i++)
    //         {
    //             if (posible_movement[i][0] != NULL)
    //             {
    //                 int next_cell_x = posible_movement[i][0];
    //                 int next_cell_y = posible_movement[i][1];
    //                 PIDturn(0.4, 0.000008, 0.01, 90 * i);
    //                 PIDmove(0.4, 0.000008, 0.01, 100);
    //                 dfs(grid, next_cell_x, next_cell_y,visit_Array,posible_movement);
    //             }
    //         }
    //     }
    // }
};
