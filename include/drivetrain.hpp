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

    int moveToPlant()
    {
        int distance_initial = 0;
        int distance_final = 0;
        int distance_togo = 0;
        string color_ind = " ";
        int colour_ind = 0;
        distance_initial = DistanceSensor.objectDistance(mm);
        Left_.setPosition(0, turns);
        Left_.setVelocity(10, percent);
        Right_.setVelocity(10, percent);
        Left_.spin(forwards);
        Right_.spin(forwards);
        while (DistanceSensor(mm) < 30;
        {}
        Left_.stop();
        Right_.stop();
        distance_final = DistanceSensor.objectDistance(mm);
        distance_togo = distance_initial - distance_final;
        if (colour
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
