#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H
#define MAX

enum Motor{LEFT_MOTOR, RIGHT_MOTOR};

const int LEFT_DIR_PIN = 8;
const int RIGHT_DIR_PIN = 9;
const int LEFT_MOTOR_PIN = 13;
const int RIGHT_MOTOR_PIN = 12;

class MotorDriver {
public:
    void begin(void);
    void driveLeft(int speed);
    void driveRight(int speed);
    void drive(int left_speed, int right_speed);
    void drive(int speed);
private:
    inline void _drive_motor(int motor_pin, int dir_pin, int speed);
};

#endif