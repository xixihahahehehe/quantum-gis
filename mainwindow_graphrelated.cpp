#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "auxiliary_func.h"
#include "ogrsf_frmts.h"
#include <QFileDialog>
#include <QDebug>
#include <iostream>
#include <QTableWidget>
#include <QMetaType>
#include <time.h>
#include <QMessageBox>
#include <map>
//read flow

void MainWindow::on_action_chart_triggered()
{
    ChartViewDialog dlg(this);
    vector<vector<double>> res;
    double b_stat = distance_decay_parameter(od_graph, res);
    dlg.getdata(res);
    dlg.draw();
    dlg.exec();
}

void MainWindow::on_action_generate_a_flowgraph_triggered()
{
    Gen_Graph_Dialog dlg(this);
    QStringList FlowColList;//停止位
    FlowColList.clear();
    for (int i=0;i<flowcollections.size();i++)
        FlowColList <<flowcollections[i].name.c_str();
    dlg.setFlowColList(FlowColList);
    dlg.updateComboBx();

    //dlg.exec();
    if (dlg.exec() == QDialog::Accepted) {
        // do something
        flowcollection taxiflows=flowcollections[dlg.getFlowColIdx()];
        od_graph.gen_flowgraph(&taxiflows);
    } else {
        // do something else
    }

}
