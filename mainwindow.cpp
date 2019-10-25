#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "auxiliary_func.h"
#include "ogrsf_frmts.h"
#include <QFileDialog>
#include <QDebug>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    layercount=0;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actiontxt_triggered()
{

}

//这个 是 shp 那个选项的
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
        if (flowcollections.size()>0)
        {
            flowcollections[flowcollections.size()-1].setLayerConnection(tmp);
            _flowviz=new flow_viz(this);
            _flowviz->get_flowcollection(flowcollections.front());
            _flowviz->update();
            this->setCentralWidget(_flowviz);
        }
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

}

void MainWindow::on_actiondirection_triggered()
{

}

void MainWindow::on_actionweight_triggered()
{

}
#include <map>
//读取流数据
//读取流数据
void MainWindow::on_actionflow_triggered()
{
    int aaa=0;
    QString fileName;
    fileName=QFileDialog::getOpenFileName(this,tr("文件"),"",tr("text(*.txt)"));
    string cFileName=fileName.toStdString();
    if(!fileName.isNull()){
        flowcollection temp_fcollection=flowcollection(cFileName);
        if (myLayers.size()>0)
        {
            temp_fcollection.setLayerConnection(myLayers.front());

            _flowviz=new flow_viz(this);
            _flowviz->get_flowcollection(temp_fcollection);
            this->setCentralWidget(_flowviz);
            _flowviz->update();
        }
        flowcollections.push_back(temp_fcollection);



    }
    else{
        qDebug()<<"取消";
    }
    //这里 测试一下生成网格
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
    QString fileName;
    fileName=QFileDialog::getOpenFileName(this,tr("OD文件"),"",tr("text(*.txt)"));
    string cFileName=fileName.toStdString();

    QString fileName2;
    fileName2=QFileDialog::getOpenFileName(this,tr("图层文件"),"",tr("shapefile(*.shp)"));
    string cFileName2=fileName2.toStdString();

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
    flowcollection tmpfc=auxiliary_func::generateFlowcollection(tmpodc,tmplayer);
    tmpfc.ODconnection=&tmpodc;
    flowcollections.push_back(tmpfc);
}
