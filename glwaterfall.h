#ifndef GLWATERFALL_H
#define GLWATERFALL_H

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QOpenGLFramebufferObject>

#include "wopenglshaderprogram.h"

class GLWaterfall : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    GLWaterfall(int fft_size, QWidget *parent = 0);
    ~GLWaterfall();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

public slots:
    void updateWaterfall(float *fft_data);

private:
    void updateTexture();
    int m_fft_size;

    GLuint m_fbo_text;
    GLuint m_fbo;
    GLuint m_vbo_pow;
    GLuint m_vbo_bin;
    GLuint m_attrib_pow;
    GLuint m_attrib_bin;

    GLuint m_vbo_quad;
    GLuint m_attrib_vec;

    int m_line = 0;

    QMatrix4x4 m_ortho;
    WOpenGLShaderProgram *m_textured_quad;
    WOpenGLShaderProgram *m_colored_line;
};

#endif // GLWATERFALL_H
