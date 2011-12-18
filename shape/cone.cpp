#include "cone.h"
#include "shape.h"
#include <stdio.h>
#include <math.h>
#include <iostream>

#include <qgl.h>

#define PI 3.141592654

cone::cone(int para1,int para2,bool drawNormals):shape(para1,para2,drawNormals)
{    int size = 6*m_para2*(2*m_para1 -1);
     m_coordinates = new double[size][4];
     makearray();

}
cone::~cone(){
    delete[] m_coordinates;
    delete[] m_textures;

}
void cone::makearray(){
    if(m_para2 <3){
        m_para2 = 3;
    }
    int size = 6*m_para2*(2*m_para1 -1);
    delete[] m_coordinates;
    m_coordinates = new double[size][4];
    m_textures = new double[size][2];
    double boundry_array[m_para2][4];
    double theta = 0;

    //The boundray_array stores the points in the biggest circle.

    for(int i=0;i<m_para2;i++){
        boundry_array[i][0] = 0.5* cos(theta);
        boundry_array[i][1] = 0.5;
        boundry_array[i][2] = 0.5* sin(theta);
        boundry_array[i][3] = 1;
        theta += 2*PI/m_para2;
    }
    if (m_para1 >1){
        double inner_p1[4];
        double inner_p2[4];
        double outer_p1[4];
        double outer_p2[4];
        for (int i=0;i<m_para2;i++){

            for(int j=0;j<m_para1-1;j++){


                outer_p1[0] = boundry_array[i][0]*double(m_para1 -j)/m_para1;
                outer_p2[0] = boundry_array[(i+1)%m_para2][0]*double(m_para1 -j)/m_para1;
                inner_p1[0] = boundry_array[i][0]*double(m_para1-j-1)/m_para1;
                inner_p2[0] = boundry_array[(i+1)%m_para2][0]*double(m_para1-j-1)/m_para1;
                outer_p1[1] = 0.5;
                outer_p2[1] = 0.5;
                inner_p1[1] = 0.5;
                inner_p2[1] = 0.5;
                outer_p1[2] = boundry_array[i][2]*double(m_para1 -j)/m_para1;
                outer_p2[2] = boundry_array[(i+1)%m_para2][2]*double(m_para1 -j)/m_para1;
                inner_p1[2] = boundry_array[i][2]*double(m_para1-j-1)/m_para1;
                inner_p2[2] = boundry_array[(i+1)%m_para2][2]*double(m_para1-j-1)/m_para1;
                outer_p1[3] = 1.0;
                outer_p2[3] = 1.0;
                inner_p1[3] = 1.0;
                inner_p2[3] = 1.0;
                for (int k=0;k<4;k++){
                    m_coordinates[i*6*(m_para1 -1)+6*j][k] = outer_p1[k];
                    m_coordinates[i*6*(m_para1 -1)+6*j+1][k] = outer_p2[k];
                    m_coordinates[i*6*(m_para1 -1)+6*j+2][k] = inner_p1[k];
                    m_coordinates[i*6*(m_para1 -1)+6*j+3][k] = inner_p1[k];
                    m_coordinates[i*6*(m_para1 -1)+6*j+4][k] = outer_p2[k];
                    m_coordinates[i*6*(m_para1 -1)+6*j+5][k] = inner_p2[k];
                }


            }
        }

    }
    if (m_para1 >1){
        double inner_p1[4];
        double inner_p2[4];
        double outer_p1[4];
        double outer_p2[4];
        for (int i=0;i<m_para2;i++){

            for(int j=0;j<m_para1-1;j++){


                outer_p1[0] = boundry_array[i][0]*double(m_para1 -j)/m_para1;
                outer_p2[0] = boundry_array[(i+1)%m_para2][0]*double(m_para1 -j)/m_para1;
                inner_p1[0] = boundry_array[i][0]*double(m_para1-j-1)/m_para1;
                inner_p2[0] = boundry_array[(i+1)%m_para2][0]*double(m_para1-j-1)/m_para1;
                outer_p1[1] = -0.5 + double(j)/m_para1;
                outer_p2[1] = -0.5 + double(j)/m_para1;
                inner_p1[1] = -0.5 + double(j+1.0)/m_para1;
                inner_p2[1] = -0.5 + double(j+1.0)/m_para1;
                outer_p1[2] = boundry_array[i][2]*double(m_para1 -j)/m_para1;
                outer_p2[2] = boundry_array[(i+1)%m_para2][2]*double(m_para1 -j)/m_para1;
                inner_p1[2] = boundry_array[i][2]*double(m_para1-j-1)/m_para1;
                inner_p2[2] = boundry_array[(i+1)%m_para2][2]*double(m_para1-j-1)/m_para1;
                outer_p1[3] = 1.0;
                outer_p2[3] = 1.0;
                inner_p1[3] = 1.0;
                inner_p2[3] = 1.0;
                for (int k=0;k<4;k++){
                    m_coordinates[3*m_para2*(2*m_para1 -1) + i*6*(m_para1 -1)+6*j][k] = outer_p1[k];
                    m_coordinates[3*m_para2*(2*m_para1 -1) + i*6*(m_para1 -1)+6*j+1][k] = inner_p1[k];
                    m_coordinates[3*m_para2*(2*m_para1 -1) + i*6*(m_para1 -1)+6*j+2][k] = outer_p2[k];
                    m_coordinates[3*m_para2*(2*m_para1 -1) + i*6*(m_para1 -1)+6*j+3][k] = outer_p2[k];
                    m_coordinates[3*m_para2*(2*m_para1 -1) + i*6*(m_para1 -1)+6*j+4][k] = inner_p1[k];
                    m_coordinates[3*m_para2*(2*m_para1 -1) + i*6*(m_para1 -1)+6*j+5][k] = inner_p2[k];
                }


            }
        }

    }
    for(int i=0;i<m_para2;i++){


        m_coordinates[m_para2*6*(m_para1 -1)+3*i][0] = 0.0;
        m_coordinates[m_para2*6*(m_para1 -1)+3*i+1][0] = boundry_array[i][0]*1.0/m_para1;
        m_coordinates[m_para2*6*(m_para1 -1)+3*i+2][0] = boundry_array[(i+1)%m_para2][0]*1.0/m_para1;
        m_coordinates[m_para2*6*(m_para1 -1)+3*i][1] = -0.5;
        m_coordinates[m_para2*6*(m_para1 -1)+3*i+1][1] = -0.5;
        m_coordinates[m_para2*6*(m_para1 -1)+3*i+2][1] = -0.5;
        m_coordinates[m_para2*6*(m_para1 -1)+3*i][2] = 0.0;
        m_coordinates[m_para2*6*(m_para1 -1)+3*i+1][2] = boundry_array[i][2]*1.0/m_para1;
        m_coordinates[m_para2*6*(m_para1 -1)+3*i+2][2] = boundry_array[(i+1)%m_para2][2]*1.0/m_para1;
        m_coordinates[3*m_para2*(2*m_para1 -1) + m_para2*6*(m_para1 -1)+3*i][0] = 0.0;
        m_coordinates[3*m_para2*(2*m_para1 -1) +m_para2*6*(m_para1 -1)+3*i+1][0] = boundry_array[(i+1)%m_para2][0]*1.0/m_para1;
        m_coordinates[3*m_para2*(2*m_para1 -1) +m_para2*6*(m_para1 -1)+3*i+2][0] = boundry_array[i][0]*1.0/m_para1;
        m_coordinates[3*m_para2*(2*m_para1 -1) +m_para2*6*(m_para1 -1)+3*i][1] = 0.5;
        m_coordinates[3*m_para2*(2*m_para1 -1) +m_para2*6*(m_para1 -1)+3*i+1][1] = 0.5 - 1.0/m_para1;
        m_coordinates[3*m_para2*(2*m_para1 -1) +m_para2*6*(m_para1 -1)+3*i+2][1] = 0.5 - 1.0/m_para1;
        m_coordinates[3*m_para2*(2*m_para1 -1) +m_para2*6*(m_para1 -1)+3*i][2] = 0.0;
        m_coordinates[3*m_para2*(2*m_para1 -1) +m_para2*6*(m_para1 -1)+3*i+1][2] = boundry_array[(i+1)%m_para2][2]*1.0/m_para1;
        m_coordinates[3*m_para2*(2*m_para1 -1) +m_para2*6*(m_para1 -1)+3*i+2][2] = boundry_array[i][2]*1.0/m_para1;


    }
    for(int i =0;i<m_para2*(2*m_para1 -1);i++){

        Vector2 a = get_texture(Vector3(m_coordinates[i*3][0],-0.5, m_coordinates[3*i][2]));
        m_textures[i*3][0] = a.x;
        m_textures[i*3][1] = a.y;
        a = get_texture(Vector3(m_coordinates[i*3+1][0],-0.5, m_coordinates[3*i+1][2]));
        m_textures[i*3+1][0] = a.x;
        m_textures[i*3+1][1] = a.y;
        a = get_texture(Vector3(m_coordinates[i*3+2][0],-0.5, m_coordinates[3*i+2][2]));
        m_textures[i*3+2][0] = a.x;
        m_textures[i*3+2][1] = a.y;


    }

    for(int i =0;i<m_para2*(2*m_para1 -1);i++){
        Vector2 a = get_texture(Vector3(m_coordinates[3*m_para2*(2*m_para1 -1)+i*3][0],m_coordinates[3*m_para2*(2*m_para1 -1)+i*3][1], m_coordinates[3*m_para2*(2*m_para1 -1)+3*i][2]));
        m_textures[3*m_para2*(2*m_para1 -1)+i*3][0] = a.x;
        m_textures[3*m_para2*(2*m_para1 -1)+i*3][1] = a.y;
        a = get_texture(Vector3(m_coordinates[3*m_para2*(2*m_para1 -1)+3*i+1][0],m_coordinates[3*m_para2*(2*m_para1 -1)+i*3+1][1], m_coordinates[3*m_para2*(2*m_para1 -1)+3*i+1][2]));
        m_textures[3*m_para2*(2*m_para1 -1)+i*3+1][0] = a.x;
        m_textures[3*m_para2*(2*m_para1 -1)+i*3+1][1] = a.y;
        a = get_texture(Vector3(m_coordinates[3*m_para2*(2*m_para1 -1)+3*i+2][0],m_coordinates[3*m_para2*(2*m_para1 -1)+i*3+2][1], m_coordinates[3*m_para2*(2*m_para1 -1)+3*i+2][2]));
        m_textures[3*m_para2*(2*m_para1 -1)+i*3+2][0] = a.x;
        m_textures[3*m_para2*(2*m_para1 -1)+i*3+2][1] = a.y;




    }

}
void cone::drawshape(){

    for(int i =0;i<m_para2*(2*m_para1 -1);i++){
        glBegin(GL_TRIANGLES);
        glNormal3f(0,-1 ,0);
        glTexCoord2f(m_textures[i*3][0],m_textures[i*3][1]);
        glVertex3f(m_coordinates[i*3][0],-0.5, m_coordinates[3*i][2]);
        glTexCoord2f(m_textures[i*3+1][0],m_textures[i*3+1][1]);
        glVertex3f(m_coordinates[3*i+1][0],-0.5, m_coordinates[3*i+1][2]);
        glTexCoord2f(m_textures[i*3+2][0],m_textures[i*3+2][1]);
        glVertex3f(m_coordinates[3*i+2][0],-0.5, m_coordinates[3*i+2][2]);

        glEnd();
    }
    double l;
    for(int i =0;i<m_para2*(2*m_para1 -1);i++){
        glBegin(GL_TRIANGLES);

        l = sqrt(5*(m_coordinates[3*m_para2*(2*m_para1 -1)+i*3][0]*m_coordinates[3*m_para2*(2*m_para1 -1)+i*3][0]+ m_coordinates[3*m_para2*(2*m_para1 -1)+3*i][2]* m_coordinates[3*m_para2*(2*m_para1 -1)+3*i][2]));
        glNormal3f(2*m_coordinates[3*m_para2*(2*m_para1 -1)+i*3][0]/l,1/sqrt(5),2*m_coordinates[3*m_para2*(2*m_para1 -1)+3*i][2]/l);
        glTexCoord2f(m_textures[3*m_para2*(2*m_para1 -1)+i*3][0],m_textures[3*m_para2*(2*m_para1 -1)+i*3][1]);
        glVertex3f(m_coordinates[3*m_para2*(2*m_para1 -1)+i*3][0],m_coordinates[3*m_para2*(2*m_para1 -1)+i*3][1], m_coordinates[3*m_para2*(2*m_para1 -1)+3*i][2]);
        l = sqrt(5*(m_coordinates[3*m_para2*(2*m_para1 -1)+i*3+1][0]*m_coordinates[3*m_para2*(2*m_para1 -1)+i*3+1][0]+ m_coordinates[3*m_para2*(2*m_para1 -1)+3*i+1][2]* m_coordinates[3*m_para2*(2*m_para1 -1)+3*i+1][2]));
        glNormal3f(2*m_coordinates[3*m_para2*(2*m_para1 -1)+i*3+1][0]/l,1/sqrt(5),2*m_coordinates[3*m_para2*(2*m_para1 -1)+3*i+1][2]/l);
        glTexCoord2f(m_textures[3*m_para2*(2*m_para1 -1)+3*i+1][0],m_textures[3*m_para2*(2*m_para1 -1)+3*i+1][1]);
        glVertex3f(m_coordinates[3*m_para2*(2*m_para1 -1)+3*i+1][0],m_coordinates[3*m_para2*(2*m_para1 -1)+i*3+1][1], m_coordinates[3*m_para2*(2*m_para1 -1)+3*i+1][2]);
        l = sqrt(5*(m_coordinates[3*m_para2*(2*m_para1 -1)+i*3+2][0]*m_coordinates[3*m_para2*(2*m_para1 -1)+i*3+2][0]+ m_coordinates[3*m_para2*(2*m_para1 -1)+3*i+2][2]* m_coordinates[3*m_para2*(2*m_para1 -1)+3*i+2][2]));
        glNormal3f(2*m_coordinates[3*m_para2*(2*m_para1 -1)+i*3+2][0]/l,1/sqrt(5),2*m_coordinates[3*m_para2*(2*m_para1 -1)+3*i+2][2]/l);
        glTexCoord2f(m_textures[3*m_para2*(2*m_para1 -1)+3*i+2][0],m_textures[3*m_para2*(2*m_para1 -1)+3*i+2][1]);
        glVertex3f(m_coordinates[3*m_para2*(2*m_para1 -1)+3*i+2][0],m_coordinates[3*m_para2*(2*m_para1 -1)+i*3+2][1], m_coordinates[3*m_para2*(2*m_para1 -1)+3*i+2][2]);


        glEnd();
    }

}
Vector2 cone::get_texture(Vector3 p_ins){
    double u,v;
    if((p_ins.y <= -0.5+EPSILON)&&(p_ins.y>= -0.5-EPSILON)){


        return Vector2(p_ins.x+ 0.5 , 0.5 - p_ins.z);
    }
    else if((p_ins.y >= 0.5 -EPSILON)&&(p_ins.y<=0.5+EPSILON)){

        return Vector2(0.5, 0);
    }
    else{
        double theta;
        theta = atan2(p_ins.z,p_ins.x);
        v = 0.5-p_ins.y;
        if (p_ins.z <0){

            u = (-theta)/(2*M_PI);


        }
        else{

            u = 1- (theta/(2*M_PI));


        }
        return Vector2(u,v);
    }

}
