#ifndef GLWATERFALLSPECTRUM_H
#define GLWATERFALLSPECTRUM_H

#include "glspectrum.h"

#include "wopenglshaderprogram.h"

class GLWaterfallSpectrum : public GLSpectrum
{
    WOpenGLShaderProgram *m_shader_textured_quad;
    WOpenGLShaderProgram *m_shader_points;

    GLuint m_texture;
    GLuint m_fbo;
    GLuint m_vbo_quad;
    GLuint m_attrib_vec;
    GLuint m_attrib_fft;
    GLuint m_attrib_bin;
    // Current scanline
    int m_line;
    // MUST be power of two!
    const int m_waterfall_height = 512;

public:
    GLWaterfallSpectrum(GLuint vbo_fft, GLuint vbo_bin, int size);
    ~GLWaterfallSpectrum();
    void draw();
    void update();
};

#endif // GLWATERFALLSPECTRUM_H
