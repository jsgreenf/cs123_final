#ifndef SPHERE_H
#define SPHERE_H
#include "shape.h"
class sphere : public shape
{
public:
    sphere(int para1,int para2,bool drawNormals);
    virtual ~sphere();
    void drawshape();
    virtual Vector2 get_texture(Vector3 p_ins);
protected:
    void makearray();

};

#endif // SPHERE_H
