#ifndef SPHERE_H
#define SPHERE_H

#include "shape.h"

class Sphere: public Shape
{
public:
    Sphere(int numStacks, int numSlices);
    virtual ~Sphere();

    void generatePoints();
    void drawNormals();
    void drawTriangles();

    // uses the implicit equation for the shape to determine if a ray intersects it and at what point, also sets corresponding normal
    //float intersectionPoint(Vector3 origin, Vector3 ray, Vector3 *normal, Vector2* texCoords);

private:

    QList<Vector3*>* surface;
    QList<Vector3*>* normals;
    QList<Vector2*>* textureCoords;

    Vector3* topNormal;
    Vector3* baseNormal;
    Vector3* topCenter;
    Vector3* baseCenter;

};

#endif // SPHERE_H
