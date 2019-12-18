#include "charts.h"
#include "ui_charts.h"
#include <QChart>
#include <QChartView>
#include <mainwindow.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <map>
using namespace std;

Charts::Charts(QWidget *parent, vector<double> *list) :
    QMainWindow(parent),
    ui(new Ui::Charts)
{
    ui->setupUi(this);

    initChart();
    bulidChart();
    prepareData(*list);
}

Charts::~Charts()
{
    delete ui;
}

void Charts::initChart()
{
   // QtCharts::QChart *chart;
   // QtCharts::QChartView *chartview;
    m_valueLabel = new QLabel(this);
    m_valueLabel->setStyleSheet(QString("QLabel{color:#1564FF; font-family:\"Microsoft Yahei\"; font-size:12px; font-weight:bold;"
                                        " background-color:rgba(21, 100, 255, 51); border-radius:4px; text-align:center;}"));
    m_valueLabel->setFixedSize(44, 24);
    m_valueLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    m_valueLabel->hide();

    QChart *chart = new QChart();
    chart->legend()->setVisible(false);
    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);

    chart->setBackgroundBrush(QBrush(QColor(248, 251, 255)));
}



void Charts::bulidChart()
{
    QChart *chart = ui->chartView->chart();

    //spline chart
    QLineSeries *series0 = new QLineSeries();

    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(4);
    pen.setColor(QColor(21, 100, 255));
    series0->setPen(pen);

    //Scatter plot
    QScatterSeries *series1 = new QScatterSeries();
    series1->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    QRadialGradient radialGrad(QPointF(100, 100), 100);
         radialGrad.setColorAt(0, QColor(255, 255, 255));
         radialGrad.setColorAt(0.9, QColor(255, 255, 255));
         radialGrad.setColorAt(0.91, QColor(255, 0, 0));
         radialGrad.setColorAt(1, QColor(212, 227, 255));

    series1->setBrush(QBrush(radialGrad));
    series1->setMarkerSize(18);


    //Scatter plot
    QScatterSeries *series2 = new QScatterSeries();
    series2->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    series2->setBorderColor(QColor(21, 100, 255));
    series2->setBrush(QBrush(QColor(21, 100, 255)));
    series2->setMarkerSize(12);
    connect(series2, &QScatterSeries::hovered, this, &Charts::slotPointHoverd);


    //Scatter plot (for center)
    QScatterSeries *series3 = new QScatterSeries();
    series3->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    series3->setBorderColor(Qt::white);
    series3->setBrush(QBrush(Qt::white));
    series3->setMarkerSize(6);
    connect(series3, &QScatterSeries::hovered, this, &Charts::slotPointHoverd);

    chart->addSeries(series0);
    chart->addSeries(series1);
    chart->addSeries(series2);
    chart->addSeries(series3);

    //X
    QValueAxis *axisX = new QValueAxis();
    chart->addAxis(axisX,Qt::AlignBottom);
    //Y
    QValueAxis *axisY = new QValueAxis();
    chart->addAxis(axisY,Qt::AlignLeft);

    series0->attachAxis(axisX);
    series0->attachAxis(axisY);
    series1->attachAxis(axisX);
    series1->attachAxis(axisY);
    series2->attachAxis(axisX);
    series2->attachAxis(axisY);
    series3->attachAxis(axisX);
    series3->attachAxis(axisY);


  //X
    axisX->setLabelFormat("%d");
    //axisX->setTickCount(10);
    axisX->setGridLineVisible(false);
    axisX->setMinorGridLineVisible(false);

    //Y

    axisY->setLabelFormat("%.0f");
    axisY->setMinorTickCount(0);

    QPen axisPen;
    axisPen.setColor(QColor(231,238,251));
    axisPen.setStyle(Qt::DotLine);
    axisPen.setWidth(2);
//    axisY->setGridLinePen(axisPen);
    chart->legend()->setVisible(false);


}

void Charts::prepareData(vector<double> list)
{
    QChart *chart = ui->chartView->chart();
    QLineSeries *series0 = (QLineSeries *)ui->chartView->chart()->series().at(0);
    QScatterSeries *series2 = (QScatterSeries *)ui->chartView->chart()->series().at(2);
    QScatterSeries *series3 = (QScatterSeries *)ui->chartView->chart()->series().at(3);

    vector<double> vec=list;
    //sort(vec.begin(), vec.end());
    int maxValue = *max_element(vec.begin(),vec.end());
    int minValue = *min_element(vec.begin(),vec.end());
    lmin = minValue;
    lmax = maxValue;

    ListCount = 10;
    lowerOutlierCount = 0;
    upperOutlierCount = 0;

    if(lmax-lmin<=ListCount)
    {
        ListWidth=1;
        ListCount=(maxValue-minValue)/ListWidth+1;

    }
    else
    {
         ListWidth = (maxValue-minValue)/ListCount;
    }

    map<double, int> num_count;// empty

    for (int i = 0; i <list.size(); i++)
    {
        int stat=ceil((list[i]-minValue)/ListWidth)+1;
        if(stat < 0)
        {
            lowerOutlierCount++;
        }
        else if(stat > ListCount)
        {
            upperOutlierCount++;
        }
        else
        {
            int res=stat*ListWidth;
             ++num_count[res];
        }

    }

    map<double, int>::iterator iter;

    double minx=1000000, miny=1000000, maxx=0, maxy=0;

    for (iter = num_count.begin(); iter != num_count.end(); iter++)
    {
       cout << "[" << iter->first << "] = " << iter->second << endl;//iter->first:key,iter->second:value
       if(iter->first<minx)
       {
           minx=iter->first;
       }
       if(iter->first>maxx)
       {
           maxx=iter->first;
       }
       if(iter->second<miny)
       {
           miny=iter->second;
       }
       if(iter->second>maxy)
       {
           maxy=iter->second;
       }

       series0->append(iter->first, iter->second);
       series2->append(iter->first, iter->second);
       series3->append(iter->first, iter->second);
    }


     //X
     QValueAxis *axisX = new QValueAxis();
     chart->addAxis(axisX,Qt::AlignBottom);
     axisX->setTickCount(ListCount);
     //Y
     QValueAxis *axisY = new QValueAxis();
     chart->addAxis(axisY,Qt::AlignLeft);


    //cal
    chart->axisX()->setRange(minx, maxx);
    chart->axisY()->setRange(miny, maxy+1);


}



void Charts::slotPointHoverd(const QPointF &point, bool state)
{
    if (state) {
        m_valueLabel->setText(QString::asprintf("%1.0f%", point.y()));

        QPoint curPos = mapFromGlobal(QCursor::pos());
        m_valueLabel->move(curPos.x() - m_valueLabel->width() / 2, curPos.y() - m_valueLabel->height() * 1.5);

        m_valueLabel->show();


        QScatterSeries *series1 = (QScatterSeries *)ui->chartView->chart()->series().at(1);
        series1->clear();
        series1->append(point);
        series1->setVisible(true);
    }
    else {
        m_valueLabel->hide();

        QScatterSeries *series1 = (QScatterSeries *)ui->chartView->chart()->series().at(1);
        series1->setVisible(false);

    }


}

