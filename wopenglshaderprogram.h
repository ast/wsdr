#ifndef WOPENGLSHADERPROGRAM_H
#define WOPENGLSHADERPROGRAM_H

#include <QOpenGLShaderProgram>

class WOpenGLShaderProgram : public QOpenGLShaderProgram
{
    Q_OBJECT
public:
    WOpenGLShaderProgram(QString name);
};

#endif // WOPENGLSHADERPROGRAM_H
