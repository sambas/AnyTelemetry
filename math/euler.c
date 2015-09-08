/**
 * @file euler.c
 * @brief euler math implementation
 */

#include <math.h>
#include "quaternion.h"
#include "euler.h"

void euler2quat(struct quaternion_t *quat, struct euler_t *euler)
{
    double c1 = cos(euler->yaw / 2);
    double s1 = sin(euler->yaw / 2);
    double c2 = cos(euler->pitch / 2);
    double s2 = sin(euler->pitch / 2);
    double c3 = cos(euler->roll / 2);
    double s3 = sin(euler->roll / 2);
    
    quat->q0 = c1 * c2 * c3 - s1 * s2 * s3;
    quat->q1 = c1 * c2 * s3 + s1 * s2 * c3;
    quat->q2 = s1 * c2 * c3 + c1 * s2 * s3;
    quat->q3 = c1 * s2 * s3 - s1 * c2 * c3;
}
