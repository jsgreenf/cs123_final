#include "sphere.h"

#define _USE_MATH_DEFINES
#include <vector.h>
#include <math.h>
#include <iostream>

Sphere::Sphere(int numStacks, int numSlices)
{

    stacks = numStacks;
    slices = numSlices;

    surface = new QList<Vector3*>();
    normals = new QList<Vector3*>();
    textureCoords = new QList<Vector2*>();
    topNormal = new Vector3(0,1,0);
    baseNormal = new Vector3(0,-1,0);
    topCenter = new Vector3(0,.5,0);
    baseCenter = new Vector3(0,-.5,0);
    generatePoints();
}

Sphere::~Sphere(){

    for (int i=0; i<surface->size(); i++){
        delete surface->at(i);
    }
    delete surface;

    for (int i=0; i<normals->size(); i++){
        delete normals->at(i);
    }
    delete normals;

    for (int i=0; i<textureCoords->size(); i++){
        delete textureCoords->at(i);
    }
    delete textureCoords;

    delete topCenter;
    delete baseCenter;

}

// uses the implicit equation for the shape to determine if a ray intersects it and at what point, also sets corresponding normal
//float Sphere::intersectionPoint(Vector3 origin, Vector3 ray, Vector3 *normal, Vector2* texCoords){
//
//    // test for intersection with the sphere
//    float a = pow(ray.x,2) + pow(ray.y,2) + pow(ray.z,2);
//    float b = 2*((origin.x*ray.x) + (origin.y*ray.y) + (origin.z*ray.z));
//    float c = pow(origin.x,2) + pow(origin.y,2) + pow(origin.z,2) - .25;
//
//    float t1,t2;
//
//    // use determinant to short-circuit the intersection test if there are no intersections
//    float det = pow(b,2) - (4*a*c);
//
//    // if there is at least one intersection
//    if (det >= 0){
//
//        t1 = (-b + sqrt(pow(b,2)-(4*a*c)))/(2*a);
//        t2 = (-b - sqrt(pow(b,2)-(4*a*c)))/(2*a);
//
//    }
//    else{
//        return -1;
//    }
//
//    // return the smallest t-value
//    *normal = (ray*min(t1,t2)+origin); // assign corresponding normal
//    normal->w = 0; // ensure that the normal is a vector
//    *normal = normal->getNormalized();
//
//    // calculate texture coordinates
//    if (normal->y == 1 || normal->y == -1){ // handle the singularity
//        texCoords->x = .5;
//    }
//    else{
//        Vector3 coords = Vector3(normal->x,0,normal->z);
//        coords.normalize();
//        texCoords->x = acos(coords.x);
//        if (coords.z > 0) texCoords->x = 2*M_PI - texCoords->x;
//        texCoords->x /= 2*M_PI;
//    }
//    texCoords->y = ((M_PI/2) - asin(normal->y))/M_PI;
//
//    return min(t1,t2);
//}

void Sphere::generatePoints(){

    //if (checkParameters(2,3)){

        for (int i=0; i<surface->size(); i++){
            delete surface->at(i);
        }
        surface->clear();

        for (int i = 0; i<(stacks-1); i++){
            for (int j=0; j<slices; j++){
                float theta = 2*M_PI*j/(float)slices;
                float phi = M_PI*(i+1)/(float)stacks;
                surface->append(new Vector3(.5*cos(theta)*sin(phi), .5*cos(phi), .5*sin(theta)*sin(phi))); //face points, dont forget to add center points
                Vector3* normal = new Vector3(.5*cos(theta)*sin(phi), .5*cos(phi), .5*sin(theta)*sin(phi));
                normal->normalize();
                normals->append(normal);
                textureCoords->append(new Vector2(float(j)/slices, float(i)/stacks));
            }
        }
    //}
}

void Sphere::drawNormals(){

}

void Sphere::drawTriangles(){

    // make sure points to be drawn exist
    //generatePoints();

    glBegin(GL_TRIANGLES);

    // generate the triangles of the surface of the sphere
    for (int col=0; col<slices; col++){
        for (int row = 0; row<stacks-1; row++){

            int index1 = row*(slices) + col;
            int index2 = row*(slices) + ((col+1)%(int)slices);
            int index3 = (row+1)*(slices) + ((col+1)%(int)slices);
            int index4 = (row+1)*(slices) + col;

            // DRAW SURFACE

            // form first triangle
            glTexCoord2fv(textureCoords->at(index1)->xy);
            glNormal3fv(normals->at(index1)->xyz);
            glVertex3f(surface->at(index1)->x, surface->at(index1)->y, surface->at(index1)->z);

            if (index2 == row*slices) glTexCoord2f(1, textureCoords->at(index2)->y);
            else glTexCoord2fv(textureCoords->at(index2)->xy);
            glNormal3fv(normals->at(index2)->xyz);
            glVertex3f(surface->at(index2)->x, surface->at(index2)->y, surface->at(index2)->z);

            // unless connecting to the bottom tip, form both triangles
            if (row < (stacks-2)){
                // finish first triangle
                if (index3 == (row+1)*slices) glTexCoord2f(1, textureCoords->at(index3)->y);
                else glTexCoord2fv(textureCoords->at(index3)->xy);
                glNormal3fv(normals->at(index3)->xyz);
                glVertex3f(surface->at(index3)->x, surface->at(index3)->y, surface->at(index3)->z);

                // form second triangle
                glTexCoord2fv(textureCoords->at(index1)->xy);
                glNormal3fv(normals->at(index1)->xyz);
                glVertex3f(surface->at(index1)->x, surface->at(index1)->y, surface->at(index1)->z);
                if (index3 == (row+1)*slices) glTexCoord2f(1, textureCoords->at(index3)->y);
                else glTexCoord2fv(textureCoords->at(index3)->xy);
                glNormal3fv(normals->at(index3)->xyz);
                glVertex3f(surface->at(index3)->x, surface->at(index3)->y, surface->at(index3)->z);
                glTexCoord2fv(textureCoords->at(index4)->xy);
                glNormal3fv(normals->at(index4)->xyz);
                glVertex3f(surface->at(index4)->x, surface->at(index4)->y, surface->at(index4)->z);
            }
            else{
                // connect to the base center
                glTexCoord2f(float(col+.5)/slices, 1);
                glNormal3fv(baseNormal->xyz);
                glVertex3f(baseCenter->x, baseCenter->y, baseCenter->z);
            }

            // form top triangles
            if (row == 0){

                if (index2 == row*slices) glTexCoord2f(1, textureCoords->at(index2)->y);
                else glTexCoord2fv(textureCoords->at(index2)->xy);
                glNormal3fv(normals->at(index2)->xyz);
                glVertex3f(surface->at(index2)->x, surface->at(index2)->y, surface->at(index2)->z);
                glTexCoord2fv(textureCoords->at(index1)->xy);
                glNormal3fv(normals->at(index1)->xyz);
                glVertex3f(surface->at(index1)->x, surface->at(index1)->y, surface->at(index1)->z);
                glTexCoord2f(float(col+.5)/slices, 0);
                glNormal3fv(topNormal->xyz);
                glVertex3f(topCenter->x, topCenter->y, topCenter->z);
            }

        }
    }

    glEnd();

}
