#ifndef __GEOM_H__
#define __GEOM_H__
#include <list>
using namespace std;

typedef struct Vertex Vertex;
typedef struct Face Face; 

struct Vertex
{
    float x;
    float y;
    float z;
};

struct Face
{
    int u;
    int v;
    int w;
};

class Trimesh
{
    list<Vertex> v_list;
    list<Face> f_list;

    public:
        void addVertex(float data[])
        {
            Vertex v = {data[0],data[1],data[2]};
            v_list.push_back(v);
        }

        void addFace(int data[])
        {
            Face f = {data[0],data[1],data[2]};
            f_list.push_back(f);
        }
};
#endif