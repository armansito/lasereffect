#include "lasereffect.h"

#ifndef __APPLE__
extern "C" {
    void glBindBuffer (GLenum, GLuint);
    void glDeleteBuffers (GLsizei, const GLuint *);
    void glGenBuffers (GLsizei, GLuint *);
    GLboolean glIsBuffer (GLuint);
    void glBufferData (GLenum, GLsizeiptr, const GLvoid *, GLenum);
}
#endif

LaserEffect::LaserEffect()
{
    const Vector3 corners[] = {

        Vector3(-1.f, -1.f, -1.f),
        Vector3( 1.f, -1.f, -1.f),
        Vector3( 1.f,  1.f, -1.f),
        Vector3(-1.f,  1.f, -1.f),

        Vector3(-1.f, -1.f,  1.f),
        Vector3( 1.f, -1.f,  1.f),
        Vector3( 1.f,  1.f,  1.f),
        Vector3(-1.f,  1.f,  1.f),
    };

    const Vector3 vertices[] = {

        corners[0], corners[1],
        corners[0], corners[3],
        corners[1], corners[2],
        corners[2], corners[3],

        corners[4], corners[5],
        corners[5], corners[6],
        corners[6], corners[7],
        corners[7], corners[4],

        corners[0], corners[4],
        corners[1], corners[5],
        corners[2], corners[6],
        corners[3], corners[7]
    };

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), (GLvoid *)vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_count = sizeof(vertices)/sizeof(Vector3);
}

LaserEffect::~LaserEffect()
{
    glDeleteBuffers(1, &m_vbo);
}

void LaserEffect::draw(const Vector3 &start, const Vector3 &end, float radius)
{
    glBegin(GL_LINES);
    glVertex3fv(start.xyz); glVertex3fv(end.xyz);
    glEnd();

    Vector3 dir = end - start;
    float vlen = dir.length();
    float len = vlen + radius + radius; 
    const Vector2 &angles = dir.toAngles();

    glPushMatrix();

    glTranslatef(start.x, start.y, start.z);    
    glRotatef(angles.x * 180.f/M_PI, 0.f, 1.f, 0.f);
    glRotatef(angles.y * 180.f/M_PI, 1.f, 0.f, 0.f);
    glTranslatef(0.f, 0.f, -vlen*0.5f);
    glScalef(radius, radius, len*0.5f);

    glColor3f(0.f, 0.f, 0.f);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_LINES, 0, m_count);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glPopMatrix();
}
