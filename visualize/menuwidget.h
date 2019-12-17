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

    void update_menu(list<ODcollection*> _odcollection,list<flowcollection*> _flowcollection,list<OGRLayer*> _basemap,vector<int> selected_OD,vector<int> selected_flow,vector<int> selected_base);

    flow_viz *_fviz;


    list<ODcollection*> menu_odcollection;
    list<flowcollection*> menu_flowcollection;
    list<OGRLayer*> menu_layer;

    vector<int> od_selected;
    vector<int> flow_selected;
    vector<int> base_selected;

private:
    Ui::MenuWidget *ui;
    QListWidget *_menu;
    int label_OD;
    int label_flow;
    int label_base;
    void update_info();
    void update_drawing();

private slots:
    //for checkbox
    void update_selecteditems();

signals:
    void send_menu(vector<int>,vector<int>,vector<int>);

};

#endif // MENUWIDGET_H
