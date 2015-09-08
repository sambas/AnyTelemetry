#include "../common.h"
#include "quaternion.h"


void quaternion_to_euler(quaternion_t *quat, euler_t *euler)
{
    float pitch;
    float roll;
    float yaw;
    
    roll = atan2f(2.0f * (quat->q0 * quat->q1 + quat->q2 * quat->q3), 1 - 2.0f * (quat->q1 * quat->q1 + quat->q2 * quat->q2));
    pitch = asinf(2.0f * (quat->q0 * quat->q2 - quat->q1 * quat->q3));
    yaw = atan2f(2.0f * (quat->q0 * quat->q3 + quat->q1 * quat->q2), 1 - 2.0f * (quat->q2 * quat->q2 + quat->q3 * quat->q3));
    
    euler->pitch = RAD2DEG(pitch);
    euler->roll = RAD2DEG(roll);
    euler->yaw = RAD2DEG(yaw);
}

void quaternion_from_euler(euler_t *euler, quaternion_t *quad)
{
    float c1, c2, c3, s1, s2, s3;
    
    c2 = cosf(euler->pitch / 2);
    c1 = cosf(euler->roll / 2);
    c3 = cosf(euler->yaw / 2);
    s1 = sinf(euler->roll / 2);
    s2 = sinf(euler->pitch / 2);
    s3 = sinf(euler->yaw / 2);

    quad->q0 = c1 * c2 * c3 + s1 * s2 * s3;
    quad->q1 = s1 * c2 * c3 - c1 * s2 * s3;
    quad->q2 = c1 * s2 * c3 + s1 * c2 * s3;    
    quad->q3 = c1 * c2 * s3 - s1 * s2 * c3;
}
