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

void MainWindow::on_actionbind_flowcollection_with_basemap_triggered()
{
    BindingDialog dlg(this);
    QStringList FlowColList;
    FlowColList.clear();
    for (int i=0;i<flowcollections.size();i++)
        FlowColList <<flowcollections[i].name.c_str();
    dlg.setFlowColList(FlowColList);
    dlg.updateFlowColList();
    QStringList LayerList;
    LayerList.clear();

    list<OGRLayer *>::iterator itor;
    for(itor=myLayers.begin();itor!=myLayers.end();++itor)
        LayerList<<(*itor)->GetName();
    dlg.setLayerList(LayerList);
    dlg.updateLayerList();
    if (dlg.exec() == QDialog::Accepted) {
        // do something
        flowcollection* taxiflows=&flowcollections[dlg.getFlowColIdx()];
        int layer_idx = dlg.getLayerIdx();
        list<OGRLayer *>::iterator itor2 = myLayers.begin();
        advance(itor2,layer_idx);
        taxiflows->layerConnection = *itor2;

    } else {
        // do something else
    }


}


void MainWindow::on_action_chart_triggered()
{
    if(od_graph.IsInitiated==false)
    {
        QMessageBox::critical(NULL, "Notice", "Graph not generated", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
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
    QStringList FlowColList;
    FlowColList.clear();
    for (int i=0;i<flowcollections.size();i++)
        FlowColList <<flowcollections[i].name.c_str();
    dlg.setFlowColList(FlowColList);
    dlg.updateComboBx();

    //dlg.exec();
    if (dlg.exec() == QDialog::Accepted) {
        // do something
        flowcollection taxiflows=flowcollections[dlg.getFlowColIdx()];
        if(taxiflows.layerConnection==NULL)
        {
            QMessageBox::critical(NULL, "Notice", "Base map not binded", QMessageBox::Ok, QMessageBox::Ok);
            return;
        }
        od_graph.gen_flowgraph(&taxiflows);
        string message="graph generated sucessfully";
        QString qmessage=QString::fromStdString(message);
        ui->statusbar->showMessage(qmessage);
    } else {
        // do something else
    }

}
