Description
===========

Simple volumetric laser-beam renderer using Qt.

Usage
=====

To render a laser beam, first set the glow color by calling `glColor` and then call `LaserEffect::draw` on the instance you created:

    LaserEffect effect;
    glColor3f(0.f, 1.f, 0.f);
    effect.draw(Vector3(-1.f, 5.f, 1.f), Vector3(1.f, 5.f, -1.f), 0.4f, 0.35f);

the result of which looks like this:
![](https://github.com/armanuguray/lasereffect/blob/master/img/img.png?raw=true)

This will render a laser "beam segment" with a green glow. If your application has a rendering loop that gets constantly updated, repeatedly constructing a LaserEffect instance
will be inefficient. You should ideally create a LaserEffect instance when the OpenGL context is constructed and ready (in a Qt application, this would be in `QGLWidget::initializeGL()` for example).