#include "vector.h"

void vector_copy(double *dst, double *src, int size)
{
    int i;

    for (i = 0; i < size; i++)
        dst[i] = src[i];
}