#include <ESP32Servo.h>
#include <RPLidarC1.h>

#include "MotorDriver.h"
#include "consts.h"
#include "gogogo.h"
#include "pid.h"

RPLidar lidar;
Servo servo;
MotorDriver motor;

PID pid_angle(0.3, 0.2, 0.3);

#define Debug

void setup() {
    #ifdef Debug
    Serial.begin(115200);  // debug serial
    #endif
    lidar.begin(Serial2);
    lidar.startScan(lidartimeout);
    // 开启计时器，用于舵机PWM控制
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    servo.setPeriodHertz(50);  // 设置舵机PWM频率
    servo.attach(SERVO_PIN);   // 连接舵机引脚
    servo.write(servo_offset);
    motor.begin();  // 开启电机驱动
}

float dist[360];
bool valid[360];

inline const RPLidarMeasurement &read_lidar();

void loop() {
    float dist[360];  // valid: 0-359
    RPLidarMeasurement p;
    do {
        p = read_lidar();
        // convert to standard polar angle
        int angle = int(p.angle);
        if (angle <= 90) angle = 90-angle; // 0-90
        else angle = 450-angle; // 91-359
        dist[angle] = p.distance;
    } while (p.angle <= 90 || p.angle >= 270); // read only front angle
    Go next_status = next(dist);
    #ifdef Debug
    Serial.printf("Operation on this loop: angle=%3.1f, speed=%3.1f\r\n",
                    next_status.angle, next_status.velocity);
    #endif
    servo.write(next_status.angle+3);
    motor.drive(next_status.velocity);
    while(read_lidar().angle<270); // flush unused angles
}

inline const RPLidarMeasurement &read_lidar() {
    // wait data point
    while (IS_FAIL(lidar.waitPoint())) {  // fail, restart and rescan
        Serial.println("lidar fail!");
        lidar.startScan(false, lidartimeout);
    }
    return lidar.getCurrentPoint();  // include 3 32-bit copy
}