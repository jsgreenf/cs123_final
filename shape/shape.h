#ifndef SHAPE_H
#define SHAPE_H

#include <QList>
#include "vector.h"
#include <math.h>
#include <qgl.h>

class Shape
{
public:
    Shape();
    virtual ~Shape();

    // generates the points that make up a certain shape
    virtual void generatePoints() = 0;

    // generates the normals for the shape's points
    virtual void drawNormals() = 0;

    // draws the shape's triangle tesselation to the screen using openGL
    virtual void drawTriangles() = 0;

    // uses the implicit equation for the shape to determine if a ray intersects it and at what point, also sets corresponding normal
    //virtual float intersectionPoint(Vector3 origin, Vector3 ray, Vector3 *normal, Vector2* texCoords) = 0;

    int stacks;
    int slices;

protected:

    float param3;
    //bool checkParameters(int p1 = 1, int p2 = 1, float p3 = 1);

private:



};

#endif // SHAPE_H
