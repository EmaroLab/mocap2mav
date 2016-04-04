#ifndef CONVERSIONS_H
#define CONVERSIONS_H
#include <cmath>
#include <cstdint>

inline static void quaternion_to_dcm(const float quaternion[4], float dcm[3][3])
{
    double a = quaternion[0];
    double b = quaternion[1];
    double c = quaternion[2];
    double d = quaternion[3];
    double aSq = a * a;
    double bSq = b * b;
    double cSq = c * c;
    double dSq = d * d;
    dcm[0][0] = aSq + bSq - cSq - dSq;
    dcm[0][1] = 2.0 * (b * c - a * d);
    dcm[0][2] = 2.0 * (a * c + b * d);
    dcm[1][0] = 2.0 * (b * c + a * d);
    dcm[1][1] = aSq - bSq + cSq - dSq;
    dcm[1][2] = 2.0 * (c * d - a * b);
    dcm[2][0] = 2.0 * (b * d - a * c);
    dcm[2][1] = 2.0 * (a * b + c * d);
    dcm[2][2] = aSq - bSq - cSq + dSq;
}

inline static void dcm_to_euler(const float dcm[3][3], float* roll, float* pitch, float* yaw)
{
    float phi, theta, psi;
    theta = asin(-dcm[2][0]);

    if (fabsf(theta - (float)M_PI_2) < 1.0e-3f) {
        phi = 0.0f;
        psi = (atan2f(dcm[1][2] - dcm[0][1],
                dcm[0][2] + dcm[1][1]) + phi);

    } else if (fabsf(theta + (float)M_PI_2) < 1.0e-3f) {
        phi = 0.0f;
        psi = atan2f(dcm[1][2] - dcm[0][1],
                  dcm[0][2] + dcm[1][1] - phi);

    } else {
        phi = atan2f(dcm[2][1], dcm[2][2]);
        psi = atan2f(dcm[1][0], dcm[0][0]);
    }

    *roll = phi;
    *pitch = theta;
    *yaw = psi;
}

inline static void dcm_to_quaternion(const float dcm[3][3], float quaternion[4])
{
    quaternion[0] = (0.5 * sqrt(1.0 +
            (double)(dcm[0][0] + dcm[1][1] + dcm[2][2])));
    quaternion[1] = (0.5 * sqrt(1.0 +
            (double)(dcm[0][0] - dcm[1][1] - dcm[2][2])));
    quaternion[2] = (0.5 * sqrt(1.0 +
            (double)(-dcm[0][0] + dcm[1][1] - dcm[2][2])));
    quaternion[3] = (0.5 * sqrt(1.0 +
            (double)(-dcm[0][0] - dcm[1][1] + dcm[2][2])));
}


inline static void mul(float q1[4],float q2[4],float res[4]) {
    res[1] =  q1[1] * q2[0] + q1[2] * q2[3] - q1[3] * q2[2] + q1[0] * q2[1];
    res[2] = -q1[1] * q2[3] + q1[2] * q2[0] + q1[3] * q2[1] + q1[0] * q2[2];
    res[3] =  q1[1] * q2[2] - q1[2] * q2[1] + q1[3] * q2[0] + q1[0] * q2[3];
    res[0] = -q1[1] * q2[1] - q1[2] * q2[2] - q1[3] * q2[3] + q1[0] * q2[0];
}
#endif
