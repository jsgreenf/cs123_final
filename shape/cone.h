#ifndef CONE_H
#define CONE_H
#include "shape2.h"
class cone : public shape
{
public:
    cone(int para1,int para2,bool drawNormals);
    virtual ~cone();
    void drawshape();

protected:
    void makearray();
    virtual Vector2 get_texture(Vector3 p_ins);

};

#endif // CONE_H
