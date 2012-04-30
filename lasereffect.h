#ifndef LASEREFFECT_H 
#define LASEREFFECT_H 

#include "vector.h"
#include <QGLShaderProgram>
#include <qgl.h>

class LaserEffect
{
public:
    LaserEffect();
    ~LaserEffect();

    void draw(const Vector3 &start, const Vector3 &end, float radius);

private:
    QGLShaderProgram *m_shaderprog;
    GLuint m_vbo;
    unsigned int m_count;
};

#endif // LASEREFFECT_H 
