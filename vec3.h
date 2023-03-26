#include <iostream>

class vector3
{
    public:
        float x;
        float y;
        float z;
        
        vector3(float new_x, float new_y, float new_z){
            x = new_x;
            y = new_y;
            z = new_z;
        }  

        vector3()
        {

        }

        void print()
        {
            std::cout << x << " " << y << " " << z << std::endl;
        }
};

class vector4
{
    public:
        float x;
        float y;
        float z;
        float w;

        vector4(){

        }

        vector4(vector3 v){
            x = v.x;
            y = v.y;
            z = v.z;
            w = 1;
        }
};