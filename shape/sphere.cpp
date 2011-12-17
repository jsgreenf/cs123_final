#include "sphere.h"

#include "shape.h"
#include <stdio.h>
#include <math.h>
#include <iostream>

#include <qgl.h>

#define PI 3.141592654


sphere::sphere(int para1,int para2,bool drawNormals):shape(para1,para2,drawNormals)
{
    int size = 6*m_para2*(m_para1 -1);
         m_coordinates = new double[size][4];
         m_textures = new double[size][2];
         makearray();
}


sphere::~sphere(){
    delete[] m_coordinates;

}

void sphere::makearray(){
    if(m_para2 <3){
        m_para2 = 3;
    }
    if(m_para1 <2){
        m_para1 = 2;
    }
    int size = 6*m_para2*(m_para1 -1);
    delete[] m_coordinates;
    m_coordinates = new double[size][4];

    //horizontal array stores all the theta value, while the vertical_array stores all the phi value.

    double horizontal_array[m_para2];
    double theta = 0;
    for (int i=0;i<m_para2+1;i++){
        horizontal_array[i] = theta;
        theta += 2*PI/m_para2;

    }

    double vertical_array[m_para1-1];
    double phi =0;
    for (int i=0;i<m_para1-1;i++){
        phi += PI/m_para1;
        vertical_array[i] = phi;
    }
    if (m_para1 >2){
        double inner_p1[4];
        double inner_p2[4];
        double outer_p1[4];
        double outer_p2[4];
        for (int i=0;i<m_para2;i++){

            for(int j=0;j<m_para1-2;j++){


                outer_p1[0] = 0.5* sin(vertical_array[j])*cos(horizontal_array[i]);
                outer_p2[0] = 0.5* sin(vertical_array[j])*cos(horizontal_array[i+1]);
                inner_p1[0] = 0.5* sin(vertical_array[j+1])*cos(horizontal_array[i]);
                inner_p2[0] = 0.5* sin(vertical_array[j+1])*cos(horizontal_array[i+1]);
                outer_p1[1] = 0.5*cos(vertical_array[j]);
                outer_p2[1] = 0.5*cos(vertical_array[j]);
                inner_p1[1] = 0.5*cos(vertical_array[j+1]);
                inner_p2[1] = 0.5*cos(vertical_array[j+1]);
                outer_p1[2] = 0.5* sin(vertical_array[j])*sin(horizontal_array[i]);
                outer_p2[2] = 0.5* sin(vertical_array[j])*sin(horizontal_array[i+1]);
                inner_p1[2] = 0.5* sin(vertical_array[j+1])*sin(horizontal_array[i]);
                inner_p2[2] = 0.5* sin(vertical_array[j+1])*sin(horizontal_array[i+1]);
                outer_p1[3] = 1.0;
                outer_p2[3] = 1.0;
                inner_p1[3] = 1.0;
                inner_p2[3] = 1.0;
                for (int k=0;k<4;k++){
                    m_coordinates[i*6*(m_para1 -2)+6*j][k] = outer_p1[k];
                    m_coordinates[i*6*(m_para1 -2)+6*j+1][k] = outer_p2[k];
                    m_coordinates[i*6*(m_para1 -2)+6*j+2][k] = inner_p1[k];
                    m_coordinates[i*6*(m_para1 -2)+6*j+3][k] = inner_p1[k];
                    m_coordinates[i*6*(m_para1 -2)+6*j+4][k] = outer_p2[k];
                    m_coordinates[i*6*(m_para1 -2)+6*j+5][k] = inner_p2[k];
                }


            }
        }

    }
    for(int i=0;i<m_para2;i++){


        m_coordinates[6*m_para2*(m_para1 -2)+6*i][0] = 0.0;
        m_coordinates[6*m_para2*(m_para1 -2)+6*i+1][0] =0.5* sin(PI/m_para1)*cos(horizontal_array[i+1]);
        m_coordinates[6*m_para2*(m_para1 -2)+6*i+2][0] =  0.5* sin(PI/m_para1)*cos(horizontal_array[i]);
        m_coordinates[6*m_para2*(m_para1 -2)+6*i][1] = 0.5;
        m_coordinates[6*m_para2*(m_para1 -2)+6*i+1][1] = 0.5*cos(PI/m_para1);
        m_coordinates[6*m_para2*(m_para1 -2)+6*i+2][1] = 0.5*cos(PI/m_para1);
        m_coordinates[6*m_para2*(m_para1 -2)+6*i][2] = 0.0;
        m_coordinates[6*m_para2*(m_para1 -2)+6*i+1][2] = 0.5* sin(PI/m_para1)*sin(horizontal_array[i+1]);
        m_coordinates[6*m_para2*(m_para1 -2)+6*i+2][2] = 0.5* sin(PI/m_para1)*sin(horizontal_array[i]);
        m_coordinates[6*m_para2*(m_para1 -2)+6*i+3][0] = 0.0;
        m_coordinates[6*m_para2*(m_para1 -2)+6*i+4][0] = 0.5* sin(PI -(PI/m_para1))*cos(horizontal_array[i]);
        m_coordinates[6*m_para2*(m_para1 -2)+6*i+5][0] = 0.5* sin(PI -(PI/m_para1))*cos(horizontal_array[i+1]);
        m_coordinates[6*m_para2*(m_para1 -2)+6*i+3][1] = -0.5;
        m_coordinates[6*m_para2*(m_para1 -2)+6*i+4][1] = 0.5*cos(PI -(PI/m_para1));
        m_coordinates[6*m_para2*(m_para1 -2)+6*i+5][1] = 0.5*cos(PI -(PI/m_para1));
        m_coordinates[6*m_para2*(m_para1 -2)+6*i+3][2] = 0.0;
        m_coordinates[6*m_para2*(m_para1 -2)+6*i+4][2] = 0.5* sin(PI -(PI/m_para1))*sin(horizontal_array[i]);
        m_coordinates[6*m_para2*(m_para1 -2)+6*i+5][2] = 0.5* sin(PI -(PI/m_para1))*sin(horizontal_array[i+1]);


    }
    for(int i =0;i<2*m_para2*(m_para1 -1);i++){
        Vector2 a = get_texture(Vector3 (m_coordinates[i*3][0],m_coordinates[i*3][1],m_coordinates[i*3][2]));
        m_textures[i*3][0] = a.x;
        m_textures[i*3][1] = a.y;
        a = get_texture(Vector3 (m_coordinates[i*3+1][0],m_coordinates[i*3+1][1],m_coordinates[i*3+1][2]));
        m_textures[i*3+1][0] = a.x;
        m_textures[i*3+1][1] = a.y;
        a = get_texture(Vector3 (m_coordinates[i*3+2][0],m_coordinates[i*3+2][1],m_coordinates[i*3+2][2]));
        m_textures[i*3+2][0] = a.x;
        m_textures[i*3+2][1] = a.y;
    }


}
void sphere::drawshape(){

    for(int i =0;i<2*m_para2*(m_para1 -3);i++){
        glBegin(GL_TRIANGLES);
        glNormal3f(2*m_coordinates[i*3][0],2*m_coordinates[i*3][1],2* m_coordinates[3*i][2]);

        glTexCoord2f(m_textures[i*3][0],m_textures[i*3][1]);
        glVertex3f(m_coordinates[i*3][0],m_coordinates[i*3][1], m_coordinates[3*i][2]);
        glNormal3f(2*m_coordinates[i*3+1][0],2*m_coordinates[i*3+1][1],2* m_coordinates[3*i+1][2]);

        glTexCoord2f(m_textures[i*3+1][0],m_textures[i*3+1][1]);
        glVertex3f(m_coordinates[3*i+1][0],m_coordinates[3*i+1][1], m_coordinates[3*i+1][2]);
        glNormal3f(2*m_coordinates[i*3+2][0],2*m_coordinates[i*3+2][1],2* m_coordinates[3*i+2][2]);

        glTexCoord2f(m_textures[i*3+2][0],m_textures[i*3+2][1]);
        glVertex3f(m_coordinates[3*i+2][0],m_coordinates[3*i+2][1], m_coordinates[3*i+2][2]);

        glEnd();
    }
    for(int i =2*m_para2*(m_para1 -3);i<2*m_para2*(m_para1 -1);i++){
        glBegin(GL_TRIANGLES);
        glNormal3f(2*m_coordinates[i*3][0],2*m_coordinates[i*3][1],2* m_coordinates[3*i][2]);

        if(m_textures[i*3][0]<0.01){
            m_textures[i*3][0] =1 -m_textures[i*3][0];
        }
        glTexCoord2f(m_textures[i*3][0],m_textures[i*3][1]);


//        std::cout<<a.x<<std::endl;



        glVertex3f(m_coordinates[i*3][0],m_coordinates[i*3][1], m_coordinates[3*i][2]);
        glNormal3f(2*m_coordinates[i*3+1][0],2*m_coordinates[i*3+1][1],2* m_coordinates[3*i+1][2]);

        if(m_textures[i*3+1][0]<0.01){
            m_textures[i*3+1][0] =1 -m_textures[i*3+1][0];
        }
        glTexCoord2f(m_textures[i*3+1][0],m_textures[i*3+1][1]);

//        std::cout<<a.x<<std::endl;



        glVertex3f(m_coordinates[3*i+1][0],m_coordinates[3*i+1][1], m_coordinates[3*i+1][2]);
        glNormal3f(2*m_coordinates[i*3+2][0],2*m_coordinates[i*3+2][1],2* m_coordinates[3*i+2][2]);

//        std::cout<<a.x<<std::endl;

//       std::cout<<"ha"<<std::endl;

        if(m_textures[i*3+2][0]<0.01){
            m_textures[i*3+2][0] =1 -m_textures[i*3+2][0];
        }
        glTexCoord2f(m_textures[i*3+2][0],m_textures[i*3+2][1]);
        glVertex3f(m_coordinates[3*i+2][0],m_coordinates[3*i+2][1], m_coordinates[3*i+2][2]);

        glEnd();
    }



}
Vector2 sphere::get_texture(Vector3 p_ins){
    double u,v;
    if((p_ins.y >= 0.5 -EPSILON)&&(p_ins.y<=0.5+EPSILON)){

        return Vector2(0.5, 0);
    }
    else if((p_ins.y <= -0.5+EPSILON)&&(p_ins.y>= -0.5-EPSILON)){

        return Vector2(0.5 ,1);
    }
    else{

        double theta;
        double fy;

        theta = atan2(p_ins.z,p_ins.x);
        fy = asin(p_ins.y/0.5);
        v = 0.5 - (fy/M_PI);
        if (theta <0){

            u = (-theta)/(2*M_PI);


        }
        else{

            u = 1- (theta/(2*M_PI));


        }



        return Vector2(u,v);
    }

}

