#ifndef SHAPE_H
#define SHAPE_H
#include "vector.h"
#include <CS123Common.h>


class shape
{
public:
    shape(int para1,int para2,bool drawNormals);

    virtual ~shape();

    void setpara1(int para1);

    virtual void setpara2(int para2);

    virtual void drawshape() = 0;
    int m_para1;
    int m_para2;
    double (*m_coordinates)[4]; //array to store points data


protected:
    virtual void makearray() = 0;





    bool m_drawNormals;

};

#endif // SHAPE_H
