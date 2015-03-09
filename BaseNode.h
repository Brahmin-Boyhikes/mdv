/* Name: Alan Ma */
/* UTEID: am65355 */
/* CSID: ama */

#ifndef __BASE__
#define __BASE__

#include <list>
#include <cmath>
#include <limits>
#include <stdlib.h> 

#include "loader.h"
using namespace std;

#define ZOOM_VELOCITY 0.01f
#define POI_VELOCITY 0.001f
#define ANGLE_ACCELERATION 0.5f
#define ANGLE_VELOCITY 0.2f


#define RADIAN 0.0174532925f


enum Mode { POINT, WIREFRAME, SOLID, SHADED };

typedef struct Vertex Vertex;
typedef struct Face Face; 
typedef struct Vector Vector; 

struct Vector 
{
    float vx;
    float vy;
    float vz;
};

struct Vertex
{
    float x;
    float y;
    float z;
    Vector normal;
};

struct Face
{
    int u;
    int v;
    int w;
    Vector normal;
};

class BaseNode
{
public:
    // constructor
    BaseNode() { }

    // destructor - calls destroy
    virtual ~BaseNode() { destroy(); }

    // release this object from memory
    void release() { delete this; }

    // update our scene node
    virtual void update()
    {
        // loop through the list and update the children
        for( list<BaseNode*>::iterator i = m_lstChildren.begin();
            i != m_lstChildren.end(); i++ )
        {
            (*i)->update();
        }
    }

    // destroy all the children
    void destroy()
    {
        for( list<BaseNode*>::iterator i = m_lstChildren.begin();
            i != m_lstChildren.end(); i++ )
            (*i)->release();

        m_lstChildren.clear();
    }

    // add a child to our custody
    void addChild( BaseNode* pNode )
    {
        m_lstChildren.push_back(pNode);
    }

protected:
    // list of children
    std::list<BaseNode*> m_lstChildren;
};

class ObjectNode: public BaseNode
{
public:
    ObjectNode() { }
    ~ObjectNode() { }

    void update()
    {
        BaseNode::update();
    }
};

class GeometryNode: public BaseNode
{
public:
    const char* path;
    Vertex center;
    float radius;

    GeometryNode(const char* p) { path = p; }
    ~GeometryNode() { }

    void initialize(){
        calculateCenter();
        calculateNormal();
        normal_toggle = 0;
        face_toggle = 0;
        vertex_toggle = 0;
    }

    void update()
    {
        switch(mode)
        {
            case POINT: PointMode(); break;
            case WIREFRAME: WireframeMode(); break;
            case SOLID: SolidMode(); break;
            case SHADED: ShadedMode(); break;
        }

        glBegin(GL_LINES);
            if(face_toggle){
                for (vector<Face>::const_iterator it = f_list.begin(); it != f_list.end(); ++it){
                    float x = (v_list[(*it).u].x + v_list[(*it).v].x + v_list[(*it).w].x) / 3.0f;
                    float y = (v_list[(*it).u].y + v_list[(*it).v].y + v_list[(*it).w].y) / 3.0f;
                    float z = (v_list[(*it).u].z + v_list[(*it).v].z + v_list[(*it).w].z) / 3.0f;

                    glVertex3f(x, y, z);
                    glVertex3f(x + (*it).normal.vx, y + (*it).normal.vy, z + (*it).normal.vz);

                }
            }
            if(vertex_toggle){
                for (vector<Vertex>::const_iterator it = v_list.begin(); it != v_list.end(); ++it){

                    glVertex3f((*it).x, (*it).y, (*it).z);
                    glVertex3f((*it).x + (*it).normal.vx,
                        (*it).y + (*it).normal.vy,
                        (*it).z + (*it).normal.vz);
                }
            }
        glEnd();

    }

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

    void setMode(Mode m){
        mode = m;
    }

    void toggle(){
        normal_toggle = normal_toggle ? 0 : 1;
    }

    void drawFaceNormal(){
        face_toggle = face_toggle ? 0 : 1;
    }

    void drawVertexNormal(){
        vertex_toggle = vertex_toggle ? 0 : 1;
    }

    void calculateCenter(){
        float minx, miny, minz;
        minx = miny = minz = numeric_limits<float>::max();
        float maxx, maxy, maxz;
        maxx = maxy = maxz = numeric_limits<float>::min();

        for (vector<Vertex>::iterator it = v_list.begin(); it != v_list.end(); ++it){
            if((*it).x < minx)
                minx = (*it).x;
            if((*it).y < miny)
                miny = (*it).y;
            if((*it).z < minz)
                minz = (*it).z;

            if((*it).x > maxx)
                maxx = (*it).x;
            if((*it).y > maxy)
                maxy = (*it).y;
            if((*it).z > maxz)
                maxz = (*it).z;
        }
        center.x = (maxx+minx)/2.0f;
        center.y = (maxy+miny)/2.0f;
        center.z = (maxz+minz)/2.0f;

        float x = (maxx-minx)/2.0f;
        float y = (maxy-miny)/2.0f;
        float z = (maxz-minz)/2.0f;

        if(x > y && x > z)
            radius = x;
        else if(y > x && y > z)
            radius = y;
        else 
            radius = z;

        radius = (radius <= 0.0) ? -radius : radius;
    }

    void calculateNormal(){
        for (vector<Face>::iterator it = f_list.begin(); it != f_list.end(); ++it){
            Vertex a = v_list[(*it).u];
            Vertex b = v_list[(*it).w];
            Vertex c = v_list[(*it).v];

            Vector s = {b.x - a.x, b.y - a.y, b.z - a.z};
            Vector t = {c.x - a.x, c.y - a.y, c.z - a.z};

            Vector cross = {
                s.vy * t.vz - s.vz * t.vy,
                s.vz * t.vx - s.vx * t.vz,
                s.vx * t.vy - s.vy * t.vx
            };

            float length = sqrt (cross.vx * cross.vx +
                cross.vy * cross.vy +
                cross.vz * cross.vz);

            (*it).normal.vx = cross.vx / length;
            (*it).normal.vy = cross.vy / length;
            (*it).normal.vz = cross.vz / length;

            v_list[(*it).u].normal.vx += (*it).normal.vx;
            v_list[(*it).u].normal.vy += (*it).normal.vy;
            v_list[(*it).u].normal.vz += (*it).normal.vz;

            v_list[(*it).w].normal.vx += (*it).normal.vx;
            v_list[(*it).w].normal.vy += (*it).normal.vy;
            v_list[(*it).w].normal.vz += (*it).normal.vz;

            v_list[(*it).v].normal.vx += (*it).normal.vx;
            v_list[(*it).v].normal.vy += (*it).normal.vy;
            v_list[(*it).v].normal.vz += (*it).normal.vz;
        }
        for (vector<Vertex>::iterator it = v_list.begin(); it != v_list.end(); ++it){

            float length = sqrt (
                (*it).normal.vx * (*it).normal.vx +
                (*it).normal.vy * (*it).normal.vy +
                (*it).normal.vz * (*it).normal.vz);

            (*it).normal.vx = (*it).normal.vx / length;
            (*it).normal.vy = (*it).normal.vy / length;
            (*it).normal.vz = (*it).normal.vz / length;
        }
    }

private:
    vector<Vertex> v_list;
    vector<Face> f_list;
    Mode mode;
    int normal_toggle;
    int face_toggle;
    int vertex_toggle;

    void PointMode() {
        glBegin(GL_POINTS);
        for (vector<Vertex>::const_iterator it = v_list.begin(); it != v_list.end(); ++it){
            glVertex3f((*it).x, (*it).y, (*it).z);
        }
        glEnd();
    }

    void WireframeMode() {
        glBegin(GL_LINES);
        for (vector<Face>::const_iterator it = f_list.begin(); it != f_list.end(); ++it){
            glVertex3f(v_list[(*it).u].x, v_list[(*it).u].y, v_list[(*it).u].z);
            glVertex3f(v_list[(*it).v].x, v_list[(*it).v].y, v_list[(*it).v].z);

            glVertex3f(v_list[(*it).v].x, v_list[(*it).v].y, v_list[(*it).v].z);
            glVertex3f(v_list[(*it).w].x, v_list[(*it).w].y, v_list[(*it).w].z);

            glVertex3f(v_list[(*it).w].x, v_list[(*it).w].y, v_list[(*it).w].z);
            glVertex3f(v_list[(*it).u].x, v_list[(*it).u].y, v_list[(*it).u].z);
        }
        glEnd();
    }

    void SolidMode() {
        glBegin(GL_TRIANGLES);
        for (vector<Face>::const_iterator it = f_list.begin(); it != f_list.end(); ++it){
            glVertex3f(v_list[(*it).u].x, v_list[(*it).u].y, v_list[(*it).u].z);
            glVertex3f(v_list[(*it).v].x, v_list[(*it).v].y, v_list[(*it).v].z);
            glVertex3f(v_list[(*it).w].x, v_list[(*it).w].y, v_list[(*it).w].z);
        }
        glEnd();
    }

    void ShadedMode(){
        glEnable(GL_LIGHTING);
        glBegin(GL_TRIANGLES);
        for (vector<Face>::const_iterator it = f_list.begin(); it != f_list.end(); ++it){

            if(normal_toggle){
                glNormal3f((*it).normal.vx, (*it).normal.vy, (*it).normal.vz);
                glVertex3f(v_list[(*it).u].x, v_list[(*it).u].y, v_list[(*it).u].z);
                glVertex3f(v_list[(*it).v].x, v_list[(*it).v].y, v_list[(*it).v].z);
                glVertex3f(v_list[(*it).w].x, v_list[(*it).w].y, v_list[(*it).w].z);
            }else{
                glNormal3f(v_list[(*it).u].normal.vx, v_list[(*it).u].normal.vy, v_list[(*it).u].normal.vz);
                glVertex3f(v_list[(*it).u].x, v_list[(*it).u].y, v_list[(*it).u].z);
                glNormal3f(v_list[(*it).v].normal.vx, v_list[(*it).v].normal.vy, v_list[(*it).v].normal.vz);
                glVertex3f(v_list[(*it).v].x, v_list[(*it).v].y, v_list[(*it).v].z);
                glNormal3f(v_list[(*it).w].normal.vx, v_list[(*it).w].normal.vy, v_list[(*it).w].normal.vz);
                glVertex3f(v_list[(*it).w].x, v_list[(*it).w].y, v_list[(*it).w].z);
            }
        }
        glEnd();
        glDisable(GL_LIGHTING);
    }
};

class TransformNode: public BaseNode
{
public:
    TransformNode() { reset(); }
    ~TransformNode() {}

    void translate(float x, float y, float z){
        translate_vector[0] = x;
        translate_vector[1] = y;
        translate_vector[2] = z;
    }

    void rotate(float a){
        glRotatef(a, center[0], center[1], center[2]);
    }

    void reset(){
        for(int i = 0; i < 3; i++)
            center[i] = 0.0f;
        for(int i = 0; i < 3; i++)
            translate_vector[i] = 0.0f;
        for(int i = 0; i < 3; i++)
            scale_vector[i] = 1.0f;
        angle = 0.0f;
    }

    void update()
    {
        glPushMatrix();
        glTranslatef(translate_vector[0], translate_vector[1], translate_vector[2]);
        glScalef(scale_vector[0], scale_vector[1], scale_vector[2]);
        glRotatef(angle, center[0], center[1], center[2]);
        BaseNode::update();
        glPopMatrix();
    }
    
private:
    float center[3];
    float translate_vector[3];
    float scale_vector[3];
    float angle;
};


class CameraNode: public BaseNode
{
public:
    CameraNode() { reset(1.0f); }
    CameraNode(float s) { reset(s); }
    ~CameraNode() { }

    void update()
    {
        calculateUpVector();
        gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
        glScalef(zoom_scale, zoom_scale, zoom_scale);
    }

    void reset(float s)
    {
        eye[0] = eye[1] = 0;
        eye[2] = -1;

        for( int i = 0; i < 3; i++ )
            center[i] = 0;
        up[1] = 1;
        up[0] = up[2] = 0;
        zoom_scale = s;
        phi = 0;
        theta = 180;
        dp = 0;
        dt = 0;
    }

    void orbit(int t, int p){
        dt = t;
        dp = p;

        theta += t * ANGLE_VELOCITY;
        phi += p * ANGLE_VELOCITY;

        theta = fmod(theta, 360.0);
        phi = fmod(phi, 180.0);

        eye[0] = center[0] + cosf(phi*RADIAN) * sinf(theta*RADIAN);
        eye[1] = center[1] + sinf(phi*RADIAN) * sinf(theta*RADIAN);
        eye[2] = center[2] + cosf(theta*RADIAN);
    }

    int decelerate(int stop){
        if(dt != 0)
            dt -= (dt / abs(dt));
        if(dp != 0)
            dp -= (dp / abs(dp));

        orbit(dt, dp);

        return !(dt < 1 && dt > -1 && dp < 1 && dp > -1);
    }

    void poi(int x, int y){
        center[0] -= x * POI_VELOCITY;
        center[1] -= y * POI_VELOCITY;
    }

    void zoom(int x) {
        zoom_scale -= x * ZOOM_VELOCITY;
        zoom_scale = (zoom_scale <= 0.0f) ? 0.01 : zoom_scale;
    }

    void calculateUpVector(){
        float length = sqrt (eye[0] * eye[0] + eye[1] * eye[1] + eye[2] * eye[2]);

        Vector s = {eye[0]/length, eye[1]/length, eye[2]/length};
        Vector t = {0, 1, 0};

        Vector r = {
            s.vy * t.vz - s.vz * t.vy,
            s.vz * t.vx - s.vx * t.vz,
            s.vx * t.vy - s.vy * t.vx
        };

        up[0] = r.vy * s.vz - r.vz * s.vy;
        up[1] = r.vz * s.vx - r.vx * s.vz;
        up[2] = r.vx * s.vy - r.vy * s.vx;
    }

private:
    float eye[3];
    float center[3];
    float up[3];
    float zoom_scale;
    float phi, theta, dp, dt;
};

class LightNode: public BaseNode
{
public:
    LightNode() { reset(); }
    ~LightNode() { }

    void reset()
    {
        light = GL_LIGHT0;
        for( int i = 0; i < 3; i++ )
            position[i] = 0.0f;
        for( int i = 0; i < 3; i++ )
            diffuse[i] = 0.0f;
        for( int i = 0; i < 3; i++ )
            ambient[i] = 0.0f;
    }

    void update()
    {
        glLightfv(light, GL_POSITION, position);
        glLightfv(light, GL_DIFFUSE, diffuse);
        glLightfv(light, GL_AMBIENT, ambient);
        glEnable(light);
        glEnable(GL_COLOR_MATERIAL);
    }

    void enable(int l){
        light = l;
    }

    void initialize(float p[], float d[], float a[]){
        for( int i = 0; i < 3; i++ )
            position[i] = p[i];
        for( int i = 0; i < 3; i++ )
            diffuse[i] = d[i];
        for( int i = 0; i < 3; i++ )
            ambient[i] = a[i];
    }

    void updateDiffuse(float r, float g, float b){
        diffuse[0] += r;
        diffuse[1] += g;
        diffuse[2] += b;

        if(diffuse[0] <= 0.0f)
            diffuse[0] = 0.0f;
        if(diffuse[0] >= 1.0f)
            diffuse[0] = 1.0f;

        if(diffuse[1] <= 0.0f)
            diffuse[1] = 0.0f;
        if(diffuse[1] >= 1.0f)
            diffuse[1] = 1.0f;

        if(diffuse[2] <= 0.0f)
            diffuse[2] = 0.0f;
        if(diffuse[2] >= 1.0f)
            diffuse[2] = 1.0f;
    }

private:
    int light;
    float position[3];
    float diffuse[3];
    float ambient[3]; 
};

#endif
