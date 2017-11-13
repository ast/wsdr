#include "glwaterfallspectrum.h"

#include <QMatrix4x4>

GLWaterfallSpectrum::GLWaterfallSpectrum(GLuint vbo_fft, GLuint vbo_bin, int size)
    : GLSpectrum(vbo_fft, vbo_bin, size)
{
    m_shader_points = new WOpenGLShaderProgram("colored_line");
    m_shader_textured_quad = new WOpenGLShaderProgram("textured_quad");

    m_attrib_fft = m_shader_points->attributeLocation("pow");
    m_attrib_bin = m_shader_points->attributeLocation("bin");
    m_attrib_vec = m_shader_textured_quad->attributeLocation("vec");

    QMatrix4x4 ortho;
    // Tranform matrix to size of texture
    ortho.ortho(0,size,0,512,-1,1);
    m_shader_points->setUniformValue("ortho", ortho);

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, (GLuint)m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size, m_waterfall_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

    // Attach texture to framebuffer
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, (GLuint)m_texture,0);
    // Unbind
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Setup quad on which to render texture
    GLfloat quad_vertices[] = {-1, -1, // bottom left corner
                               -1,  1, // top left corner
                               1,  1, // top right corner
                               1, 1, // bottom right
                               1,-1,
                               -1,-1}; // bottom left corner

    glGenBuffers(1, &m_vbo_quad);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_quad);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);
}

GLWaterfallSpectrum::~GLWaterfallSpectrum() {
    // Context must be current
    delete m_shader_points;
    delete m_shader_textured_quad;
}

void GLWaterfallSpectrum::update() {
    // Contect must be current
    // Draw line on texture
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    // Size of texture
    glViewport(0,0,size(),m_waterfall_height);
    m_shader_points->bind();

    glEnableVertexAttribArray(m_attrib_fft);
    glEnableVertexAttribArray(m_attrib_bin);

    glBindBuffer(GL_ARRAY_BUFFER, vboFFT());
    glVertexAttribPointer(m_attrib_fft, 1, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, vboBin());
    glVertexAttribPointer(m_attrib_bin, 1, GL_FLOAT, GL_FALSE, 0, 0);

    // Current scanline
    m_shader_points->setUniformValue("line", float(m_line));

    glDrawArrays(GL_POINTS, 0, size());
    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_shader_points->release();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Next scanline, wrap if over
    m_line++;
    if(m_line > m_waterfall_height) m_line = 0;
}

void GLWaterfallSpectrum::draw() {
    // Context and viewport must be set
    m_shader_textured_quad->bind();
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_quad);
    glEnableVertexAttribArray(m_attrib_vec);
    glVertexAttribPointer(m_attrib_vec,2,GL_FLOAT,GL_FALSE,0,0);
    // TODO
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    m_shader_textured_quad->setUniformValue("waterfall", 0);
    m_shader_textured_quad->setUniformValue("offset", float(m_line/float(m_waterfall_height)));
    // Draw
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // Unbind
    glDisableVertexAttribArray(m_attrib_vec);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    m_shader_textured_quad->release();
}
