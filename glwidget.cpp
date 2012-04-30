#include "glwidget.h"
#include "camera.h"
#include <QApplication>

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));
    m_camera = new Camera(45.f, 1.f, 0.1f, 1000);
}

GLWidget::~GLWidget()
{
    delete m_camera;
}

void GLWidget::initializeGL()
{
    glClearColor(1.f, 1.f, 1.f, 1.f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

    glColor4f(0.7f, 0.7f, 0.7f, 0.6f);
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
    glColor4f(1.f, 1.f, 1.f, 1.f);

    static float rot = 0.f;
    glRotatef(rot, 0.f, 0.f, 1.f);
    rot++;
    glBegin(GL_QUADS);
    glColor3f(1.f, 0.f, 0.f); glVertex3f(-0.5f, 0.5f, 0.f);
    glColor3f(1.f, 1.f, 0.f); glVertex3f(-0.5f,-0.5f, 0.f);
    glColor3f(0.f, 1.f, 0.f); glVertex3f( 0.5f,-0.5f, 0.f);
    glColor3f(0.f, 0.f, 1.f); glVertex3f( 0.5f, 0.5f, 0.f);
    glEnd();
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
