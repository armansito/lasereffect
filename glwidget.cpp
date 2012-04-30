#include "glwidget.h"
#include "camera.h"
#include <QApplication>
#include "lasereffect.h"

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));
    m_camera = new Camera(45.f, 1.f, 0.1f, 1000);
    m_lasereffect = NULL;
}

GLWidget::~GLWidget()
{
    delete m_camera;
    delete m_lasereffect;
}

void GLWidget::initializeGL()
{
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);

    m_lasereffect = new LaserEffect();

    m_timer.start(1000/60);
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    m_camera->setAspect((float)w/(float)h);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_camera->loadModelviewMatrix();

    glColor3f(0.7f, 0.7f, 0.7f);
    glBegin(GL_LINES);
    const float incr = 2.f;
    const float dim = 10.f; 
    for (float i = -dim*incr; i <= dim*incr; i += incr) {
        glVertex3f( dim*incr, 0.f, i);
        glVertex3f(-dim*incr, 0.f, i);
    }
    for (float i = -dim*incr; i <= dim*incr; i += incr) {
        glVertex3f(i, 0.f, dim*incr);
        glVertex3f(i, 0.f,-dim*incr);
    }
    glEnd();
    glColor3f(0.f, 0.f, 1.f);

    m_lasereffect->draw(Vector3(-1.f, 1.f, 1.f), Vector3(1.f, 1.f, -1.f), 0.5f);
}

void GLWidget::tick()
{
    updateGL();
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) QApplication::quit();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    m_mousepos = Vector2(event->x(), event->y());
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    Vector2 pos(event->x(), event->y());
    Vector2 delta = pos - m_mousepos;
    m_mousepos = pos;
    delta *= 5e-3f;
    m_camera->rotate(delta.x, delta.y);
}
