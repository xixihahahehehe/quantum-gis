#ifndef FLOW_VIZ_OPENGL_H
#define FLOW_VIZ_OPENGL_H

#include <QWidget>
#include <QtOpenGL>

#include <QPen>
#include <QBrush>
#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>

#include "collection/flowcollection.h"

namespace Ui {
class flow_viz_opengl;
}

class flow_viz_opengl : public QOpenGLWidget,protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit flow_viz_opengl(QOpenGLWidget *parent = nullptr);
    ~flow_viz_opengl();

private:
    Ui::flow_viz_opengl *ui;
    flowcollection *fcollection;

    bool						useShanders;
    QGLShaderProgram			*shaderProgram;
    int							shaderMatrix;
    int							shaderVertices;
    int							shaderColor;


protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
};

#endif // FLOW_VIZ_OPENGL_H
