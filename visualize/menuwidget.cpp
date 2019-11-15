#include "menuwidget.h"
#include "ui_menuwidget.h"
#include <QPalette>

MenuWidget::MenuWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::MenuWidget)
{
    ui->setupUi(this);
    _menu=new QListWidget();
    this->setWidget(_menu);

    //set the backcolor of QDockWidget
    QPalette pal;
    pal.setColor(QPalette::Background,QColor(100,100,100));
    setAutoFillBackground(true);
    setPalette(pal);
}

MenuWidget::~MenuWidget()
{
    delete ui;
}

void MenuWidget::update_menu(const QStringList &od_list, const QStringList &flow_list, const QStringList &base_list)
{
    if(_menu)
    {
      _menu->clear();
    }
    _menu->addItem("  OD");
    _menu->addItems(od_list);
    _menu->addItem("  Flow");
    _menu->addItems(flow_list);
    _menu->addItem("  Basemap");
    _menu->addItems(base_list);
}
