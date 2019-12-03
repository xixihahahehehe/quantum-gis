#include "menuwidget.h"
#include "ui_menuwidget.h"
#include <QPalette>
#include <QCheckBox>

MenuWidget::MenuWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::MenuWidget)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Menu"));
    QListWidgetItem *item_OD=new QListWidgetItem(tr("OD"));
    QListWidgetItem *item_Flow=new QListWidgetItem(tr("Flow"));
    QListWidgetItem *item_Basemap=new QListWidgetItem(tr("Basemap"));
    _menu=new QListWidget();
    _menu->addItem(item_OD);
    _menu->addItem(item_Flow);
    _menu->addItem(item_Basemap);
    this->setWidget(_menu);

    //set the backcolor of QDockWidget
    QPalette pal;
    pal.setColor(QPalette::Background,QColor(200,200,200));
    setAutoFillBackground(true);
    setPalette(pal);
}

MenuWidget::~MenuWidget()
{
    delete ui;
}

void MenuWidget::update_menu(QStringList od_list, QStringList flow_list, QStringList base_list, vector<int> od_selected, vector<int> flow_selected, vector<int> base_selected)
{
    if(_menu)
    {
      _menu->clear();
    }

    //update menu for OD info
    QListWidgetItem *item_OD=new QListWidgetItem(tr("OD"),_menu);
    //std::list<ODcollection *>::iterator it_OD;
    for(int i=0;i<od_list.size();i++)
    {
        QListWidgetItem *temp_item=new QListWidgetItem(od_list.at(i),_menu);
    }
    //update menu of flow info
    QListWidgetItem *item_Flow=new QListWidgetItem(tr("Flow"),_menu);
    //std::list<flowcollection *>::iterator it_Flow;
    for (int i=0;i<flow_list.size();i++)
    {
        QListWidgetItem *temp_item=new QListWidgetItem(flow_list.at(i),_menu);
        //temp_item->setFlags(Qt::ItemIsUserCheckable);
        //temp_item->setCheckState(Qt::Unchecked);
    }
    //update menu of basemap info
    QListWidgetItem *item_Baemap=new QListWidgetItem(tr("Basemap"),_menu);
    for (int i=0;i<base_list.size();++i)
    {
        QListWidgetItem *temp_item=new QListWidgetItem(base_list.at(i),_menu);
        //temp_item->setFlags(Qt::ItemIsUserCheckable);
        //temp_item->setCheckState(Qt::Unchecked);
    }
    for(int i=0;i<od_selected.size();i++)
    {
        _menu->item(od_selected.at(i))->setCheckState(Qt::Checked);
    }
    for(int i=0;i<flow_selected.size();i++)
    {
        _menu->item(flow_selected.at(i)+od_list.size()+1)->setCheckState(Qt::Checked);
    }
    for(int i=0;i<base_selected.size();i++)
    {
        _menu->item(flow_list.size()+od_list.size()+base_selected.at(i)+2)->setCheckState(Qt::Checked);
    }
}
