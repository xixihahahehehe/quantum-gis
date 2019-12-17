#include "mini_map.h"
#include "ui_mini_map.h"

mini_map::mini_map(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mini_map)
{
    ui->setupUi(this);
}

mini_map::~mini_map()
{
    delete ui;
}
