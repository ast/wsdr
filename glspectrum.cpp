#include "glspectrum.h"

#include <QDebug>
#include <QPainter>
#include <QMouseEvent>

#include "wopenglshaderprogram.h"


GLSpectrum::GLSpectrum(int fft_size, float bandwidth, QWidget *parent)
    : m_fft_size(fft_size),
      m_bandwidth(bandwidth),
      QOpenGLWidget(parent)
{ }


GLSpectrum::~GLSpectrum()
{
    makeCurrent();

    delete grid;
    delete fft_line;

    doneCurrent();
}

void GLSpectrum::loadShaders()
{

}

void GLSpectrum::setCenterFrequency(float centerfrequency) {
    if(m_centerFrequency != centerfrequency) {
        m_centerFrequency = centerfrequency;
        update();
    }
}

void GLSpectrum::setSelectedOffset(float selectedOffset)  {
    if(m_selectedOffset != selectedOffset) {
        m_selectedOffset = selectedOffset;
        emit selectedOffsetChanged(selectedOffset);
        update();
    }
}

void GLSpectrum::setBandwidth(float bandwidth)  {
    if(m_bandwidth != bandwidth) {
        m_bandwidth = bandwidth;
        update();
    }
}

void GLSpectrum::setFilterBandwidth(float filterBandwidth) {
    if(m_filterBandwidth != filterBandwidth) {
        m_filterBandwidth = filterBandwidth;
        update();
    }
}

void GLSpectrum::mousePressEvent(QMouseEvent *event) {
    qDebug() << "mouse press" << (float(event->x()) / width());



    float offset =  m_bandwidth * float(event->x()) / width() - m_bandwidth/2.0;

    qDebug() << "offset" << offset;
    setSelectedOffset(offset);

    update();
}

void GLSpectrum::initializeGL()
{
    initializeOpenGLFunctions();

    freq_text.setPerformanceHint(QStaticText::AggressiveCaching);
    freq_text.setText("14.000.000");

    fft_line = new WOpenGLShaderProgram("fft_line");
    grid = new WOpenGLShaderProgram("h_grid");

    ortho.ortho(0, m_fft_size, -100,0, -1, 1);
    ortho_grid.ortho(0, 1, -100, 0, -1, 1);

    vec_attrib = grid->attributeLocation("vec_attrib");
    qDebug() << vec_attrib;

    pow_attrib = glGetAttribLocation(fft_line->programId(), "pow_attrib");
    qDebug() << pow_attrib;
    bin_attrib = glGetAttribLocation(fft_line->programId(), "bin_attrib");
    qDebug() << bin_attrib;

    QVector<float> grid;
    // horizontal lines
    for(int i = 1; i < 10; i++) {
        grid.push_back(0.);
        grid.push_back(-i*10);
        grid.push_back(1.);
        grid.push_back(-i*10);
    }
    // vertical lines
    for(int i = 1; i < 10; i++) {
        float ymax = 0;
        float ymin = -100;
        float x    = i / 10.0;

        grid.push_back(x);
        grid.push_back(ymax);
        grid.push_back(x);
        grid.push_back(ymin);
    }

    glGenBuffers(1, &vbo_hgrid);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_hgrid);
    glBufferData(GL_ARRAY_BUFFER, grid.size() * sizeof(float), grid.constData(), GL_STATIC_DRAW);

    // gen bins
    const int n = m_fft_size;
    GLfloat bins[n];
    for(int i = 0; i < n; i++) {
        bins[i] = float(i);
    }

    //glBindVertexArray(vao_fft);
    glGenBuffers(1, &vbo_bins);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_bins);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bins), bins, GL_STATIC_DRAW);

    // zero power buffer
    GLfloat pow[n] = {0.};
    glGenBuffers(1, &vbo_pow);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_pow);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pow), pow, GL_STREAM_DRAW);
}


void GLSpectrum::updateWaterfall(float *fft_data)
{
    makeCurrent();
    glBindBuffer(GL_ARRAY_BUFFER, vbo_pow);
    glBufferData(GL_ARRAY_BUFFER, m_fft_size * sizeof(float), fft_data, GL_STREAM_DRAW);
    doneCurrent();

    update();
}



void GLSpectrum::resizeGL(int w, int h)
{
    //qDebug() << "resize";
}


void GLSpectrum::paintGL() {

    glClear(GL_COLOR_BUFFER_BIT);

    grid->bind();
    grid->setUniformValue("ortho", ortho_grid);
    glEnableVertexAttribArray(vec_attrib);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_hgrid);
    glVertexAttribPointer(vec_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_LINES, 0, 36); // number of vertecies
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(vec_attrib);
    grid->release();

    fft_line->bind();
    fft_line->setUniformValue("ortho", ortho);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_bins);
    glEnableVertexAttribArray(bin_attrib);
    glVertexAttribPointer(bin_attrib, 1, GL_FLOAT, GL_FALSE, 0, 0);

    // power
    glBindBuffer(GL_ARRAY_BUFFER, vbo_pow);
    glEnableVertexAttribArray(pow_attrib);
    glVertexAttribPointer(pow_attrib, 1, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_LINE_STRIP, 0, m_fft_size);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    fft_line->release();

    // QT painting starts here
    QPainter painter(this);
    QColor fontColor(127, 127, 127, 127);

    // Render text
    //QFont font = QFont();
    // font.setPixelSize(50);
    //freq_text.size()
    //painter.setPen(fontColor);
    //painter.setFont(font);
    //painter.drawStaticText(10, 10, freq_text);

    QRectF rect;

    rect.setWidth(width() * m_filterBandwidth/m_bandwidth);
    rect.setHeight(height());
    rect.moveRight(width() * (m_selectedOffset + m_bandwidth/2) / m_bandwidth + rect.width()/2);

    painter.fillRect(rect, fontColor);
    painter.end();
}
