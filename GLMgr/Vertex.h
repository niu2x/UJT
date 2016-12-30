#ifndef __vertex__
#define __vertex__

typedef struct __Vertex{
    union{
        struct {GLfloat _data[12];};
        struct {
            GLfloat x, y, z;
            GLfloat u, v;
            GLfloat r, g, b, a;
            GLfloat nx, ny, nz;
        };
    };
}Vertex;

#endif