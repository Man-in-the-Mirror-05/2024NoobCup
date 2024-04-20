/// includes controling algorithm
#ifndef CUP_GO_H
#define CUP_GO_H

struct Go{float velocity, angle;};
Go next(const float* dist, const bool* valid);

#endif // CUP_GO_H
