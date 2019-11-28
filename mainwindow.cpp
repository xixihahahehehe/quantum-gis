#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "auxiliary_func.h"
#include "ogrsf_frmts.h"
#include <QFileDialog>
#include <QDebug>
#include <iostream>
#include <QTableWidget>
#include <QMetaType>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    layercount=0;

    _leftbar=new MenuWidget(this);
    _leftbar->setMaximumWidth(200);
    this->addDockWidget(Qt::LeftDockWidgetArea,_leftbar,Qt::Vertical);

    //_ptable=new PropertyTable(this);
    //_ptable->setMaximumWidth(200);
    //this->addDockWidget(Qt::RightDockWidgetArea,_ptable,Qt::Vertical);

    _flowviz=new flow_viz(this);
    this->setCentralWidget(_flowviz);
    ui->statusbar->showMessage("aaaaa",0);

    //deal with signals and slots

    /*workcontrol() {
        workers *worker = new workers;
        worker->moveToThread(&workerThread);
        connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &workcontrol::operate, worker, &workers::doWork);
        connect(worker, &workers::resultReady, this, &workcontrol::handleResults);
        workerThread.start();
    }*/
    workers *worker = new workers;
    worker->moveToThread(&workerThread);
    connect(&workerThread,&QThread::finished,worker,&QObject::deleteLater);
    connect(this,&MainWindow::operate,worker,&workers::doWork);
    connect(worker,&workers::resultReady,this,&MainWindow::handleResults);
    workerThread.start();
    //deal with communcations between threads
    qRegisterMetaType<ODcollection>("ODcollection");

}

MainWindow::~MainWindow()
{
    delete ui;
    workerThread.quit();
    workerThread.wait();
}


void MainWindow::on_actiontxt_triggered()
{

}

//shp
void MainWindow::on_actionflow_txt_triggered()
{
    QString fileName;
    fileName=QFileDialog::getOpenFileName(this,tr("文件"),"",tr("shapefile(*.shp)"));
    string cFileName=fileName.toStdString();
    char c[1000];
    strcpy(c,cFileName.c_str());

    GDALAllRegister();

    GDALDataset *poDS;

    poDS = (GDALDataset*) GDALOpenEx( c, GDAL_OF_VECTOR, NULL, NULL, NULL );
    if( poDS == NULL )
    {
        printf( "Open failed.\n" );
        exit( 1 );
    }
    int tmplayercount=poDS->GetLayerCount();
    for (int i=0;i<tmplayercount;i++) {
        static OGRLayer *tmp=poDS->GetLayer(i);
        myLayers.push_back(tmp);
    }

}

void MainWindow::on_actionOD_triggered()
{
    QString fileName;
    fileName=QFileDialog::getOpenFileName(this,tr("文件"),"",tr("text(*.txt)"));
    string cFileName=fileName.toStdString();
    if(!fileName.isNull()){
        ODcollections.push_back(ODcollection(cFileName));
    }
    else{
        qDebug()<<"取消";
    }
}

void MainWindow::on_actiondistance_triggered()
{
    //    struct geod_geodesic g;
    //    double s12;
    //    double pazi1;
    //    double pazi2;
    //    geod_init(&g, 6378137, 1/298.257223563);
    //    geod_inverse(&g, 40.64, -73.78, 1.36, 103.99, &s12,  &pazi1, &pazi2);
    //    printf("%.3f\n", s12 );
    //    printf("%.3f\n", pazi1 );
    //    printf("%.3f\n", pazi2 );
    vector<string>distancelist;
    flowcollection a=flowcollections[0];
    string str3;
    for(int i=0;i<a.Countflow();i++)
    {
        flowdata flow=a.Getflow(i);
        int od=flow.OID;
        int dd=flow.DID;
        double distance=auxiliary_func::get_collection_distance(od,dd,a.layerConnection);
        string str_od=to_string(od);
        string str_dd=to_string(dd);
        string str_distance=to_string(distance);
        str3=str_od+","+str_dd+","+str_distance;
        distancelist.push_back(str3);
    }
    for(int j=0;j<distancelist.size();j++)
    {
        cout<<distancelist[j]<<endl;
    }
}

void MainWindow::on_actiondirection_triggered()
{
    vector<string>directionlist;
    flowcollection a=flowcollections[0];
    string direction;
    for(int i=0;i<a.Countflow();i++)
    {
        flowdata flow=a.Getflow(i);
        int od=flow.OID;
        int dd=flow.DID;
        direction=auxiliary_func::get_collection_angle(od,dd,a.layerConnection);
        string str_od=to_string(od);
        string str_dd=to_string(dd);
        string str3=str_od+","+str_dd+","+direction;
        directionlist.push_back(str3);
    }
    for(int j=0;j<directionlist.size();j++)
    {
        cout<<directionlist[j]<<endl;
    }
}

void MainWindow::on_actionweight_triggered()
{
    flowcollection a=flowcollections[0];
    vector<string>weightlist;
    for(int i=0;i<a.Countflow();i++)
    {
        flowdata flow=a.Getflow(i);
        int od=flow.OID;
        int dd=flow.DID;
        double weight=flow.weight;
        string str_od=to_string(od);
        string str_dd=to_string(dd);
        string str_weight=to_string(weight);
        string str3=str_od+","+str_dd+","+str_weight;
        weightlist.push_back(str3);
    }
    for(int j=0;j<weightlist.size();j++)
    {
        cout<<weightlist[j]<<endl;
    }
}
#include <map>
//read flow
void MainWindow::on_actionflow_triggered()
{
    int aaa=0;
    QString fileName;
    fileName=QFileDialog::getOpenFileName(this,tr("文件"),"",tr("text(*.txt)"));
    string cFileName=fileName.toStdString();
    if(!fileName.isNull()){
        flowcollection temp_fcollection=flowcollection(cFileName);
        flowcollections.push_back(temp_fcollection);
    }
    else{
        qDebug()<<"取消";
    }
    //test net generation
    /*OGRLayer * aa= auxiliary_func::generateGrid(OGRPoint(0,0),OGRPoint(100,100),5);
    OGRFeature *poFeature;
n
    aa->ResetReading();
    while( (poFeature = aa->GetNextFeature()) != NULL )
    {
        aaa++;
    }
    int bbb=0;
    int ccc=0;*/
}



void MainWindow::on_actionod_shp_triggered()
{
    open_odshp *open=new open_odshp;
    open->exec();
    QString fileName;
    fileName=open->get_od_filename();
    QString fileName2;
    fileName2=open->get_shp_filename();
    string cFileName=fileName.toStdString();
    string cFileName2=fileName2.toStdString();
    /*QString fileName;
    fileName=QFileDialog::getOpenFileName(this,tr("OD文件"),"",tr("text(*.txt)"));
    string cFileName=fileName.toStdString();

    QString fileName2;
    fileName2=QFileDialog::getOpenFileName(this,tr("图层文件"),"",tr("shapefile(*.shp)"));
    string cFileName2=fileName2.toStdString();*/

    ODcollection tmpodc=ODcollection(cFileName);

    char c[1000];
    strcpy(c,cFileName2.c_str());
    GDALAllRegister();
    GDALDataset *poDS;
    poDS = (GDALDataset*) GDALOpenEx( c, GDAL_OF_VECTOR, NULL, NULL, NULL );
    if( poDS == NULL )
    {
        printf( "Open failed.\n" );
        exit( 1 );
    }
    OGRLayer *tmplayer=poDS->GetLayer(0);
    ODcollections.push_back(tmpodc);
    //cut from here
    flowcollection eee;
    flowcollections.push_back(eee);
    int index=flowcollections.size()-1;
    emit operate(tmpodc,tmplayer,&flowcollections[index]);
    //ui->statusbar->showMessage("working",0);
    ui->statusbar->showMessage("working");
    eee.ODconnection=&tmpodc;



}

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
    //dlg.exec();
    if (dlg.exec() == QDialog::Accepted) {
        // do something
        flowcollection taxiflows=flowcollections[0];
        od_graph.gen_flowgraph(&taxiflows);
    } else {
        // do something else
    }

}

void MainWindow::handleResults(const QString & aa)
{
    //ui->statusbar->showMessage("work finished",0);
    ui->statusbar->showMessage("job finished");
    /*flowcollection tmpfc=auxiliary_func::generateFlowcollection(tmpodc,tmplayer);
        tmpfc.ODconnection=&tmpodc;

        flowcollections.push_back(tmpfc);*/

    _flowviz->set_flowcollection(&flowcollections.front());
    _flowviz->repaint();

    base_list<<flowcollections.front().layerConnection->GetName();
    char chod[100];
    strcpy(chod,ODcollections.front().name.c_str());
    od_list<<chod;
    char chflow[100];
    strcpy(chflow,flowcollections.front().name.c_str());
    flow_list<<chflow;
    _leftbar->update_menu(od_list,flow_list,base_list);


    //_ptable->update_table(flowcollections.front().layerConnection);
    OGRLayer *polayer=flowcollections.front().layerConnection;

    OGRFeatureDefn *poFDefn=polayer->GetLayerDefn();
    int n=poFDefn->GetFieldCount();
    int m=int(polayer->GetFeatureCount());
    QTableWidget *_table=new QTableWidget(m, n);
    polayer->ResetReading();
    QStringList header;

    for(int i=0;i<n;i++)
    {
       header+=poFDefn->GetFieldDefn(i)->GetNameRef();
    }
    for(int i=0;i<m;i++)
    {
       OGRFeature *poFeature=polayer->GetNextFeature();
       for(int j=0;j<n;j++)
       {
         _table->setItem(i, j, new QTableWidgetItem(QString::fromLocal8Bit(poFeature->GetFieldAsString(j))));
       }
     }
       _table->setHorizontalHeaderLabels(header);
       _table->setStyleSheet("QTableWidget::item{border:1px solid;}");
       //_table->update();
       _table->show();
}
