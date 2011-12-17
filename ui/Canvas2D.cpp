/**

  CS123 2-dimensional canvas. Contains support code necessary for Brush,
  Filter, Intersect, and Ray.

  YOU WILL NEED TO FILL THIS IN!

  Please contact the authors if there are any bugs.

  @date   2010-08-24
  @author Evan Wallace (edwallac)
  @author Ben Herila (ben@herila.net)

**/

// For your convenience, a few headers are included for you.
#include <math.h>
#include <assert.h>
#include "Canvas2D.h"
#include "Canvas3D.h"
#include "Settings.h"
#include "RayScene.h"
#include <QCoreApplication>
#include <Camera.h>
#include <QPainter>
#include "brush/Brush.h"
#include "brush/ConstantBrush.h"
#include "brush/LinearBrush.h"
#include "brush/QuadraticBrush.h"
#include "brush/SmudgeBrush.h"
#include "filter/invert.h"
#include "filter/grayscale.h"
#include "filter/sobel.h"
#include "filter/blur.h"
#include "filter/scale.h"
#include "filter/median.h"
#include "mythread.h"
Canvas2D::Canvas2D()
{
    // @TODO: Initialize any pointers in this class here.
    m_scene = NULL;

    int currentRadius = settings.brushRadius;
    BGRA currentColor;
    currentColor.b = settings.brushBlue;
    currentColor.g = settings.brushGreen;
    currentColor.r = settings.brushRed;
    int currentFlow = settings.brushAlpha;


    m_constantbrush = new ConstantBrush(currentColor,currentFlow,currentRadius);
    m_linearbrush = new LinearBrush(currentColor,currentFlow,currentRadius);
    m_quadraticbrush = new QuadraticBrush(currentColor,currentFlow,currentRadius);
    m_smudgebrush = new SmudgeBrush(currentColor,currentFlow,currentRadius);

    m_invert = new invert();
    m_grayscale = new grayscale();
    m_sobel = new sobel();
    m_blur = new blur();
    m_scale = new scale();
    m_median = new median();



}

Canvas2D::~Canvas2D()
{
    // @TODO: Be sure to release all memory that you allocate.
    delete m_constantbrush;
    delete m_linearbrush;
    delete m_quadraticbrush;
    delete m_smudgebrush;
    delete m_scene;
    delete m_invert;
    delete m_grayscale;
    delete m_sobel;
    delete m_blur;
    delete m_scale;
    delete m_median;
}

// This is called when the canvas size is changed. You can change the canvas size
// by calling resize(...). You probably won't need to fill this in, but you can
// if you want to.
void Canvas2D::notifySizeChanged(int w, int h) {

}

void Canvas2D::paintEvent(QPaintEvent *e) {
    // You probably won't need to fill this in, but you can if you want to override
    // any painting events for the 2D canvas. For now, we simply call the superclass.

    SupportCanvas2D::paintEvent(e);

}

// *****************************************************************************************
// ** BRUSH
// *****************************************************************************************

void Canvas2D::mouseDown(int x, int y)
{
    // @TODO: [BRUSH] Mouse interaction for brush. You will probably want to create a
    //        separate class for each of your brushes. Remember that you can use the
    //        static Settings object to get the currently selected brush and its parameters.

    int currentBrush = settings.brushType;
    int currentRadius = settings.brushRadius;

    BGRA currentColor;

    currentColor.b = settings.brushBlue;

    currentColor.g = settings.brushGreen;
    currentColor.r = settings.brushRed;
    currentColor.a = 255;
    int oldRadius = -1;
    int oldFlow = -1;


    int currentFlow = settings.brushAlpha;

    if (currentBrush == 0){
        m_constantbrush ->setBlue(currentColor.b);
        m_constantbrush ->setGreen(currentColor.g);
        m_constantbrush ->setRed(currentColor.r);
        if (oldRadius != currentRadius){

            m_constantbrush ->setRadius(currentRadius);
             oldRadius = currentRadius;


        }
        if (oldFlow != currentFlow){
            m_constantbrush ->setFlow(currentFlow);
             oldFlow = currentFlow;

        }

        m_constantbrush ->paintOnce(x,y,this);

    }
    if (currentBrush == 1){
        m_linearbrush ->setBlue(currentColor.b);
        m_linearbrush ->setGreen(currentColor.g);
        m_linearbrush ->setRed(currentColor.r);
        if (oldRadius != currentRadius){

            m_linearbrush ->setRadius(currentRadius);
             oldRadius = currentRadius;


        }
        if (oldFlow != currentFlow){
            m_linearbrush ->setFlow(currentFlow);
             oldFlow = currentFlow;

        }

        m_linearbrush ->paintOnce(x,y,this);

    }
    if (currentBrush == 2){
        m_quadraticbrush ->setBlue(currentColor.b);
        m_quadraticbrush ->setGreen(currentColor.g);
        m_quadraticbrush ->setRed(currentColor.r);
        if (oldRadius != currentRadius){

            m_quadraticbrush ->setRadius(currentRadius);
             oldRadius = currentRadius;


        }
        if (oldFlow != currentFlow){
            m_quadraticbrush ->setFlow(currentFlow);
             oldFlow = currentFlow;

        }

        m_quadraticbrush ->paintOnce(x,y,this);

    }
    if (currentBrush == 3){

        if (oldRadius != currentRadius){

            m_smudgebrush ->setRadius(currentRadius);
             oldRadius = currentRadius;


        }
        if (oldFlow != currentFlow){
            m_smudgebrush ->setFlow(currentFlow);
             oldFlow = currentFlow;

        }


        m_smudgebrush->pickUpPaint(x,y,this);

    }



    // you're going to need to leave the alpha value on the canvas itself at 255, but
    // you will need to use the actual alpha value to compute the new color of the pixel

}

void Canvas2D::mouseDragged(int x, int y)
{
    // TODO: [BRUSH] Mouse interaction for Brush.

    int currentBrush = settings.brushType;


    if (currentBrush == 0){

        m_constantbrush ->paintOnce(x,y,this);

    }
    if (currentBrush == 1){

        m_linearbrush ->paintOnce(x,y,this);

    }
    if (currentBrush == 2){

        m_quadraticbrush ->paintOnce(x,y,this);

    }
    if (currentBrush == 3){

        m_smudgebrush ->paintOnce(x,y,this);


    }

}

void Canvas2D::mouseUp(int x, int y)
{
    // TODO: [BRUSH] Mouse interaction for Brush.
    int currentBrush = settings.brushType;
    if (currentBrush == 0){

        m_constantbrush ->paintOnce(x,y,this);

    }
    if (currentBrush == 1){

        m_linearbrush ->paintOnce(x,y,this);

    }
    if (currentBrush == 2){

        m_quadraticbrush ->paintOnce(x,y,this);

    }
//    if (currentBrush == 3){

//        m_smudgebrush ->paintOnce(x,y,this);

//    }

}



// *****************************************************************************************
// ** FILTER
// *****************************************************************************************

void Canvas2D::filterImage()
{
    // TODO: [FILTER] Filter the image. Some example code to get the filter type is provided below.


    switch (settings.filterType) {

    case FILTER_BLUR:
        m_blur->execute(marqueeStart(),marqueeStop(),this,settings.blurRadius);
        break;
    case FILTER_INVERT:
        m_invert->execute(marqueeStart(),marqueeStop(),this);
        // fill in the rest
        break;
    case FILTER_GRAYSCALE:
        m_grayscale->execute(marqueeStart(),marqueeStop(),this);
        break;
    case FILTER_EDGE_DETECT:
        m_grayscale->execute(marqueeStart(),marqueeStop(),this);

        m_sobel->execute(marqueeStart(),marqueeStop(),this,settings.edgeDetectThreshold);
        break;
    case FILTER_SCALE:
        m_scale->execute(marqueeStart(),marqueeStop(),this,settings.scaleX,settings.scaleY);
        break;
    case FILTER_SPECIAL_1:
        m_median->execute(marqueeStart(),marqueeStop(),this);
        break;

    }
    this->update();

}

void Canvas2D::setScene(RayScene *scene)
{
    delete m_scene;
    m_scene = scene;
}


void Canvas2D::renderImage(Camera *camera, int width, int height)
{

    int core_number = QThread::idealThreadCount();
    if (m_scene)
    {
        // @TODO: raytrace the scene based on settings
        //        YOU MUST FILL THIS IN FOR INERSECT/RAY

        // If you want the interface to stay responsive, make sure to call
        // QCoreApplication::processEvents() periodically during the rendering
        Matrix4x4 view_m = camera->getViewingMatrix();

        this->resize(width,height);

        if((core_number ==4)&&(settings.useMultiThreading ==true)){

            RayScene* t1 = new RayScene(m_scene->getscene(),m_scene->getlight(),m_scene->getglobaldata());
            RayScene* t2 = new RayScene(m_scene->getscene(),m_scene->getlight(),m_scene->getglobaldata());
            RayScene* t3 = new RayScene(m_scene->getscene(),m_scene->getlight(),m_scene->getglobaldata());
            MyThread thread1;
            MyThread thread2;
            MyThread thread3;
            thread1.setarg(t1,this,view_m,1,4);
            thread2.setarg(t2,this,view_m,2,4);
            thread3.setarg(t3,this,view_m,3,4);
            thread1.start();
            thread2.start();
            thread3.start();
            m_scene->render(this,view_m,0,4);
            thread1.wait();
            thread2.wait();
            thread3.wait();
            delete t1;
            delete t2;
            delete t3;

        }
        else{
            m_scene->render(this,view_m,0,1);

        }

        this->update();




    }
}

void Canvas2D::cancelRender()
{
    // TODO: cancel the raytracer (optional)
}



void Canvas2D::settingsChanged() {

    // TODO: Process changes to the application settings.

}
