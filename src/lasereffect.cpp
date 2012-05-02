#include "lasereffect.h"
#include <cstring>

#ifndef __APPLE__
extern "C" {
    GLuint glCreateShader(GLenum);
    void glShaderSource(GLuint, GLsizei, const GLchar **, const GLint *);
    void glCompileShader(GLuint);
    void glUniformMatrix4fv(GLint, GLsizei, GLboolean, const GLfloat *);
    GLuint glCreateProgram();
    void glAttachShader(GLuint, GLuint);
    void glLinkProgram(GLuint);
    void glUseProgram(GLuint);
    void glUniform3fv(GLint, GLsizei, const GLfloat *);
    void glUniform1f(GLint, GLfloat);
    GLint glGetUniformLocation(GLuint, const GLchar *);
    void glDeleteProgram(GLuint);
    void glDeleteShader(GLuint);

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

    const GLchar *vshader =
        "uniform vec3 start;\n"
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
        "}\n";

    const GLchar *fshader = 
        "uniform float radius;\n"
        "uniform float beam_ratio;\n"
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
        "    float sc, tc, sN, tN, sD = D, tD = D;\n"
        "    if (D < SMALL_NUM) {\n"
        "        sN = 0.0;\n"
        "        sD = 1.0;\n"
        "        tN = e;\n"
        "        tD = c;\n"
        "    } else {\n"
        "        sN = (b*e - c*d);\n"
        "        tN = (a*e - b*d);\n"
        "        if (sN < 0.0) {\n"
        "            sN = 0.0;\n"
        "            tN = e;\n"
        "            tD = c;\n"
        "        } else if (sN > sD) {\n"
        "            sN = sD;\n"
        "            tN = e + b;\n"
        "            tD = c;\n"
        "        }\n"
        "    }\n"
        "    sc = abs(sN) < SMALL_NUM ? 0.0 : sN / sD;\n"
        "    tc = abs(tN) < SMALL_NUM ? 0.0 : tN / tD;\n"
        "    vec3 dP = w + (sc * u) - (tc * v);\n"
        "    return length(dP);\n"
        "}\n"

        "void main(void)\n"
        "{\n"
        "    float dist = dist_segments(startp, endp, vec3(0.0), pos);\n"
        "    float factor = 1.0 - dist/radius;\n"
        "    vec3 color;\n"
        "    if (dist < beam_ratio * radius)\n"
        "        color = mix(vec3(1.0, 1.0, 1.0), gl_Color.xyz, dist/(beam_ratio*radius)) * factor;\n"
        "    else\n"
        "        color = gl_Color.xyz * factor;\n"
        "    gl_FragColor = vec4(color, 1.0);\n"
        "}\n";
    
    GLuint vert, frag;
    vert = glCreateShader(GL_VERTEX_SHADER);
    frag = glCreateShader(GL_FRAGMENT_SHADER);

    size_t vlen = strlen(vshader),
           flen = strlen(fshader);
    glShaderSource(vert, 1, &vshader, (const GLint *)&vlen);
    glShaderSource(frag, 1, &fshader, (const GLint *)&flen);

    glCompileShader(vert);
    glCompileShader(frag);

    m_prog = glCreateProgram();
    glAttachShader(m_prog, vert);
    glAttachShader(m_prog, frag);
    glLinkProgram(m_prog);
    m_vert = vert;
    m_frag = frag;
}

LaserEffect::~LaserEffect()
{
    glDeleteBuffers(1, &m_vbo);
    glDeleteProgram(m_prog);
    glDeleteShader(m_vert);
    glDeleteShader(m_frag);
}

void LaserEffect::draw(const Vector3 &start, const Vector3 &end, float radius, float beam_ratio)
{
    Vector3 dir = end - start;
    float vlen = dir.length();
    float len = vlen + radius + radius; 
    const Vector2 &angles = dir.toAngles();

    GLfloat mv[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, mv);

    glCullFace(GL_FRONT);
    glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);

    glUseProgram(m_prog);
    glUniform3fv(glGetUniformLocation(m_prog, "start"), 1, start.xyz);
    glUniform3fv(glGetUniformLocation(m_prog, "end"), 1, end.xyz);
    glUniform1f(glGetUniformLocation(m_prog, "radius"), radius);
    glUniform1f(glGetUniformLocation(m_prog, "beam_ratio"), beam_ratio);
    glUniformMatrix4fv(glGetUniformLocation(m_prog, "camera_mv"), 1, GL_FALSE, mv);
    
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
    
    glUseProgram(0);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glCullFace(GL_BACK);
}
