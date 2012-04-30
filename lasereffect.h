#ifndef LASEREFFECT_H 
#define LASEREFFECT_H 

#include "vector.h"
#include <QGLShaderProgram>

class LaserEffect
{
public:
    LaserEffect();
    ~LaserEffect();

    void draw(const Vector3 &start, const Vector3 &end, float radius);

private:
    QGLShaderProgram *m_shaderprog;    
};

#endif // LASEREFFECT_H 
