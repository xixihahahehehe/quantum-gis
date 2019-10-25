#ifndef FLOW_VIZ_H
#define FLOW_VIZ_H

#include <QWidget>
#include "collection/flowcollection.h"

namespace Ui {
class flow_viz;
}

class flow_viz : public QWidget
{
    Q_OBJECT

public:
    explicit flow_viz(QWidget *parent = nullptr);
    ~flow_viz();
    flowcollection _flowcollection;

    void get_flowcollection(flowcollection _flowcollection1);

private:
    Ui::flow_viz *ui;
    virtual void paintEvent( QPaintEvent * );
};

#endif // FLOW_VIZ_H
