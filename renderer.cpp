#include<iostream>
#include<vector>
#include<cmath>
#include"matrix.h"
#include<string>

#if defined(NOSYNC)
    std::cout.sync_with_stdio(false);
#endif

char print_val(float float_val)
{
    if(float_val == -10)
    {
        return ' '; 
    }
    else if(float_val < 0)
    {
        return '.';
    }
    else if(float_val < 0.75)
    {
        return 'O';
    }
    else if(float_val < 1.5)
    {
        return ':';
    }
    else if(float_val < 2)
    {
        return '@';
    }else{
        return 'W';
    }

}

class frame_buffer{
    public:
        float *buffer_data;
        int size_x, size_y;
        int off_x, off_y;
        char *frame_print;
        int buffer_size;

        frame_buffer(int res_x, int res_y)
        {
            buffer_data = new float[res_x * res_y];
            size_x = res_x;
            size_y = res_y;
            off_x = size_x/2;
            off_y = size_y/2;
            frame_print = new char[(res_x * res_y)];

            buffer_size = res_x * res_y;
        }

        ~frame_buffer()
        {
            delete[] buffer_data;
        }

        float get_pixel(int x, int y)
        {
            if(x >= 0 && x < size_x && y >= 0 && y < size_y){
                return buffer_data[x + (y * size_x)];
            }
            return -1;
        }

        float set_pixel(int new_x, int new_y, float value){
            int x = new_x;
            int y = new_y;
            if(x >= 0 && x < size_x+1 && y >= 0 && y < size_y){
                buffer_data[x + (y * (size_x))] = value;
            }
            return 0;
        }

        void print_frame()
        {
            std::cout << std::flush;
            std::string outputval;
            for(int y = 0; y < size_y; y++)
            {
                //char this_line[size_x];
                for(int x = 0; x < size_x; x++){
                    //this_line[x] = (print_val(buffer_data[x + (y * size_x)]));
                    //frame_print[x + (y * (size_x + 1))] = print_val(buffer_data[x + (y * (size_x + 1))]);
                    char val = print_val(buffer_data[x + (y * size_x)]);
                    outputval += val;
                    outputval += val;
                    //outputval += print_val(buffer_data[x + (y * size_x)]);
                }
                outputval += '\n';
                //frame_print [(y+1) * (size_x+1)] = '\n';
                //frame_print [y * (size_x+1)] = '\\';
                //std::cout << this_line;
                //delete[] this_line;
            }
            std::cout << outputval;
        }

        void clear_buffer()
        {
            char* val;
            for(int x = 0; x < (size_x) * size_y; x++)
            {
                buffer_data[x] = -10;
            }
        }

        void draw_line(int ax, int ay, int bx, int by)
        {
            if(abs(ax - bx) > abs(ay - by))
            {
                int xdist = abs(ax - bx);
                float ydiff = (float)(ay - by) / xdist; 
                int val = -1;
                if(ax - bx > 0)
                    val = 1;
                for(int x = 0; x < xdist; x++)
                {
                    int xpos = ax + (-x * val);
                    int ypos = ay - (ydiff * x);
                    set_pixel(xpos, ypos, (get_pixel(ax, ay) - get_pixel(bx, by)) * ((float)x / xdist));
                }
            }
            else
            {
                int ydist = abs(ay - by);
                float xdiff = (float)(ax - bx) / ydist;
                int val = -1;
                if(ay - by > 0)
                    val = 1;
                for(int y = 0; y < ydist; y++)
                {
                    int xpos = ax - (xdiff * y);
                    int ypos = ay + (-y * val);
                    set_pixel(xpos, ypos, (get_pixel(ax, ay) - get_pixel(bx, by)) * ((float)y / ydist));
                }
            }
        }

        void rasterize_tri()
        {
            for(int y = 0; y < size_y; y++)
            {
                int off = y * size_x;
                int first_ind = -1;
                int second_ind = -1;
                for(int x = 0; x < size_x; x++)
                {
                    if(buffer_data[x + off] != -10)
                    {
                        if(first_ind == -1)
                        {
                            first_ind = x + off;
                        }else{
                            if(second_ind == -1)
                            {
                                if(first_ind + 1 != x + off)
                                {
                                    second_ind = x + off;
                                    for(int i = 0; i < second_ind - first_ind; i++)
                                    {
                                        buffer_data[(first_ind + i)] = (buffer_data[first_ind] - buffer_data[second_ind]) * ((float)i / (second_ind - first_ind));
                                    }
                                    second_ind = -1;
                                    first_ind = -1;
                                    }
                                else
                                {
                                    first_ind = x + off;
                                }
                            }
                        }
                    }
                }
            }
        }

        void apply(frame_buffer *to_apply, bool depth_check)
        {
            if(depth_check)
            {
                for(int x = 0; x < buffer_size; x++)
                {
                    if(buffer_data[x] > to_apply->buffer_data[x])
                    {
                        to_apply->buffer_data[x] = buffer_data[x];
                    }
                }
            }
        }

};

class vertex{
    public:
        int x;
        int y;
        int z;

        vertex(int new_x, int new_y, int new_z){
            x = new_x;
            y = new_y;
            z = new_z;
        }

        vertex()
        {

        }
};

class renderer
{
    public:
        int x;
        int y; 
        int z;
};

class camera: public renderer{
    public:
        float camera_fov;

        camera(int new_x, int new_y, int new_z, float fov){
            x = new_x;
            y = new_y;
            z = new_z;
            camera_fov = fov;
        }
};

class object{
    
    
    public:

        static std::vector<object*> world_objects;
        

        vertex *object_vertices;
        int ob_vertex_count;

        int *object_tris;
        int ob_tri_count;

        vector3 object_position;
        vector3 object_size;        
        vector3 object_rotation;


        int array_index = 0;

        object(int vertex_count, int tri_count){
            object_vertices = new vertex[vertex_count];
            world_objects.push_back(this);
            ob_vertex_count = vertex_count;
            array_index = world_objects.size()-1;
            ob_tri_count = tri_count;
            object_tris = new int[tri_count * 3];
        }

        int set_tri(int index, int a, int b, int c)
        {
            object_tris[(index*3) + 0] = a;
            object_tris[(index*3) + 1] = b;
            object_tris[(index*3) + 2] = c;
        }

        int set_vertex(int index, vertex new_vert)
        {
            object_vertices[index] = new_vert;
            return 0;
        }

        ~object(){
            delete[] object_vertices;
            world_objects[array_index] = world_objects[world_objects.size()-1];
            world_objects[array_index]->array_index = array_index;
            world_objects[world_objects.size()-1] = this;
            world_objects.pop_back();
        }
};

vector3 vert_render(object* o, int x, frame_buffer* buffer)
{
    vector3 v = vector3(o->object_vertices[x].x, o->object_vertices[x].y, o->object_vertices[x].z);

    v = mat_x_vec3(mat_rotate_x(o->object_rotation.x), v);           

    v = mat_x_vec3(mat_rotate_y(o->object_rotation.y), v);           

    v = mat_x_vec3(mat_rotate_z(o->object_rotation.z), v);            

    v = mat_x_vec3(mat_scale(o->object_size.x, o->object_size.y, o->object_size.z), v);           

    v = mat_x_vec3(mat_transform(o->object_position.x, o->object_position.y, o->object_position.z), v);           
            
           
    int new_x = (v.x);
    int new_y = (v.y);
    int new_z = (v.z);

    buffer->set_pixel(v.x, v.y, v.z);

    return v;
}

int render_tri(object* ob, int tri_index, frame_buffer* buffer)
{
    vector3 a = vert_render(ob, ob->object_tris[(tri_index*3) + 0], buffer);   
    vector3 b = vert_render(ob, ob->object_tris[(tri_index*3) + 1], buffer);   
    vector3 c = vert_render(ob, ob->object_tris[(tri_index*3) + 2], buffer);
    //std::cout << "rendering tri" << std::endl;
    ////a.print();
    //b.print();
    //c.print();


    buffer->draw_line(a.x, a.y, b.x, b.y);
    buffer->draw_line(b.x, b.y, c.x, c.y);
    buffer->draw_line(c.x, c.y, a.x, a.y);


    buffer->rasterize_tri();   
}

std::vector<object*> object::world_objects;

int render_frame(frame_buffer* this_buffer, renderer this_source)
{
    for(object* o : object::world_objects)
    {
        for(int x = 0; x < o->ob_tri_count; x++)
        {
            //std::cout << "rendering tri" << std::endl;
            render_tri(o, x, this_buffer);
        }
    }
    return 0;
}

int main (int argc, char *argv[])
{
    frame_buffer main_buffer = frame_buffer(150, 120);
    camera main_cam = camera(0, 0, 0, 90);
    vertex new_vert = vertex(0.5, 0.5, -0.5);
    object test_object = object(8, 12);
    float size = 20;
    test_object.object_size.x = size;
    test_object.object_size.y = size;
    test_object.object_size.z = size;
    test_object.object_position.x = 50;
    test_object.object_position.y = 50;
    test_object.object_position.z = 0;
    const float vertexval = 1;

    /*
    
        0           1

            2           3


        6           7
        
            4           5 
    */
    test_object.set_vertex(0, vertex(-vertexval, vertexval, -vertexval));
    test_object.set_vertex(1, vertex(vertexval, vertexval, -vertexval));
    test_object.set_vertex(2, vertex(-vertexval, vertexval, vertexval));
    test_object.set_vertex(3, vertex(vertexval, vertexval, vertexval));
    test_object.set_vertex(4, vertex(-vertexval, -vertexval, vertexval));
    test_object.set_vertex(5, vertex(vertexval, -vertexval, vertexval));
    test_object.set_vertex(6, vertex(-vertexval, -vertexval, -vertexval));
    test_object.set_vertex(7, vertex(vertexval, -vertexval, -vertexval));
    test_object.set_tri(0, 0, 1, 2);
    test_object.set_tri(1, 1, 2, 3);
    test_object.set_tri(2, 2, 3, 4);
    test_object.set_tri(3, 3, 4, 5);
    test_object.set_tri(4, 4, 5, 6);
    test_object.set_tri(5, 5, 6, 7);
    test_object.set_tri(6, 6, 7, 0);
    test_object.set_tri(7, 7, 0, 1);
    test_object.set_tri(8, 0, 2, 4);
    test_object.set_tri(9, 2, 4, 6);
    test_object.set_tri(10, 1, 3, 7);
    test_object.set_tri(11, 3, 7, 5);


    test_object.object_rotation = vector3(32, 42, 10);

    /*object new_object = object(3, 1);
    new_object.object_size = vector3(1, 1, 1);
    new_object.set_vertex(0, vertex(2, 15, 0));
    new_object.set_vertex(1, vertex(40, 4, 0));
    new_object.set_vertex(2, vertex(2, 15, 0));
    new_object.set_tri(0, 0, 2, 1);*/
    
    /*vector3 v = vector3(0.5, 0.5, -0.5);
    
    print_mat(mat_scale(2, 2, 2));

    v.print();

    std::cout << std::endl;

    vector3 newvec = mat_x_vec3(mat_scale(2, 2, 2), v);


    print_mat(mat_rotate_x(23));

    std::cout << std::endl;


    print_mat(mat_scale(4, 2, 1));

    std::cout << std::endl;

    v = mat_x_vec3(mat_transform(2, 2, 2), v);
    v.print();*/

    main_buffer.clear_buffer();

    //print_mat(mat_transform(2, 3, 4));
    //main_buffer.print_frame();

    //main_buffer.clear_buffer();

    render_frame(&main_buffer, main_cam);
    main_buffer.print_frame();

    while(true)
    {
        render_frame(&main_buffer, main_cam);
        main_buffer.print_frame();
        system("cls");
        main_buffer.clear_buffer();
        //test_object.object_size.x += 0.1;
        //test_object.object_position.x += 0.1;
        test_object.object_rotation.x += 0.05;
        test_object.object_rotation.y += 0.05;
    }

    char *tempval;
    std::cin >> tempval;
    return 0;
}