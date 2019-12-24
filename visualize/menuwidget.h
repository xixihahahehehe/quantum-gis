#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QDockWidget>
#include <QListWidget>
#include <QStringList>
#include "flow_viz.h"
#include "collection/odcollection.h"
#include "collection/flowcollection.h"

namespace Ui {
class MenuWidget;
}

class MenuWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit MenuWidget(QWidget *parent = nullptr);
    ~MenuWidget();
    void update_menu(QStringList od_list,QStringList flow_list,QStringList base_list,vector<int> od_selected,vector<int> flow_selected,vector<int> base_selected);
    flow_viz *_fviz;

private:
    Ui::MenuWidget *ui;
    QListWidget *_menu;
    //for checkbox
    vector<int> od_selected;
    vector<int> flow_selected;
    vector<int> base_selected;

};

#endif // MENUWIDGET_H
