#ifndef RAY_MATH_H
#define RAY_MATH_H


#include <stdbool.h>


#define ISCLOSE_1 0.1f
#define ISCLOSE_2 0.01f
#define ISCLOSE_3 0.001f
#define ISCLOSE_4 0.0001f
#define ISCLOSE_5 0.00001f
#define ISCLOSE_6 0.000001f
#define ISCLOSE_7 0.0000001f
#define ISCLOSE_8 0.00000001f
#define ISCLOSE_9 0.000000001f
#define ISCLOSE_10 0.000000001f


#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))


float map(float value, float old_low, float old_high, float new_low, float new_high);

float constrain(float value, float low, float high);

bool isclose(float a, float b, float epsilon);

float degrees(float angle);

float radians(float angle);


#endif // RAY_MATH_H

