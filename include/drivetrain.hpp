#pragma once
#include "iq2_cpp.h"
using namespace vex;

class Drivetrain
{
private:
    motor left_;
    motor right_;
    inertial BrainInertial;
    timer timeout;

public:
    Drivetrain(char left_Port, char right_Port, char bumper_Port, char pump_Port) : left_(left_Port, false),
                                                                                    right_(right_Port, true)
    {
        left_.setStopping(brakeType::hold);
        right_.setStopping(brakeType::hold);
        left_.setVelocity(0, percent);
        right_.setVelocity(0, percent);
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