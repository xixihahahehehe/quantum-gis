#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QtCharts>
QT_CHARTS_USE_NAMESPACE
#include <QMainWindow>
#include "collection/flowcollection.h"
#include "collection/odcollection.h"
#include <vector>
#include <gdal_priv.h>
#include "visualize/flow_viz.h"
#include "visualize/menuwidget.h"
#include "visualize/propertytable.h"
#include <QStringList>
#include "dialog/open_odshp.h"
#include "collection/flowgraph.h"
#include "method/DistanceDecayPara.h"
#include "dialog/chartviewdialog.h"
#include "dialog/gen_graph_dialog.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    vector<flowcollection> flowcollections;

    //OD
    vector<ODcollection> ODcollections;

    list<OGRLayer *> myLayers;
    int layercount;
    flowgraph od_graph;



    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actiontxt_triggered();

    void on_actionflow_txt_triggered();

    void on_actionOD_triggered();

    void on_actionflow_triggered();

    void on_actiondistance_triggered();

    void on_actiondirection_triggered();

    void on_actionweight_triggered();
    void on_actionod_shp_triggered();
    void on_action_chart_triggered();



    void on_action_generate_a_flowgraph_triggered();

private:
    Ui::MainWindow *ui;
    flow_viz *_flowviz;
    MenuWidget *_leftbar;
    //PropertyTable *_ptable;

    QStringList od_list;
    QStringList flow_list;
    QStringList base_list;
};
#endif // MAINWINDOW_H
