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
    connect(_menu,&QListWidget::itemSelectionChanged,this,&MenuWidget::update_selecteditems);

}

MenuWidget::~MenuWidget()
{
    delete ui;
    menu_odcollection.clear();
    menu_flowcollection.clear();
    menu_layer.clear();
    delete _fviz;
    delete _menu;
}

void MenuWidget::update_menu(list<ODcollection*> _odcollection,list<flowcollection*> _flowcollection,list<OGRLayer*> _basemap,vector<int> selected_OD,vector<int> selected_flow,vector<int> selected_base)
{
    menu_odcollection=_odcollection;
    menu_flowcollection=_flowcollection;
    menu_layer=_basemap;
    od_selected=selected_OD;
    flow_selected=selected_flow;
    base_selected=selected_base;
    update_info();
}

void MenuWidget::update_info()
{
    if(_menu)
    {
      _menu->clear();
    }

    //update menu for OD info
    QListWidgetItem *item_OD=new QListWidgetItem(tr("OD"),_menu);
    label_OD=0;
    std::list<ODcollection *>::iterator it_OD;
    for(it_OD=menu_odcollection.begin();it_OD!=menu_odcollection.end();it_OD++)
    {
        QListWidgetItem *temp_item=new QListWidgetItem((*it_OD)->GetName().c_str(),_menu);
        temp_item->setCheckState(Qt::Unchecked);
    }
    //update menu of flow info
    QListWidgetItem *item_Flow=new QListWidgetItem(tr("Flow"),_menu);
    label_flow=menu_odcollection.size()+1;
    std::list<flowcollection *>::iterator it_Flow;
    for (it_Flow=menu_flowcollection.begin();it_Flow!=menu_flowcollection.end();it_Flow++)
    {
        QListWidgetItem *temp_item=new QListWidgetItem((*it_Flow)->GetName().c_str(),_menu);
        temp_item->setCheckState(Qt::Unchecked);
    }
    //update menu of basemap info
    QListWidgetItem *item_Baemap=new QListWidgetItem(tr("Basemap"),_menu);
    label_base=menu_odcollection.size()+menu_flowcollection.size()+2;
    std::list<OGRLayer *>::iterator it_layer;
    for (it_layer=menu_layer.begin();it_layer!=menu_layer.end();it_layer++)
    {
        QListWidgetItem *temp_item=new QListWidgetItem((*it_layer)->GetName(),_menu);
        temp_item->setCheckState(Qt::Unchecked);
    }
    for(int i=0;i<od_selected.size();i++)
    {
        _menu->item(od_selected.at(i))->setCheckState(Qt::Checked);
    }
    for(int i=0;i<flow_selected.size();i++)
    {
        _menu->item(flow_selected.at(i)+menu_odcollection.size()+1)->setCheckState(Qt::Checked);
    }
    for(int i=0;i<base_selected.size();i++)
    {
        _menu->item(menu_flowcollection.size()+menu_odcollection.size()+base_selected.at(i)+2)->setCheckState(Qt::Checked);
    }
}

void MenuWidget::update_selecteditems()
{
    od_selected.clear();
    flow_selected.clear();
    base_selected.clear();
    _fviz->od_draw=false;
    _fviz->flow_draw=false;
    _fviz->map_draw=false;
    for (int i=0;i<_menu->count();i++)
    {
        if(_menu->item(i)->checkState()==Qt::Checked)
        {
            if (i>label_OD & i<label_flow)
            {
               od_selected.push_back(i);
               std::list<ODcollection*>::iterator it_od;
               int j=0;
               for (it_od=menu_odcollection.begin();it_od!=menu_odcollection.end();it_od++)
               {
                   ++j;
                   if (j==i)
                   {
                       _fviz->set_odcollection((*it_od));
                       _fviz->od_draw=true;
                   }
               }
            }
            else if(i>label_flow & i<label_base)
            {
                flow_selected.push_back(i-label_flow);
                std::list<flowcollection*>::iterator it_flow;
                int j=0;
                for (it_flow=menu_flowcollection.begin();it_flow!=menu_flowcollection.end();it_flow++)
                {
                    ++j;
                    if (j==i-label_flow)
                    {
                        _fviz->set_flowcollection((*it_flow));
                        _fviz->flow_draw=true;
                    }
                }
            }
            else
            {
                base_selected.push_back(i-label_base);
                std::list<OGRLayer*>::iterator it_base;
                int j=0;
                for (it_base=menu_layer.begin();it_base!=menu_layer.end();it_base++)
                {
                    ++j;
                    if (j==i-label_base)
                    {
                        _fviz->map_draw=true;
                        //adjust flow_viz for single basemap visualize
                    }
                }
            }
        }
    } 
    _fviz->update();
}

void MenuWidget::update_drawing()
{

}
