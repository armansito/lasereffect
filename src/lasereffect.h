#ifndef LASEREFFECT_H 
#define LASEREFFECT_H 

#include "vector.h"
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

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
    GLuint m_prog, m_vert, m_frag;
    GLuint m_vbo;
    unsigned int m_count;
};

#endif // LASEREFFECT_H 
