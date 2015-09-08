#ifndef _QUATERNION_H
#define _QUATERNION_H


typedef struct {
    float pitch;
    float roll;
    float yaw;
} euler_t;

typedef struct {
    float q0;
    float q1;
    float q2;
    float q3;
} quaternion_t;  
  
void quaternion_to_euler(quaternion_t *quat, euler_t *euler);

void quaternion_from_euler(euler_t *euler, quaternion_t *quad);


#endif /* _QUATERNION_H */
