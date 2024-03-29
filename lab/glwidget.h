#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QHash>
#include <QString>
#include <QTimer>
#include <QTime>
#include "sphere.h"
#include "cone.h"
#include "camera.h"
#include "vector.h"
#include "resourceloader.h"

class QGLShaderProgram;
class QGLFramebufferObject;


class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

protected:
    // Overridden QGLWidget methods
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

    // Initialization code
    void initializeResources();
    void loadCubeMap();
    void createShaderPrograms();
    void createFramebufferObjects(int width, int height);

    // Drawing code
    void applyOrthogonalCamera(float width, float height);
    void applyPerspectiveCamera(float width, float height);
    void renderTexturedQuad(float width, float height, bool flip);
    void renderScene();
    void paintText();
    void drawtree(Vector3 start, int depth, double angle, double length, double x,double z);

private:
    QTimer m_timer;
    QTime m_clock;
    int m_prevTime;
    float m_prevFps, m_fps;
    Vector2 m_prevMousePos;
    OrbitCamera m_camera;
    Sphere *planet_sphere, *cloud_sphere, *water_sphere, *sun_sphere;
    cone* m_cone;
    // Resources
    QHash<QString, QGLShaderProgram *> m_shaderPrograms; // hash map of all shader programs
    QHash<QString, QGLFramebufferObject *> m_framebufferObjects; // hash map of all framebuffer objects
    GLuint m_skybox; // skybox call list ID
    GLuint m_cubeMap; // cubeMap texture ID
    QList<GLuint> m_terrains;
    int terrain_index;
    GLuint m_t1;
    GLuint m_t2;
    GLuint m_t3;
    GLuint m_t4;
    GLuint m_t5;
    GLuint m_t6;
    bool m_clouds;
    int terrain_smoothing;
    QFont m_font; // font for rendering text

};

#endif // GLWIDGET_H
