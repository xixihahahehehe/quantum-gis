#include "chartviewdialog.h"
QT_CHARTS_USE_NAMESPACE
#include <QDialog>

#include "ui_chartviewdialog.h"

ChartViewDialog::ChartViewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChartViewDialog)
{
    ui->setupUi(this);
//    QChart* chart = new QChart();
//    QLineSeries *series = new QLineSeries();

//    for (quint32 i = 0; i < 100; i++) {

//        series->append(i, sin(0.6f*i));

//    }
//    chart->addSeries(series);

//    chart->createDefaultAxes();
//    ui->chartwidget->setChart(chart);

}

ChartViewDialog::~ChartViewDialog()
{
    delete ui;
}
void ChartViewDialog::draw()
{
    QSplineSeries *series = new QSplineSeries();
    QScatterSeries *series0 = new QScatterSeries();
    QScatterSeries *series1 = new QScatterSeries();
    series0->setName("scatter1");
    series0->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    series0->setMarkerSize(1);
    for (int i = 0; i < res[0].size(); i++)
    {
        series0->append(log(res[0][i]), log(res[1][i]));
        series1->append(res[0][i],res[1][i]);
    }
    QChart *chart = new QChart();
    double x_max = log(*max_element(res[0].begin(), res[0].end()));
    double x_min = log(*min_element(res[0].begin(), res[0].end()));
    series->append(x_min, -res[2][0]*x_min+ res[2][1]);
    series->append(x_max, -res[2][0] * x_max + res[2][1]);
    //chart->addSeries(series1);
    chart->addSeries(series0);
    chart->addSeries(series);
    chart->legend()->hide();
    chart->createDefaultAxes();
    //chart->axisY()->setRange(0, 0.007);
    //chart->setTheme(QChart::ChartThemeDark);
    ui->chartwidget->setChart(chart);
}
void ChartViewDialog::getdata(vector<vector<double> > res0)
{
    res = res0;
}
