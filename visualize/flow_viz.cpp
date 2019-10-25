#include "flow_viz.h"
#include "ui_flow_viz.h"
#include <QPainter>

flow_viz::flow_viz(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::flow_viz)
{
    ui->setupUi(this);
}

flow_viz::~flow_viz()
{
    delete ui;
    //delete flowcollection
}

void flow_viz::get_flowcollection(flowcollection _flowcollection1)
{
    _flowcollection=_flowcollection1;
}

void flow_viz::paintEvent(QPaintEvent *)
{
    QPainter *painter=new QPainter(this);
    painter->setRenderHint(QPainter::Antialiasing);
    _flowcollection.draw_basemap(painter,rect());
    //_flowcollection.draw_flow(painter,rect());
    painter->end();
}
