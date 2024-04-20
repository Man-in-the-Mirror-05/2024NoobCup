#include "Arduino.h"
#include "MotorDriver.h"

void MotorDriver::begin() {
    pinMode(LEFT_DIR_PIN, OUTPUT);
    pinMode(LEFT_MOTOR_PIN, OUTPUT);
    pinMode(RIGHT_DIR_PIN, OUTPUT);
    pinMode(RIGHT_MOTOR_PIN, OUTPUT);
    drive(0);
}

void MotorDriver::driveLeft(int speed) {
    _drive_motor(LEFT_MOTOR_PIN, LEFT_DIR_PIN, speed);
}

void MotorDriver::driveRight(int speed) {
    _drive_motor(RIGHT_MOTOR_PIN, RIGHT_DIR_PIN, speed);
}

inline void MotorDriver::_drive_motor(int motor_pin, int dir_pin, int speed) {
    if (speed >= 0) {
        digitalWrite(dir_pin, LOW);
        analogWrite(motor_pin, speed<256?speed:255);
    } else {
        digitalWrite(dir_pin, HIGH);
        speed=-speed;
        analogWrite(motor_pin, speed<256?speed:255);
    }
}

void MotorDriver::drive(int left_speed, int right_speed) {
    driveLeft(left_speed);
    driveRight(right_speed);
}

void MotorDriver::drive(int speed) {
    driveLeft(speed);
    driveRight(speed);
}
