#include "vec3.h"

class matrix
{

    public:
        float* matrix;
        //1, 0, 0, 0
        //0, 1, 0, 0
        //0, 0, 1, 0
        //0, 0, 0, 1        
};

float* default_mat();

float* mat_transform(float x, float y, float z);

float* mat_scale(float x, float y, float z);

float* mat_rotate_x(float value);

float* mat_rotate_y(float value);

float* mat_rotate_z(float value);

vector3 mat_x_vec3(float* mul, vector3 vec);

void print_mat(float* mat);
