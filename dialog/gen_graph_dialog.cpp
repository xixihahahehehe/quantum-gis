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
