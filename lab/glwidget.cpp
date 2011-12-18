#include "glwidget.h"

#include <iostream>
#include <QFileDialog>
#include <QGLFramebufferObject>
#include <QGLShaderProgram>
#include <QMouseEvent>
#include <QTime>
#include <QTimer>
#include <QWheelEvent>
#include "glm.h"
#include "ui_mainwindow.h"

using std::cout;
using std::endl;

extern "C"
{
    extern void APIENTRY glActiveTexture(GLenum);
}

static const int MAX_FPS = 120;

/**
  Constructor.  Initialize all member variables here.
 **/
GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent),
    m_timer(this), m_prevTime(0), m_prevFps(0.f), m_fps(0.f),
    m_font("Deja Vu Sans Mono", 8, 4)
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    m_camera.center = Vector3(0.f, 0.f, 0.f);
    m_camera.up = Vector3(0.f, 1.f, 0.f);
    m_camera.zoom = 3.5f;
    m_camera.theta = M_PI * 1.5f, m_camera.phi = 0.2f;
    m_camera.fovy = 60.f;
    planet_sphere = new Sphere(250,250);
    water_sphere = new Sphere(100,100);
    cloud_sphere = new Sphere(100,100);
    sun_sphere = new Sphere(50,50);
    m_cone = new cone(10,10,false);
    terrain_index = 0;
    m_clouds = true;
    terrain_smoothing = 1;
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(update()));
}

/**
  Destructor.  Delete any 'new'ed objects here.
 **/
GLWidget::~GLWidget()
{
    foreach (QGLShaderProgram *sp, m_shaderPrograms)
        delete sp;
    foreach (QGLFramebufferObject *fbo, m_framebufferObjects)
        delete fbo;
    glDeleteLists(m_skybox, 1);
    const_cast<QGLContext *>(context())->deleteTexture(m_cubeMap);
    delete planet_sphere;
    delete water_sphere;
    delete cloud_sphere;
    delete sun_sphere;
    delete m_cone;

    // delete textures
    for (int i = 0; i < m_terrains.size(); i++){
        glDeleteTextures(1,&(m_terrains[i]));
    }
    glDeleteTextures(1,&m_t2);
    glDeleteTextures(1,&m_t3);
    glDeleteTextures(1,&m_t4);
    glDeleteTextures(1,&m_t5);
    glDeleteTextures(1,&m_t6);

}

/**
  Initialize the OpenGL state and start the drawing loop.
 **/
void GLWidget::initializeGL()
{
    // Set up OpenGL
    glEnable(GL_TEXTURE_2D);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glDisable(GL_DITHER);

    glDisable(GL_LIGHTING);
    glShadeModel(GL_FLAT);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Load resources, including creating shader programs and framebuffer objects
    initializeResources();

    // Start the drawing timer
    m_timer.start(1000.0f / MAX_FPS);
}

/**
  Initialize all resources.
  This includes models, textures, call lists, shader programs, and framebuffer objects.
 **/
void GLWidget::initializeResources()
{
    cout << "Using OpenGL Version " << glGetString(GL_VERSION) << endl << endl;
    // Ideally we would now check to make sure all the OGL functions we use are supported
    // by the video card.  But that's a pain to do so we're not going to.
    cout << "--- Loading Resources ---" << endl;

    m_skybox = ResourceLoader::loadSkybox();
    cout << "Loaded skybox..." << endl;

    loadCubeMap();
    cout << "Loaded cube map..." << endl;

    createShaderPrograms();
    cout << "Loaded shader programs..." << endl;

    createFramebufferObjects(width(),height());
    cout << "Loaded framebuffer objects..." << endl;

    cout << " --- Finish Loading Resources ---" << endl;
}

/**
  Load a cube map for the skybox
 **/
void GLWidget::loadCubeMap()
{
    QList<QFile *> fileList;

    fileList.append(new QFile("../cs123_final/textures/nebula/PN_right1.png"));
    fileList.append(new QFile("../cs123_final/textures/nebula/PN_left2.png"));
    fileList.append(new QFile("../cs123_final/textures/nebula/PN_top3.png"));
    fileList.append(new QFile("../cs123_final/textures/nebula/PN_bottom4.png"));
    fileList.append(new QFile("../cs123_final/textures/nebula/PN_front5.png"));
    fileList.append(new QFile("../cs123_final/textures/nebula/PN_back6.png"));

    m_cubeMap = ResourceLoader::loadCubeMap(fileList);

    // load all different planet terrains
    m_terrains.append(ResourceLoader::loadtexture2D(new QFile("../cs123_final/textures/planet_Klendathu.png")));
    m_terrains.append(ResourceLoader::loadtexture2D(new QFile("../cs123_final/textures/planet_Telos.png")));
    m_terrains.append(ResourceLoader::loadtexture2D(new QFile("../cs123_final/textures/planet_Trantor.png")));
    m_terrains.append(ResourceLoader::loadtexture2D(new QFile("../cs123_final/textures/planet_DamBaDa.png")));
    m_terrains.append(ResourceLoader::loadtexture2D(new QFile("../cs123_final/textures/planet_Arnessk.png")));
    m_terrains.append(ResourceLoader::loadtexture2D(new QFile("../cs123_final/textures/planet_Bog.png")));
    m_terrains.append(ResourceLoader::loadtexture2D(new QFile("../cs123_final/textures/planet_serendip.jpg")));
    m_terrains.append(ResourceLoader::loadtexture2D(new QFile("../cs123_final/textures/planet_Mars.png")));
    m_terrains.append(ResourceLoader::loadtexture2D(new QFile("../cs123_final/textures/planet_Reststop.png")));
    m_terrains.append(ResourceLoader::loadtexture2D(new QFile("../cs123_final/textures/planet_Terminus.png")));
    m_terrains.append(ResourceLoader::loadtexture2D(new QFile("../cs123_final/textures/planet_Jinx.png")));
    m_terrains.append(ResourceLoader::loadtexture2D(new QFile("../cs123_final/textures/planet_Trask.png")));
    m_terrains.append(ResourceLoader::loadtexture2D(new QFile("../cs123_final/textures/planet_Alba.png")));

    //m_t1 = ResourceLoader::loadtexture2D(new QFile("../cs123_final/textures/planet_Klendathu.png")));
    m_t2 = ResourceLoader::loadtexture2D(new QFile("../cs123_final/textures/gstar-original.jpg"));
    m_t3 = ResourceLoader::loadtexture2D(new QFile("../cs123_final/textures/clouds.jpg"));
    m_t4 = ResourceLoader::loadtexture2D(new QFile("../cs123_final/textures/green.jpg"));
    m_t5 = ResourceLoader::loadtexture2D(new QFile("../cs123_final/textures/leave.jpg"));
    m_t6 = ResourceLoader::loadtexture2D(new QFile("../cs123_final/textures/tree.jpg"));

}

/**
  Create shader programs.
 **/
void GLWidget::createShaderPrograms()
{
    const QGLContext *ctx = context();
    m_shaderPrograms["reflect"] = ResourceLoader::newShaderProgram(ctx, "../cs123_final/shaders/reflect.vert",
                                                                   "../cs123_final/shaders/reflect.frag");
    m_shaderPrograms["terrain"] = ResourceLoader::newShaderProgram(ctx, "../cs123_final/shaders/terrain.vert",
                                                                   "../cs123_final/shaders/terrain.frag");
    m_shaderPrograms["clouds"] = ResourceLoader::newFragShaderProgram(ctx, "../cs123_final/shaders/clouds.frag");
}

/**
  Allocate framebuffer objects.

  @param width: the viewport width
  @param height: the viewport height
 **/
void GLWidget::createFramebufferObjects(int width, int height)
{
    // Allocate the main framebuffer object for rendering the scene to
    // This needs a depth attachment
    m_framebufferObjects["fbo_0"] = new QGLFramebufferObject(width, height, QGLFramebufferObject::Depth,
                                                             GL_TEXTURE_2D, GL_RGB16F_ARB);
    m_framebufferObjects["fbo_0"]->format().setSamples(16);
    // Allocate the secondary framebuffer obejcts for rendering textures to (post process effects)
    // These do not require depth attachments
    m_framebufferObjects["fbo_1"] = new QGLFramebufferObject(width, height, QGLFramebufferObject::NoAttachment,
                                                             GL_TEXTURE_2D, GL_RGB16F_ARB);
}

/**
  Called to switch to an orthogonal OpenGL camera.
  Useful for rending a textured quad across the whole screen.

  @param width: the viewport width
  @param height: the viewport height
**/
void GLWidget::applyOrthogonalCamera(float width, float height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0.f, -1.f, 1.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/**
  Called to switch to a perspective OpenGL camera.

  @param width: the viewport width
  @param height: the viewport height
**/
void GLWidget::applyPerspectiveCamera(float width, float height)
{
    float ratio = ((float) width) / height;
    Vector3 dir(-Vector3::fromAngles(m_camera.theta, m_camera.phi));
    Vector3 eye(m_camera.center - dir * m_camera.zoom);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(m_camera.fovy, ratio, 0.1f, 1000.f);
    gluLookAt(eye.x, eye.y, eye.z, eye.x + dir.x, eye.y + dir.y, eye.z + dir.z,
              m_camera.up.x, m_camera.up.y, m_camera.up.z);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/**
  Draws the scene to a buffer which is rendered to the screen when this function exits.
 **/
void GLWidget::paintGL()
{
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update the fps
    int time = m_clock.elapsed();
    m_fps = 1000.f / (time - m_prevTime);
    m_prevTime = time;
    int width = this->width();
    int height = this->height();

    // Render the scene to a framebuffer
    m_framebufferObjects["fbo_0"]->bind();
    applyPerspectiveCamera(width, height);
    renderScene();
    m_framebufferObjects["fbo_0"]->release();

    // TODO: Add drawing code here
    applyOrthogonalCamera(width, height);
    glBindTexture(GL_TEXTURE_2D, m_framebufferObjects["fbo_0"]->texture());
    renderTexturedQuad(width, height, true);
    glBindTexture(GL_TEXTURE_2D, 0);

    paintText();
}

/**
  Renders the scene.  May be called multiple times by paintGL() if necessary.
**/
void GLWidget::renderScene() {

    // hold a uniform time so that all elements that use it are synched
    float elapsed = m_clock.elapsed()*.001;

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

    // Enable cube maps and draw the skybox
    glEnable(GL_TEXTURE_CUBE_MAP);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);
    glCallList(m_skybox);
    glBindTexture(GL_TEXTURE_CUBE_MAP,0);
    glDisable(GL_TEXTURE_CUBE_MAP);

    // Enable culling (back) faces for rendering the dragon
    glEnable(GL_CULL_FACE);

    // Render the planet with the terrain shader bound
    glEnable(GL_TEXTURE_2D);
    m_shaderPrograms["terrain"]->bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,m_terrains[terrain_index]);
    m_shaderPrograms["terrain"]->setUniformValue("planetTexture",GL_TEXTURE0);
    m_shaderPrograms["terrain"]->setUniformValue("xDim",planet_sphere->stacks);
    m_shaderPrograms["terrain"]->setUniformValue("yDim",planet_sphere->slices);
    m_shaderPrograms["terrain"]->setUniformValue("timer", elapsed);
    m_shaderPrograms["terrain"]->setUniformValue("terrain_smoothing", terrain_smoothing);
    glPushMatrix();
    planet_sphere->drawTriangles();
    glPopMatrix();
    m_shaderPrograms["terrain"]->release();

    // Render an interior chamber to the planet
    glEnable(GL_TEXTURE_2D);  
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,m_t4);
    glPushMatrix();
    glScalef(.65,.65,.65);
    glCullFace(GL_FRONT);
    planet_sphere->drawTriangles();
    glPopMatrix();
    glCullFace(GL_BACK);

    // draw the tree
    glPushMatrix();
    glScalef(.012,.012,.012);
    drawtree(Vector3(0,-12,0), 5, 10, 20, .6, .8);
    glPopMatrix();

    // create an orbiting sun around the planet
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,m_t2);
    glPushMatrix();
    glScalef(.25,.25,.25);
    glRotatef(elapsed,0,1,0);
    glTranslatef(20,0,0);
    sun_sphere->drawTriangles();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    // set blend to enable slightly transparent water
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

    // add water to the planet surface
    glEnable(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);
    glPushMatrix();
    m_shaderPrograms["reflect"]->bind();
    m_shaderPrograms["reflect"]->setUniformValue("CubeMap", GL_TEXTURE0);
    m_shaderPrograms["reflect"]->setUniformValue("timer", elapsed);
    glScalef(.95,.95,.95);
    water_sphere->drawTriangles();
    glPopMatrix();
    m_shaderPrograms["reflect"]->release();
    glDisable(GL_TEXTURE_CUBE_MAP);
    glDisable(GL_BLEND);

    // cloud effect
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,m_t3);
    m_shaderPrograms["clouds"]->bind();
    m_shaderPrograms["clouds"]->setUniformValue("cloudTexture",GL_TEXTURE0);
    glPushMatrix();
    glRotatef((-elapsed),0,1,0);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glScalef(1.1,1.1,1.1);
    if (m_clouds){
        glCullFace(GL_FRONT);
        cloud_sphere->drawTriangles();
        glCullFace(GL_BACK);
        cloud_sphere->drawTriangles();
    }
    glPopMatrix();
    m_shaderPrograms["clouds"]->release();
    glDisable(GL_BLEND);

    // Disable culling, depth testing and cube maps
    glActiveTexture(GL_TEXTURE0);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
}

/**
  Called when the mouse is dragged.  Rotates the camera based on mouse movement.
**/
void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    Vector2 pos(event->x(), event->y());
    if (event->buttons() & Qt::LeftButton || event->buttons() & Qt::RightButton)
    {
        m_camera.mouseMove(pos - m_prevMousePos);
    }
    m_prevMousePos = pos;
}

/**
  Record a mouse press position.
 **/
void GLWidget::mousePressEvent(QMouseEvent *event)
{
    m_prevMousePos.x = event->x();
    m_prevMousePos.y = event->y();
}

/**
  Called when the mouse wheel is turned.  Zooms the camera in and out.
**/
void GLWidget::wheelEvent(QWheelEvent *event)
{
    if (event->orientation() == Qt::Vertical)
    {
        m_camera.mouseWheel(event->delta());
    }
}

/**
  Called when the GLWidget is resized.
 **/
void GLWidget::resizeGL(int width, int height)
{
    // Resize the viewport
    glViewport(0, 0, width, height);

    // Reallocate the framebuffers with the new window dimensions
    foreach (QGLFramebufferObject *fbo, m_framebufferObjects)
    {
        const QString &key = m_framebufferObjects.key(fbo);
        QGLFramebufferObjectFormat format = fbo->format();
        delete fbo;
        m_framebufferObjects[key] = new QGLFramebufferObject(width, height, format);
    }
}

/**
  Draws a textured quad. The texture most be bound and unbound
  before and after calling this method - this method assumes that the texture
  has been bound before hand.

  @param w: the width of the quad to draw
  @param h: the height of the quad to draw
  @param flip: flip the texture vertically
**/
void GLWidget::renderTexturedQuad(float width, float height, bool flip) {
    // Clamp value to edge of texture when texture index is out of bounds
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Draw the  quad
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, flip ? 1.0f : 0.0f);
    glVertex2f(0.0f, 0.0f);
    glTexCoord2f(1.0f, flip ? 1.0f : 0.0f);
    glVertex2f(width, 0.0f);
    glTexCoord2f(1.0f, flip ? 0.0f : 1.0f);
    glVertex2f(width, height);
    glTexCoord2f(0.0f, flip ? 0.0f : 1.0f);
    glVertex2f(0.0f, height);
    glEnd();
}

/**
  Handles any key press from the keyboard
 **/
void GLWidget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_S:{
        QImage qi = grabFrameBuffer(false);
        QString filter;
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("PNG Image (*.png)"), &filter);
        qi.save(QFileInfo(fileName).absoluteDir().absolutePath() + "/" + QFileInfo(fileName).baseName() + ".png", "PNG", 100);
        break;
    }
        case Qt::Key_Right: // right press increments terrain index
            terrain_index = (terrain_index + 1) % m_terrains.size();
        break;
        case Qt::Key_Left: // left press decrements terrain index
            if (terrain_index > 0) terrain_index = (terrain_index - 1);
            else terrain_index = m_terrains.size()-1;
        break;
        case Qt::Key_C: // c-key toggles cloud cover
            m_clouds = !m_clouds;
        break;
        case Qt::Key_Down: // up press increments terrain smoothing
            terrain_smoothing++;
        break;
        case Qt::Key_Up: // down press decrements terrain smoothing
            terrain_smoothing = max(0, (terrain_smoothing-1));
    }
}

/**
  Draws text for the FPS and screenshot prompt
 **/
void GLWidget::paintText()
{
    glColor3f(1.f, 1.f, 1.f);

    // Combine the previous and current framerate
    if (m_fps >= 0 && m_fps < 1000)
    {
       m_prevFps *= 0.95f;
       m_prevFps += m_fps * 0.05f;
    }

    // QGLWidget's renderText takes xy coordinates, a string, and a font
    renderText(10, 20, "FPS: " + QString::number((int) (m_prevFps)), m_font);
    renderText(10, 35, "S: Save screenshot", m_font);
    renderText(10, 50, "Left / Right: Switch terrain", m_font);
    renderText(10, 65, "Up / Down: Terrain severity", m_font);
    renderText(10, 80, "C: Toggle clouds", m_font);

}

// fractally generated tree/fern/branch
void GLWidget::drawtree(Vector3 start, int depth, double angle, double length, double x,double z){

    Vector3 trans1,trans2,trans3,trans4;
    srand((unsigned)time(0));
    if (depth >0){
        double scoef = sin(angle*M_PI/180);
        double ccoef = cos(angle*M_PI/180);

        // make branches
        glPushMatrix();
        trans3 = Vector3(start.x - sqrt(scoef*length)*z,start.y+ccoef*length/2.0,start.z +sqrt(scoef*length)*x);
        trans2 = Vector3(start.x - sqrt(scoef*length)*z*1.2,start.y+ccoef*length/2.0*1.2,start.z +sqrt(scoef*length)*x*1.2);
        trans1 = Vector3(start.x - sqrt(scoef*length)*z*0.5,start.y+ccoef*length/2.0 *0.5,start.z + sqrt(scoef*length)*x*0.5);
        trans4 = Vector3(start.x - sqrt(scoef*length)*z*1.6,start.y+ccoef*length/2.0 *1.6,start.z + sqrt(scoef*length)*x*1.6);
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,m_t6);
        glTranslatef(trans3.x,trans3.y,trans3.z);
        glRotatef(angle,x,0,z);
        glScalef(pow(1.4,depth)/5,length,pow(1.4,depth)/5);
        glColor3f(0.5,0.5,0.4);
        m_cone->drawshape();
        glColor3f(1,1,1);
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D,0);

        // make leaves
        glEnable(GL_CULL_FACE);
        if (depth == 1){
            glColor3f(0.3,0.6,0.3);
            glEnable(GL_TEXTURE_2D);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D,m_t5);
            glPushMatrix();
            glTranslatef(trans2.x,trans2.y,trans2.z);
            glRotatef(angle,x,0,z);
            renderTexturedQuad(0.8,0.8,true);
            glRotatef(180,1,0,0);
            renderTexturedQuad(0.8,0.8,true);
            glPopMatrix();
            glPushMatrix();
            glTranslatef(trans3.x,trans3.y,trans3.z);
            glRotatef(angle,x,0,z);
            glRotatef(90,1,0,1);
            renderTexturedQuad(0.8,0.8,true);
            glRotatef(180,1,0,0);
            renderTexturedQuad(0.8,0.8,true);
            glPopMatrix();
            glPushMatrix();
            glTranslatef(trans4.x,trans4.y,trans4.z);
            glRotatef(angle,x,0,z);
            glRotatef(90,1,0,1);
            renderTexturedQuad(0.8,0.8,true);
            glRotatef(180,1,0,0);
            renderTexturedQuad(0.8,0.8,true);
            glPopMatrix();
            glBindTexture(GL_TEXTURE_2D,0);
            glColor3f(1,1,1);
        }

        drawtree(trans1,depth-1,angle+15,length*2/3,0.7,-sqrt(1-0.7*0.7));
        drawtree(trans2,depth-1,angle+25,length*2/3,-0.9,-sqrt(1-0.9*0.9));
        drawtree(trans3,depth-1,angle+20,length*2/3,0.2,sqrt(1-0.2*0.2));

    }
}
