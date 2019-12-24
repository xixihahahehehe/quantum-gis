#include "open_odshp.h"
#include "ui_open_odshp.h"


open_odshp::open_odshp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::open_odshp)
{
    ui->setupUi(this);
}

open_odshp::~open_odshp()
{
    delete ui;
}

void open_odshp::on_od_pushButton_clicked()
{
    QString od_filename;
    od_filename=QFileDialog::getOpenFileName(this,tr("OD文件"),"",tr("text(*.txt)"));
    ui->od_comboBox->setEditText(od_filename);
}

void open_odshp::on_shp_pushButton_clicked()
{
    QString shp_filename;
    shp_filename=QFileDialog::getOpenFileName(this,tr("图层文件"),"",tr("shapefile(*.shp)"));
    ui->shp_comboBox->setEditText(shp_filename);
}

void open_odshp::on_pushButton_clicked()
{
    _od_filename=ui->od_comboBox->currentText();
    _shp_filename=ui->shp_comboBox->currentText();
    threadnum=ui->comboBox->currentIndex()+1;
    this->close();
}


QString open_odshp::get_od_filename()
{
    return _od_filename;
}

QString open_odshp::get_shp_filename()
{
    return _shp_filename;
}

void open_odshp::on_buttonBox_accepted()
{
    _od_filename=ui->od_comboBox->currentText();
    _shp_filename=ui->shp_comboBox->currentText();
    threadnum=ui->comboBox->currentIndex()+1;
}
