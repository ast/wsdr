#ifndef GLSPECTRUM_H
#define GLSPECTRUM_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QStaticText>
#include <complex>

#include "wopenglshaderprogram.h"


class GLSpectrum : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

    //Q_PROPERTY(float frequency READ frequency WRITE setFrequency NOTIFY frequencyChanged)

public:
    GLSpectrum(int fft_size, float bandwidth, QWidget *parent = 0);
    ~GLSpectrum();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void mousePressEvent(QMouseEvent *event);

    float frequency();

public slots:
    void setCenterFrequency(float centerfrequency);
    void setSelectedOffset(float selectedOffset);
    void setBandwidth(float bandwidth);
    void setFilterBandwidth(float filterBandwidth);
    void updateWaterfall(float *fft_data);

signals:
    void selectedOffsetChanged(float selectedOffset);

private:
    void loadShaders();

    int m_fft_size;

    float m_centerFrequency;
    float m_bandwidth;
    float m_filterBandwidth;
    float m_selectedOffset;

    WOpenGLShaderProgram *fft_line;
    WOpenGLShaderProgram *grid;

    QStaticText freq_text;

    GLuint vao_hgrid;
    GLuint vbo_hgrid;
    GLuint vec_attrib;

    GLuint vao_fft;
    GLuint vbo_pow;
    GLuint vbo_bins;

    GLuint pow_attrib;
    GLuint bin_attrib;

    QMatrix4x4 ortho;
    QMatrix4x4 ortho_grid;
};

#endif // GLSPECTRUM_H
