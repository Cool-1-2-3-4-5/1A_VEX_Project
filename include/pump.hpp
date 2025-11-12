#pragma once
#include "iq2_cpp.h"
using namespace vex;

class Pump
{
private:
    motor pump;
    timer timeout;
public:
    Pump(char Port) : pump(Port, false)
    {
        pump.setVelocity(40, percent);
    }
    void pumpSpinFor(int sec)
    {
        pump.spin(forward);
    }
    void stop()
    {
        pump.stop();
    }
    void pumpSpinFor(float kp, float ki, float kd, int sec)
    {
        timeout.reset();
        bool check = true;
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
            // wait(20, msec);
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
};

    // bool bumpMove(float dist_to_middle)
    // {
    //     Brain.Screen.clearScreen();
    //     left_.setPosition(0, degrees);
    //     right_.setPosition(0, degrees);
    //     const float tolerance = 9;
    //     float current_distance = 0;
    //     float difference = 0;
    //     left_.setVelocity(50, percent);
    //     right_.setVelocity(50, percent);
    //     left_.spin(forward);
    //     right_.spin(forward);
    //     while ((!Bumper9.pressing()) && (((left_.position(degrees) + right_.position(degrees)) / 2.0) * (200.0 / 360)) < dist_to_middle)
    //     {
    //     left_.stop();
    //     right_.stop();
    //     current_distance = ((left_.position(degrees) + right_.position(degrees)) / 2.0) * (200.0 / 360);
    //     Brain.Screen.clearScreen();
    //     wait(2, sec);
    //     Brain.Screen.clearScreen();
    //     difference = current_distance - dist_to_middle;
    //     if (difference < 0)
    //     {
    //         difference = -difference;
    //     }
    //     if (difference <= tolerance)
    //     {
    //         return false;
    //     }
    //     else
    //     {
    //         PIDmove(0.4, 0.000008, 0.01, -1 * current_distance);
    //         return true;
    //     }

    //     wait(3, sec); // Keep results visible longer
    // }