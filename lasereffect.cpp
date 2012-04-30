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

        Vector3(-1.f, -1.f,  1.f),
        Vector3( 1.f, -1.f,  1.f),
        Vector3( 1.f,  1.f,  1.f),
        Vector3(-1.f,  1.f,  1.f),

        Vector3(-1.f, -1.f, -1.f),
        Vector3( 1.f, -1.f, -1.f),
        Vector3( 1.f,  1.f, -1.f),
        Vector3(-1.f,  1.f, -1.f),
    };

    const Vector3 vertices[] = {

        corners[0], corners[1], corners[2], corners[3],
        corners[7], corners[6], corners[5], corners[4],
        corners[0], corners[3], corners[7], corners[4],
        corners[1], corners[5], corners[6], corners[2],
        corners[0], corners[4], corners[5], corners[1],
        corners[3], corners[2], corners[6], corners[7]

    };

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), (GLvoid *)vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_count = sizeof(vertices)/sizeof(Vector3);

    m_shaderprog = new QGLShaderProgram();
    m_shaderprog->addShaderFromSourceCode(QGLShader::Vertex, "uniform vec3 start;\n"
                                                             "uniform vec3 end;\n"
                                                             "uniform mat4 camera_mv;\n"
                                                             "varying vec3 pos;\n"
                                                             "varying vec3 startp;\n"
                                                             "varying vec3 endp;\n"
                                                             
                                                             "void main(void)\n"
                                                             "{\n"
                                                             "    pos = (gl_ModelViewMatrix * gl_Vertex).xyz;\n"
                                                             "    startp = (camera_mv * vec4(start, 1.0)).xyz;\n" 
                                                             "    endp = (camera_mv * vec4(end, 1.0)).xyz;\n" 
                                                             "    gl_FrontColor = gl_Color;\n" 
                                                             "    gl_Position = ftransform();\n"
                                                             "}\n");

    m_shaderprog->addShaderFromSourceCode(QGLShader::Fragment, "uniform float radius;\n"
                                                               "varying vec3 startp;\n"
                                                               "varying vec3 endp;\n"
                                                               "varying vec3 pos;\n"
                                                               
                                                               "float dist_segments(in vec3 start1, in vec3 end1, in vec3 start2, in vec3 end2)\n"
                                                               "{\n"
                                                               "    const float SMALL_NUM = 1e-4;\n"
                                                               "    vec3 u = end1 - start1;\n"
                                                               "    vec3 v = end2 - start2;\n"
                                                               "    vec3 w = start1 - start2;\n"
                                                               "    float a = dot(u, u);\n"
                                                               "    float b = dot(u, v);\n"
                                                               "    float c = dot(v, v);\n"
                                                               "    float d = dot(u, w);\n"
                                                               "    float e = dot(v, w);\n"
                                                               "    float D = a*c - b*b;\n"
                                                               "    float sc, tc;\n"
                                                               "    if (D < SMALL_NUM) {\n"
                                                               "        sc = 0.0;\n"
                                                               "        tc = (b > c ? d/b : e/c);\n"
                                                               "    } else {\n"
                                                               "        sc = (b*e - c*d) / D;\n"
                                                               "        tc = (a*e - b*d) / D;\n"
                                                               "    }\n"
                                                               "    vec3 dP = w + (sc * u) - (tc * v);\n"
                                                               "    return length(dP);\n"
                                                               "}\n"

                                                               "void main(void)\n"
                                                               "{\n"
                                                               "    float dist = dist_segments(startp, endp, vec3(0.0), pos);\n"
                                                               "    vec3 color = gl_Color.xyz * (1.0 - dist/radius);\n"
                                                               "    gl_FragColor = vec4(color, 1.0);\n"
                                                               "}\n");
    m_shaderprog->link();
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

    GLfloat mv[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, mv);

    glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_ONE);

    m_shaderprog->bind();
    m_shaderprog->setUniformValue("start", start.x, start.y, start.z);
    m_shaderprog->setUniformValue("end", end.x, end.y, end.z);
    m_shaderprog->setUniformValue("radius", radius);
    glUniformMatrix4fv(m_shaderprog->uniformLocation("camera_mv"), 1, GL_FALSE, mv);
    glPushMatrix();

    glTranslatef(start.x, start.y, start.z);    
    glRotatef(angles.x * 180.f/M_PI, 0.f, 1.f, 0.f);
    glRotatef(angles.y * 180.f/M_PI, 1.f, 0.f, 0.f);
    glTranslatef(0.f, 0.f, -vlen*0.5f);
    glScalef(radius, radius, len*0.5f);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_QUADS, 0, m_count);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glPopMatrix();
    
    m_shaderprog->release();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
