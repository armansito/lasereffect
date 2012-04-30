#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QMouseEvent>
#include "vector.h"

class LaserEffect;
class Camera;

class GLWidget : public QGLWidget
{
Q_OBJECT
public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

private:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    QTimer m_timer;
    Camera *m_camera;
    LaserEffect *m_lasereffect;

    Vector2 m_mousepos;

private slots:
    void tick();
};

#endif // GLWIDGET_H
