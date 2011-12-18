#include "shape.h"

Shape::Shape()
{
    stacks = 0;
    slices = 0;
    param3 = 0;
}

Shape::~Shape()
{

}

// checks if shape parameters have changed so that the shape points can be recalculated
//bool Shape::checkParameters(int p1, int p2, float p3)
//{
//    bool changed = false;
//    if (stacks != max(settings.shapeParameter1, p1))
//    {
//        stacks = max(settings.shapeParameter1,p1);
//        changed = true;
//    }
//    if (slices != max(settings.shapeParameter2, p2))
//    {
//        slices = max(settings.shapeParameter2, p2);
//        changed = true;
//    }
//    if (param3 != max((float)settings.shapeParameter3, p3))
//    {
//        param3 = max((float)settings.shapeParameter3, p3);
//        changed = true;
//    }
//    return changed;
//}
