#include "glwaterfall.h"

#include <QDebug>

GLWaterfall::GLWaterfall(int fft_size, QWidget *parent)
    : m_fft_size(fft_size), QOpenGLWidget(parent)
{

}

GLWaterfall::~GLWaterfall() {
    makeCurrent();
    delete m_colored_line;
    delete m_textured_quad;
    doneCurrent();
}

void GLWaterfall::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0,0,0,1);

    m_colored_line = new WOpenGLShaderProgram("colored_line");
    m_attrib_pow = m_colored_line->attributeLocation("pow");
    m_attrib_bin = m_colored_line->attributeLocation("bin");

    m_textured_quad = new WOpenGLShaderProgram("textured_quad");
    m_attrib_vec = glGetAttribLocation(m_textured_quad->programId(), "vec");

    glGenTextures(1, &m_fbo_text);
    glBindTexture(GL_TEXTURE_2D, (GLuint)m_fbo_text);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_fft_size, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, (GLuint)m_fbo_text,0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    m_ortho.ortho(0, m_fft_size, 0, 512, -1, 1);

    GLfloat quad_vertices[] = {-1, -1, // bottom left corner
                               -1,  1, // top left corner
                               1,  1, // top right corner
                               1, 1, // bottom right
                              1,-1, // bottom right
                              -1,-1}; // bottom left corner

    glGenBuffers(1, &m_vbo_quad);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_quad);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);

    // Create bin buffer
    GLfloat bins[m_fft_size];
    for(int i = 0; i < m_fft_size; i++) {
        bins[i] = float(i);
    }

    glGenBuffers(1, &m_vbo_bin);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_bin);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bins), bins, GL_STATIC_DRAW);

    // Create and zero power buffer
    glGenBuffers(1, &m_vbo_pow);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_pow);
    glBufferData(GL_ARRAY_BUFFER, m_fft_size * sizeof(float), 0, GL_STREAM_DRAW);

    qDebug() << "glerror" << glGetError();
}


void GLWaterfall::updateWaterfall(float *fft_data)
{
    makeCurrent();
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_pow);
    glBufferData(GL_ARRAY_BUFFER, m_fft_size * sizeof(float), fft_data, GL_STREAM_DRAW);
    updateTexture();
    doneCurrent();
    update();
}

void GLWaterfall::resizeGL(int w, int h)
{
    qDebug() << "resize";
}

void GLWaterfall::updateTexture() {
    // Draw on texture
    //qDebug() << "update " << glGetError();
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    glViewport(0,0,m_fft_size,512);
    m_colored_line->bind();

    glEnableVertexAttribArray(m_attrib_pow);
    glEnableVertexAttribArray(m_attrib_bin);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_pow);
    glVertexAttribPointer(m_attrib_pow, 1, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_bin);
    glVertexAttribPointer(m_attrib_bin, 1, GL_FLOAT, GL_FALSE, 0, 0);

    m_colored_line->setUniformValue("line", float(m_line));
    m_colored_line->setUniformValue("ortho", m_ortho);

    glDrawArrays(GL_POINTS, 0, m_fft_size);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    m_colored_line->release();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Next scanline
    m_line++;
    if(m_line > 512) m_line = 0;
}

void GLWaterfall::paintGL() {
    // Draw textured quad
    glViewport(0,0,width(),height());
    glClear(GL_COLOR_BUFFER_BIT);

    m_textured_quad->bind();

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_quad);
    glEnableVertexAttribArray(m_attrib_vec);
    glVertexAttribPointer(m_attrib_vec,2,GL_FLOAT,GL_FALSE,0,0);
    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_fbo_text);
    m_textured_quad->setUniformValue("waterfall", 0);
    m_textured_quad->setUniformValue("offset", float(m_line/512.0));

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(m_attrib_vec);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    m_textured_quad->release();
}
