#ifndef CUP_PID_H
#define CUP_PID_H
class PID{
    float k_p, k_i, k_d;
    float last, avg;
public:
    /// @brief 
    /// @param p,i,d: pid algorithm parameters
    PID(float p, float i, float d):
        k_p(p), k_i(i), k_d(d),
        last(0), avg(0){}
    
    float next(float current){
        avg=avg*0.8+current*0.2;
        float output=
            k_p * current +
            k_i * avg +
            k_d * (current-last);
        last=current;
        return output;
    }
};
#endif