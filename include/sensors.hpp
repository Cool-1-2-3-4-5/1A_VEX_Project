#pragma once
#include "iq2_cpp.h"
using namespace vex;

class Sensors
{
    optical Optical;
    bumper bumper;
    inertial BrainInertial;
    brain Brain;

public:
    Sensors(char Optical_port, char Bumper_port):
        left_(left_Port,false),
        right_(right_Port,true)
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
    void PIDmove(float kp, float ki, float kd, float distance){
        distance = distance * (360.0/200);
        Brain.Screen.clearScreen();
        left_.setPosition(0,degrees);
        right_.setPosition(0,degrees);
        float average_dist = 0;
        float derivate_difference;
        float cumerror = 0;
        bool check = true;
        float error;
        float speed;
        float prev_error = 0;
        timer timeout;
        timeout.reset();
        left_.spin(forward);
        right_.spin(forward);
        while(check){
            average_dist = (left_.position(degrees)+right_.position(degrees))/2;
            //potential
            error = distance-average_dist;
            //integral
            cumerror += error;
            // derivative
            derivate_difference = error - prev_error;
            speed = kp*(error) + ki*cumerror + kd*derivate_difference;
            left_.setVelocity(speed, percent);
            right_.setVelocity(speed, percent);
            prev_error = error;
            if(timeout.time(sec) > 3.0) {
                check = false;
                Brain.Screen.printAt(25, 30, "Timeout!");
            }
            // wait(20, msec);
        }
        left_.stop();
        right_.stop();
    }
    void PIDturn(float kp, float ki, float kd, float angle){
        Brain.Screen.clearScreen();
        float average_dist = 0;
        float derivate_difference;
        float cumerror = 0;
        bool check = true;
        float error;
        float speed;
        float prev_error = 0;
        timer timeout;
        timeout.reset();
        left_.spin(forward);
        right_.spin(forward);
        while(check){
            average_dist = (left_.position(degrees)+right_.position(degrees))/2;
            //potential
            error = angle-average_dist;
            //integral
            cumerror += error;
            // derivative
            derivate_difference = error - prev_error;
            speed = kp*(error) + ki*cumerror + kd*derivate_difference;
            left_.setVelocity(speed, percent);
            right_.setVelocity(speed, percent);
            prev_error = error;
            if(timeout.time(sec) > 3.0) {
                check = false;
                Brain.Screen.printAt(25, 30, "Timeout!");
            }
            // wait(20, msec);
        }
        left_.stop();
        right_.stop();
    }
};