/**
 * @file vector3d.h
 * @brief 3D vector interface
 */

#ifndef _VECTOR3D_H
#define _VECTOR3D_H


/**
 * @addtogroup ufo
 * @{
 */

/**
 * @defgroup math
 * @{
 */


/**
 * u = v + w
 */
void vector3d_add(float u[3], float v[3], float w[3]);

/**
 * u = v - w
 */
void vector3d_sub(float u[3], float v[3], float w[3]);
/*
vector3d_scale();
void vector3d_sadd();
void vector3d_ssub();
double vector3d_dot_product(vector3d_t u, vector3d v);
void vector3d_cross_product(vector3d u, vector3d v, vector3d w);
vector3d_normalize();
vector3d_mult_matrix();
*/    
/** @} */

/** @} */

#endif /* _VECTOR3D_H */
