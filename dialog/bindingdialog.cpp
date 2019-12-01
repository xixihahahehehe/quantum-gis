#include "bindingdialog.h"
#include "ui_bindingdialog.h"

BindingDialog::BindingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BindingDialog)
{
    ui->setupUi(this);
}

BindingDialog::~BindingDialog()
{
    delete ui;
}


void BindingDialog::setFlowColList(QStringList qlist)
{
    FlowColList = qlist;

}
void BindingDialog::updateFlowColList()
{
    ui->comboBox->clear();
    ui->comboBox->addItems(FlowColList);

}
int BindingDialog::getFlowColIdx()
{
    return ui->comboBox->currentIndex();

}
void BindingDialog::setLayerList(QStringList qlist)
{
    LayerList = qlist;

}
void BindingDialog::updateLayerList()
{
    ui->comboBox_2->clear();
    ui->comboBox_2->addItems(LayerList);

}
int BindingDialog::getLayerIdx()
{
    return ui->comboBox_2->currentIndex();
}
