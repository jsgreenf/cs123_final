#include "shape.h"

shape::shape(int para1,int para2,bool drawNormals)
{   m_para1 = para1;
    m_para2 = para2;

    m_drawNormals = drawNormals;

}
shape::~shape(){

}
void shape::setpara1(int para1){
    m_para1 = para1;
    makearray();
}
void shape::setpara2(int para2){
    m_para2 = para2;
    makearray();
}

void shape::drawshape(){

}
