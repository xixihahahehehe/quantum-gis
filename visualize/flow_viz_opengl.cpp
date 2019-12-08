#include "flow_viz_opengl.h"
#include "ui_flow_viz_opengl.h"

flow_viz_opengl::flow_viz_opengl(QOpenGLWidget *parent) :
    QOpenGLWidget(parent),
    ui(new Ui::flow_viz_opengl)
{
    ui->setupUi(this);
}

flow_viz_opengl::~flow_viz_opengl()
{
    delete ui;
}

void flow_viz_opengl::initializeGL()
{
    // Set up the rendering context, load shaders and other resources, etc.:
    //QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    //f->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    initializeOpenGLFunctions();
    glClearColor(1.0f,1.0f,1.0f,1.0f);

}

void flow_viz_opengl::resizeGL(int w, int h)
{

}

void flow_viz_opengl::paintGL()
{
    //clear the screen with the coolor initializing
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


}

void flow_viz_opengl::mousePressEvent(QMouseEvent *event)
{

}


void flow_viz_opengl::mouseMoveEvent(QMouseEvent *event)
{

}

void flow_viz_opengl::wheelEvent(QWheelEvent *event)
{

}
