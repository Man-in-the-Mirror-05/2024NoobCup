#include "gogogo.h"
#include "consts.h"
#include <math.h>
#include <Arduino.h>
#define Debug
// two situations:
// - should run in the middle of left and right bound
// - have one bound in front, should turn

/// return x coordinate of polar cord (r, theta)
inline float x_of(float r, float theta) {
    return r*cosf(theta*M_PI/180.);
}

const float bound_judge=10;

Go next(const float* dist, const bool *valid){
    // left boundary
    int bound_left=150;
    while((--bound_left)>=30){
        if (dist[bound_left+1]<10 || dist[bound_left]<10) continue;
        float diff=dist[bound_left+1]-dist[bound_left];
        if(diff>bound_judge || diff<-bound_judge) break; // boundary found
    }
    // boundary not found, turn!
    if (bound_left<30){
        float sum_left=0, sum_right=0;
        int count_left=0, count_right=0;
        for(int i=30; i<150; i+=3){
            if (dist[i+3]<10 || dist[i]<10) continue;
            float dx=dist[i+3]-dist[i];
            if(dx>0) {sum_left+=1./dist[i]; count_left++;}
            else {sum_right+=1./dist[i]; count_right++;}
        }
        if(count_left!=0) sum_left/=count_left;
        if(count_right!=0)sum_right/=count_right;
        float angle=90+5.*(sum_left-sum_right);
        #ifdef Debug
        Serial.printf("no boundary, turn! sum:(%f, %f), angle: %f\r\n", sum_left, sum_right, angle);
        #endif
        
        return Go{speed/3, constrain(angle, 80, 100)};
    }
    // boundary found, find right bound (which MUST exist)
    int bound_right=30;
    while((++bound_right)<=bound_left){
        if (dist[bound_right-1]<10 || dist[bound_right]<10) continue;
        float diff=dist[bound_right-1]-dist[bound_right];
        if(diff>bound_judge || diff<-bound_judge) break; // boundary found
    }

    float sum_left = 0;
    int cnt_left = 0;
    for (int i = bound_left; i <= 150; i++) {
        if (valid[i]) {
            sum_left += x_of(dist[i],i)-car_left;
            cnt_left++;
        }
    sum_left /= cnt_left;
    }
    float sum_right = 0; 
    int cnt_right = 0;
    for (int i <= bound_right; i <= 30; i++) {
        if (valid[i]) {
            sum_right += x_of(dist[i],i)-car_right;
            cnt_right++;
        }
    sum_right /= cnt_right;
    }
    // 
    // TODO: O(log(n)) implement proof
    float left_angle=150;
    bool left_angle_found=false;
    for(int i=150; i>=bound_left; i--){
        if (dist[i]<10) continue;
        if(x_of(dist[i], i)>car_left) { // crash!
            left_angle=i;
            left_angle_found=true;
            break;
        }
    }
    float right_angle=30;
    bool right_angle_found=false;
    for(int i=30; i<=bound_right; i++){
        if (dist[i]<10) continue;
        if(x_of(dist[i], i)<car_right) { // crash!
            right_angle=i-1;
            right_angle_found=true;
            break;
        }
    }
    #ifdef Debug
    Serial.printf("boundary found! (%f, %f) \r\n", left_angle, right_angle);
    #endif
    if (!(left_angle_found || right_angle_found)) // safe
        return Go{speed, 90};
    if (!left_angle_found && right_angle_found) { // right will crash
        return Go{speed/3, 100};
    }
    if (left_angle_found && !right_angle_found) { // left will crash
        return Go{speed/3, 80};
    }
    // will both crash, boom! (?)
    return Go{speed/5, constrain(90+(left_angle-right_angle)/2,80,100)};
}