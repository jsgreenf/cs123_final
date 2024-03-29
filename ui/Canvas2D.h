/*!
   @file   Canvas2D.h
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010
*/

#ifndef CANVAS2D_H
#define CANVAS2D_H

#include "SupportCanvas2D.h"

class RayScene;

class Camera;
class ConstantBrush;
class LinearBrush;
class QuadraticBrush;
class SmudgeBrush;
class invert;
class grayscale;
class sobel;
class blur;
class scale;
class median;
/*!
   @class Canvas2D

   @brief 2D canvas that students will implement in the Brush and Filter assignments.
   The same canvas will be used to display raytraced images in the Intersect and Ray
   assignments.

**/
class Canvas2D : public SupportCanvas2D
{
    Q_OBJECT
public:
    Canvas2D();
    virtual ~Canvas2D();

    void setScene(RayScene *scene);

    //! UI willvoid execute(QPoint m1, QPoint m2,Canvas2D* canvas); call this from the button on the "Ray" dock
    void renderImage(Camera *camera, int width, int height);

    //! This will be called when the settings have changed
    virtual void settingsChanged();


public slots:
    //! UI will call this from the button on the "Ray" dock
    void cancelRender();

    //! UI will call this from the button on the "Filter" dock
    void filterImage();

protected:
    virtual void paintEvent(QPaintEvent *);  /// overridden from SupportCanvas2D
    virtual void mouseDown(int x, int y);    /// called when the left mouse button is pressed down on the canvas
    virtual void mouseDragged(int x, int y); /// called when the left mouse button is dragged on the canvas
    virtual void mouseUp(int x, int y);      /// called when the left mouse button is released

    //! This will be called when the size of the canvas has been changed
    virtual void notifySizeChanged(int w, int h);



private:
    RayScene *m_scene;

    //TODO: [BRUSH, INTERSECT, RAY] Put your member variables here.
    ConstantBrush* m_constantbrush; /// pointer for constant brush
    LinearBrush* m_linearbrush;    /// pointer for linear brush
    QuadraticBrush* m_quadraticbrush; /// pointer for quadraticbrush
    SmudgeBrush* m_smudgebrush; /// pointer for smudgebrush
    invert* m_invert;
    grayscale* m_grayscale;
    sobel* m_sobel;
    blur* m_blur;
    scale* m_scale;
    median* m_median;




};

#endif // CANVAS2D_H
