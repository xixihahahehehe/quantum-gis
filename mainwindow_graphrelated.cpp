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
#include <QBarSeries>
#include <QBarSet>
#include "dialog/graphexploredlg.h"

//read flow

void MainWindow::on_actionbind_flowcollection_with_basemap_triggered()
{
    BindingDialog dlg(this);
    QStringList FlowColList;
    FlowColList.clear();
    for (int i=0;i<flowcollections.size();i++)
        FlowColList <<flowcollections[i].GetName().c_str();
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


//    QChart *chart = new QChart;
//    QBarSet *set0 = new QBarSet("Degree");
//    for(int i=0;i<v_hist.size();i++)
//    {
//        *set0<<v_hist[i];
//    }
//    QBarSeries *series = new QBarSeries();
//    series->append(set0);
//    chart->addSeries(series);
//    //chart->createDefaultAxes();
//    //QValueAxis *axisX = new QValueAxis;
//    //axisX->setTickCount(10);
//    //chart->addAxis(axisX, Qt::AlignBottom);
//    QValueAxis *axisY = new QValueAxis;
//    chart->addAxis(axisY, Qt::AlignLeft);
//    //series->attachAxis(axisX);
//    series->attachAxis(axisY);

//    chart->setTitle("Degree Distribution");
//    chart->setAnimationOptions(QChart::SeriesAnimations);
//    chart->legend()->setVisible(true);
//    chart->legend()->setAlignment(Qt::AlignBottom);
//    chart->layout()->setContentsMargins(0, 0, 0, 0);


//    QChartView *chartView = new QChartView(chart);
//    chartView->setRenderHint(QPainter::Antialiasing);
//    GraphExploreDlg *window= new GraphExploreDlg();
//    window->setCentralWidget(chartView);
//    window->resize(800, 600);
//    window->setWindowModality(Qt::WindowModal);

//    window->show();


}

void MainWindow::on_action_generate_a_flowgraph_triggered()
{
    Gen_Graph_Dialog dlg(this);
    QStringList FlowColList;
    FlowColList.clear();
    for (int i=0;i<flowcollections.size();i++)
        FlowColList <<flowcollections[i].GetName().c_str();
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

void MainWindow::on_actiongraph_properties_triggered()
{
//    vector<int> v_degree,v_hist;
//    od_graph.get_degree(v_degree);
//    Histogram hist(v_degree);
//    hist.getHistVec(v_hist);
    GraphExploreDlg* dlg2 = new GraphExploreDlg;
    dlg2->GetData(od_graph);
    dlg2->show();
}
