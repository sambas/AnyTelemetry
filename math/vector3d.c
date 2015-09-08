#include "vector3d.h"

void vector3d_add(double u[3], double v[3], double w[3])
{
    int i;
    for (i = 0; i < 3; i++)
        u[i] = v[i] + w[i];
}

void vector3d_sub(double u[3], double v[3], double w[3])
{
    int i;
    for (i = 0; i < 3; i++)
        u[i] = v[i] - w[i];
}
