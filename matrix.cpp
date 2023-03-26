#include "matrix.h"
#include <cmath>
#include <iostream>

float* default_mat(){
    float* new_mat = new float[16];
    for(int x = 0; x < 16; x++)
    {
        new_mat[x] = 0;
    }
    new_mat[0] = 1;
    new_mat[5] = 1;
    new_mat[10] = 1;
    new_mat[15] = 1;
    return new_mat;
}

float* mat_transform(float x, float y, float z){
    float* new_mat = default_mat();
    new_mat[3] = x;
    new_mat[7] = y;
    new_mat[11] = z;
    return new_mat; 
}

float* mat_scale(float x, float y, float z){    
    float* new_mat = default_mat();
    new_mat[0] = x;
    new_mat[5] = y;
    new_mat[10] = z;
    return new_mat;
}

float* mat_rotate_z(float value){
    float* new_mat = default_mat();
    new_mat[0] = cos(value);
    new_mat[1] = -sin(value);
    new_mat[4] = sin(value);
    new_mat[5] = cos(value);
    return new_mat;
}

float* mat_rotate_y(float value){
    float* new_mat = default_mat();
    new_mat[0] = cos(value);
    new_mat[2] = sin(value);
    new_mat[8] = -sin(value);
    new_mat[10] = cos(value); 
    return new_mat;
}

float* mat_rotate_x(float value){
    float* new_mat = default_mat();
    new_mat[5] = cos(value);
    new_mat[6] = -sin(value);
    new_mat[9] = sin(value);
    new_mat[10] = cos(value);
    return new_mat;
}

vector3 mat_x_vec3(float* mul, vector3 vec)
{
    float new_vec_4[4] = {vec.x, vec.y, vec.z, 1};
    float actual_vec_4[4];

    for(int x = 0; x < 4; x++)
    {
        float new_val = 0;
        for(int y = 0; y < 4; y++)
        {
            new_val += mul[y + (x * 4)] * new_vec_4[y];
        }
        actual_vec_4[x] = new_val;
    }
    delete[] mul;    

    return vector3(actual_vec_4[0], actual_vec_4[1], actual_vec_4[2]);
}

void print_mat(float* mat){
    for(int x = 0; x < 4; x++)
    {
        for(int y =0; y < 4; y++)
        {
            std::cout << mat[y + (x * 4)] << " ";
        }
        std::cout << std::endl;
    }
}