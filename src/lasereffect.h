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

    /**
     * Draws a laser beam that glows with the color specified with the
     * most recent call to glColor.
     *
     * @param start       - starting point of the laser beam segment, in object space
     * @param end         - end point of the laser beam segment, in object space
     * @param radius      - radius of the glow, centered at the line segment specified
     *                      by start and end
     * @param beam_ratio  - ratio of the radius of the inner white glow to the outer
     *                      glow
     */
    void draw(const Vector3 &start, const Vector3 &end, float radius, float beam_ratio);

private:
    QGLShaderProgram *m_shaderprog;
    GLuint m_vbo;
    unsigned int m_count;
};

#endif // LASEREFFECT_H 
