#ifndef CUP_CONSTS_H
#define CUP_CONSTS_H

#define Debug
#undef Debug

const unsigned lidartimeout=100; // in ms

// offsets
const int servo_offset = 95;  // 用于调整舵机中点
const float car_left=-10;
const float car_right=10;
const float carlen=150;

// pins
const int SERVO_PIN = 3;

const int speed=20;

#endif // CUP_CONSTS_H