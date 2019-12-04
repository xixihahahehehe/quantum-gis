#include "graphexploredlg.h"
#include "ui_graphexploredlg.h"

GraphExploreDlg::GraphExploreDlg(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GraphExploreDlg)
{
    ui->setupUi(this);
}

GraphExploreDlg::~GraphExploreDlg()
{
    delete ui;
}
