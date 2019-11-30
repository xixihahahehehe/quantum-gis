#include "gen_graph_dialog.h"
#include "ui_gen_graph_dialog.h"

Gen_Graph_Dialog::Gen_Graph_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Gen_Graph_Dialog)
{
    ui->setupUi(this);
}

Gen_Graph_Dialog::~Gen_Graph_Dialog()
{
    delete ui;
}
void Gen_Graph_Dialog::setFlowColList(QStringList qlist)
{
    FlowColList = qlist;
}
void Gen_Graph_Dialog::updateComboBx()
{
    ui->comboBox->clear();
    ui->comboBox->addItems(FlowColList);
}
int Gen_Graph_Dialog::getFlowColIdx()
{
    return ui->comboBox->currentIndex();
}
