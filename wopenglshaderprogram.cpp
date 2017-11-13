#include "wopenglshaderprogram.h"

#include <stdexcept>

WOpenGLShaderProgram::WOpenGLShaderProgram(QString name)
{
    QString prefix("shaders/%1.%2");

    addShaderFromSourceFile(QOpenGLShader::Vertex, prefix.arg(name).arg("vert"));
    addShaderFromSourceFile(QOpenGLShader::Fragment, prefix.arg(name).arg("frag"));

    if(!link()) {
        throw std::runtime_error("link");
    }
    if(!bind()) {
        throw std::runtime_error("bind");
    }
}
