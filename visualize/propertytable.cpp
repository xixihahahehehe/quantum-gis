#include "propertytable.h"
#include "ui_propertytable.h"

PropertyTable::PropertyTable(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::PropertyTable)
{
    ui->setupUi(this);
    _table=new QTableWidget(this);
    this->setWidget(_table);
}

PropertyTable::~PropertyTable()
{
    delete ui;
}

void PropertyTable::update_table(OGRLayer *_ogrlayer)
{
    polayer=_ogrlayer;
    if(_table)
    {
        _table->clear();
    }
    polayer->ResetReading();
    OGRFeatureDefn *poFDefn=polayer->GetLayerDefn();
    int n=poFDefn->GetFieldCount();
    int m=int(polayer->GetFeatureCount());
    _table=new QTableWidget(m, n);
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
