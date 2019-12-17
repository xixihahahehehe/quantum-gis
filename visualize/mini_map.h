#ifndef MINI_MAP_H
#define MINI_MAP_H

#include <QWidget>
#include "flow_viz.h"

namespace Ui {
class mini_map;
}

class mini_map : public QWidget
{
    Q_OBJECT

public:
    explicit mini_map(QWidget *parent = nullptr);
    ~mini_map();

private:
    Ui::mini_map *ui;
    flow_viz *_fviz;
};

#endif // MINI_MAP_H
