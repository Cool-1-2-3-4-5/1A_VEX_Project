#include "drivetrain.hpp"
Drivetrain::Drivetrain(char left_Port, char right_Port, char distanceSensor_port, char colourSensor_port, char touchSensor_port) : left_(left_Port, false), right_(right_Port, true), DistanceSensor(distanceSensor_port), ColourSensor(colourSensor_port), TouchSensor(touchSensor_port)
{
    // Setup motors first
    left_.setStopping(brakeType::hold);
    right_.setStopping(brakeType::hold);
    left_.setVelocity(0, percent);
    right_.setVelocity(0, percent);
    left_.setPosition(0, turns);
    right_.setPosition(0, turns);

    // Setup screen
    Brain.Screen.clearScreen();
    Brain.Screen.setFont(mono15);

    // Setup sensors
    ColourSensor.setLight(ledState::on);
    ColourSensor.brightness(100);

    // Calibrate IMU LAST - this takes time
    BrainInertial.calibrate();
    while (BrainInertial.isCalibrating())
    {
        wait(50, msec);
    }
    BrainInertial.setRotation(0, degrees);
    BrainInertial.setHeading(0, degrees);

    Brain.Screen.clearScreen();
    Brain.Screen.printAt(10, 50, "Drivetrain Initialized!");
    wait(1, seconds);
    Brain.Screen.clearScreen();
}
Drivetrain::~Drivetrain()
{
    Brain.Screen.clearScreen();
    Brain.Screen.printAt(10, 50, "Code Complete!");
}
void Drivetrain::setGrid(int x, int y)
{
    grid_rows = x;
    grid_cols = y;
}

void Drivetrain::stop()
{
    left_.stop();
    right_.stop();
}

void Drivetrain::move()
{
    right_.setVelocity(100, percent);
    left_.setVelocity(100, percent);
    left_.spin(forward);
    right_.spin(forward);
}
void Drivetrain::PIDmove(float distance, float kp, float ki, float kd)
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
        average_dist = ((left_.position(degrees) + right_.position(degrees)) / 2) * 200.0 / 360;
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
        if (timeout.time(sec) > 3.5)
        {
            check = false;
        }
    }
    left_.stop();
    right_.stop();
}
void Drivetrain::PIDturn(float angle, float kp, float ki, float kd)
{
    // float average_dist = 0;
    float derivate_difference;
    float cumerror = 0;
    bool check = true;
    float error;
    float speed;
    float prev_error = 0;
    timeout.reset();
    left_.spin(forward);
    right_.spin(forward);
    if (angle > 180)
    {
        angle = angle - 360;
    }
    while (check)
    {
        // potential
        error = angle - BrainInertial.rotation();
        Brain.Screen.printAt(10, 50, "angle: %.1f", error);
        // integral
        cumerror += error;
        // derivative
        derivate_difference = error - prev_error;
        speed = kp * (error) + ki * cumerror + kd * derivate_difference;
        left_.setVelocity(1 * speed, percent);
        right_.setVelocity(-1 * speed, percent);
        prev_error = error;
        if (timeout.time(sec) > 3.5)
        {
            check = false;
        }
    }
    left_.stop();
    right_.stop();
}
bool Drivetrain::checkForPlant()
{
    Brain.Screen.printAt(10, 50, "DisPlant: %.1f", DistanceSensor.objectDistance(mm));
    wait(0.2, seconds);
    if (DistanceSensor.objectDistance(mm) < 200)
    {
        return true;
    }
    else
    {
        return false;
    }
}
int Drivetrain::moveToPlant()
{
    int colourVal = 0;
    float distance_initial = 0;

    // Reset timeout BEFORE using it
    timeout.reset();

    // Display distance for 5 seconds
    while (timeout.time(sec) < 2.0)
    {
        Brain.Screen.clearScreen();
        Brain.Screen.printAt(10, 50, "Distance: %.1f mm", DistanceSensor.objectDistance(mm));
        wait(0.1, seconds); // Don't spam the screen
    }

    distance_initial = DistanceSensor.objectDistance(mm);
    distance_initial = distance_initial - 20.0;
    Brain.Screen.clearScreen();
    Brain.Screen.printAt(10, 50, "Moving: %.1f mm", distance_initial);
    wait(0.2, seconds);

    PIDmove(distance_initial);

    // Get color hue
    float hue = ColourSensor.hue();
    Brain.Screen.clearScreen();
    Brain.Screen.printAt(10, 50, "Hue: %.1f", hue);
    wait(0.2, seconds);

    if (hue >= 35 && hue <= 59)
    {
        colourVal = 1; // yellow
    }
    else if (hue >= 85 && hue <= 120)
    {
        colourVal = 2; // green
    }
    else if (hue >= 279 && hue <= 303)
    {
        colourVal = 3; // purple
    }
    else if (hue >= 340 || hue <= 24)
    {
        colourVal = 4; // orange/red
    }
    else
    {
        colourVal = 8; // unknown
    }

    Brain.Screen.clearScreen();
    Brain.Screen.printAt(10, 80, "Colour: %d", colourVal);
    wait(0.2, seconds);

    PIDmove(-1 * distance_initial);
    wait(0.2, seconds);
    return colourVal;
}
void Drivetrain::WateringPosition(float &distance_initial)
{
    distance_initial = DistanceSensor.objectDistance(mm);
    distance_initial = distance_initial - 30;
    PIDmove(distance_initial);
    // waterPlant(timeval);
    wait(0.2, sec);
}

int Drivetrain::colourtotime(int colourValue)
{
    int timetowater = 0;
    if (colourValue == 1)
    {
        timetowater = 3;
    }
    if (colourValue == 2)
    {
        timetowater = 6;
    }
    if (colourValue == 3)
    {
        timetowater = 9;
    }
    if (colourValue == 4)
    {
        timetowater = 11;
    }
    return timetowater;
}
void Drivetrain::touchandgo()
{
    while (!TouchSensor.pressing())
    {
    }
    while (TouchSensor.pressing())
    {
    }
}

void Drivetrain::dfs(int grid[][3], int &current_x_pos, int &current_y_pos, bool visit_Array[][3])
{
    // Check if current position is out of bounds or already visited
    if (current_x_pos < 0 || current_x_pos >= grid_rows || current_y_pos < 0 || current_y_pos >= grid_cols)
    {
    }
    // Mark current position as visited
    else
    {
        visit_Array[current_x_pos][current_y_pos] = true;
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
            wait(0.2, seconds);

            // Check if position is valid before turning and checking
            if (new_pos_x < 0 || new_pos_x >= grid_rows || new_pos_y < 0 || new_pos_y >= grid_cols)
            {
                Brain.Screen.clearScreen();
                Brain.Screen.printAt(10, 50, "Out of bounds:");
                Brain.Screen.printAt(10, 70, "[%d][%d]", new_pos_x, new_pos_y);
                wait(0.2, seconds);
            }
            else if (visit_Array[new_pos_x][new_pos_y])
            {
                Brain.Screen.clearScreen();
                Brain.Screen.printAt(10, 50, "Already visited:");
                Brain.Screen.printAt(10, 70, "[%d][%d]", new_pos_x, new_pos_y);
                wait(0.2, seconds);
            }
            else
            {
                PIDturn(90 * i);
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
                        wait(0.2, seconds);
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
                    wait(0.2, seconds);
                    cnt++;
                }
            }
        }

        // Now visit all possible empty cells (DFS recursion)
        for (int i = 0; i < cnt; i++)
        {
            int next_cell_x = posible_movement[i][0];
            int next_cell_y = posible_movement[i][1];

            if (!visit_Array[next_cell_x][next_cell_y])
            {

                // Calculate which direction to turn to face the next cell
                int dx = next_cell_x - current_x_pos;
                int dy = next_cell_y - current_y_pos;
                int direction = 0;
                if (dy == -1)
                {
                    direction = 0; // Up
                }
                else if (dx == 1)
                {
                    direction = 1; // Right
                }
                else if (dy == 1)
                {
                    direction = 2; // Down
                }
                else
                {
                    direction = 3; // Left
                }
                PIDturn(90 * direction);
                wait(0.1, seconds);
                PIDmove(325);
                wait(0.1, seconds);

                // check new position
                dfs(grid, next_cell_x, next_cell_y, visit_Array);

                // Backtrack: turn around, move back, turn around again
                Brain.Screen.clearScreen();
                Brain.Screen.printAt(10, 50, "DEADEND %d:", cnt);
                Brain.Screen.printAt(10, 70, "[%d][%d]", dx, dy);
                wait(0.1, seconds);
                Brain.Screen.clearScreen();
                PIDturn(90 * direction);
                wait(0.1, seconds);
                PIDmove(-325);
                wait(0.1, seconds);
            }
            else
            {
                Brain.Screen.clearScreen();
                Brain.Screen.printAt(10, 50, "Checked from recusrion!!!");
                wait(2, seconds);
            }
        }
        Brain.Screen.clearScreen();
        Brain.Screen.printAt(10, 50, "DFS DONE!!!");
    }
}

void Drivetrain::array_changer(int array1[][3], int array2[][3])
{
    for (int i = 0; i < grid_rows; i++)
    {
        for (int j = 0; j < grid_cols; j++)
        {
            array2[i][j] = array1[j][i];
        }
    }
}

void Drivetrain::index_finder(int &x_pos, int &y_pos, int grid[][3], int colour_num)
{
    for (int i = 0; i < grid_rows; i++)
    {
        for (int j = 0; j < grid_cols; j++)
        {
            if (grid[i][j] == colour_num)
            {
                x_pos = i;
                y_pos = j;
            }
        }
    }
}

void Drivetrain::mapping(int grid[][3], int &numcnt, bool &finalcheck, int &x_pos, int &y_pos, int &new_x, int &new_y, bool verify[][3], int &verifycnt, int cur_x, int cur_y, int movement[], int dead[], int wanted_x, int wanted_y)
{
    if (finalcheck)
    {
        int direction[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
        int possible[4][2] = {{0}};
        int cnt = 0;
        x_pos = new_x;
        y_pos = new_y;
        for (int i = 0; i < 4; i++) // check directions
        {
            x_pos += direction[i][0];
            y_pos += direction[i][1];
            if (x_pos < 0 || y_pos < 0 || x_pos > (grid_rows - 1) || y_pos > (grid_cols - 1) || verify[x_pos][y_pos])
            {
                // out of bounds or already visited
            }
            else
            {
                if (grid[x_pos][y_pos] != 0)
                {
                    verify[x_pos][y_pos] = true;
                }
                else
                {
                    possible[cnt][0] = x_pos;
                    possible[cnt][1] = y_pos;
                    cnt++;
                }
                if (x_pos == wanted_x && y_pos == wanted_y)
                {
                    finalcheck = false;
                    if (x_pos - new_x == 0)
                    {
                        if (y_pos - new_y == 1)
                        {
                            movement[numcnt] = 1; // right
                        }
                        else
                        {
                            movement[numcnt] = 2; // left
                        }
                    }
                    else
                    {
                        if (x_pos - new_x == 1)
                        {
                            movement[numcnt] = 3; // down
                        }
                        else
                        {
                            movement[numcnt] = 4; // up
                        }
                    }
                }
            }
            x_pos = new_x;
            y_pos = new_y;
        }
        cur_x = new_x;
        cur_y = new_y;
        verify[cur_x][cur_y] = true;
        if (finalcheck)
        {
            for (int j = 0; j < cnt; j++)
            {
                if (finalcheck)
                {
                    verifycnt += 1;
                    new_x = possible[j][0];
                    new_y = possible[j][1];
                    verify[new_x][new_y] = true;
                    if (cur_x - new_x == 0)
                    {
                        if (cur_y - new_y == 1)
                        {
                            movement[numcnt] = 2; // left
                        }
                        else
                        {
                            movement[numcnt] = 1; // right
                        }
                    }
                    else
                    {
                        if (cur_x - new_x == 1)
                        {
                            movement[numcnt] = 4; // up
                        }
                        else
                        {
                            movement[numcnt] = 3; // down
                        }
                    }
                    numcnt++;
                    if (finalcheck)
                    {
                        mapping(grid, numcnt, finalcheck, x_pos, y_pos, new_x, new_y, verify, verifycnt, cur_x, cur_y, movement, dead, wanted_x, wanted_y);
                        if (finalcheck)
                        {
                            dead[numcnt - 1] = 1;
                            new_x = cur_x;
                            new_y = cur_y;
                        }
                    }
                }
            }
        }
    }
    else
    {
        numcnt = 0;
    }
}

void Drivetrain::GoToPos(int coming[], int finalcnt)
{
    for (int i = 0; i < (finalcnt - 1); i++)
    {

        if (coming[i] == 1)
        {
            PIDturn(90);
        }
        else if (coming[i] == 2)
        {
            PIDturn(270);
        }
        else if (coming[i] == 3)
        {
            PIDturn(180);
        }
        else
        {
            PIDturn(0);
        }
        PIDmove(340);
    }
    // Final turn to face the plant
    if (coming[finalcnt - 1] == 1)
    {
        PIDturn(90);
    }
    else if (coming[finalcnt - 1] == 2)
    {
        PIDturn(270);
    }
    else if (coming[finalcnt - 1] == 3)
    {
        PIDturn(180);
    }
    else
    {
        PIDturn(0);
    }
    // WateringPosition(distanceToPos);
    // Brain.Screen.clearScreen();
    // return distanceToPos;
}

void Drivetrain::comeHome(int coming[], int finalcnt)
{
    for (int i = 1; i < (finalcnt); i++)
    {
        if (coming[i] == 1)
        {
            PIDturn(90);
        }
        else if (coming[i] == 2)
        {
            PIDturn(270);
        }
        else if (coming[i] == 3)
        {
            PIDturn(180);
        }
        else
        {
            PIDturn(0);
        }
        PIDmove(340);
    }
    wait(0.2, sec);
    PIDturn(0);
}