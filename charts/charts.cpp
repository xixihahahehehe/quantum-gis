#include "charts.h"
#include "ui_charts.h"
#include <QChart>
#include <QChartView>

Charts::Charts(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Charts)
{
    ui->setupUi(this);

    initChart();
    bulidChart();
    prepareData();
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
    chart->removeAllSeries();
    chart->removeAxis(chart->axisX());
    chart->removeAxis(chart->axisY());

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
    QCategoryAxis *axisX = new QCategoryAxis();
    axisX->setMin(0);
    axisX->setMax(16);

    axisX->setStartValue(5);

    axisX->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
    for(int i=0; i<16; i++) {
        QString str = QString("%1").arg(i+1);
        axisX->append(str, i);
    }

    axisX->setTickCount(32);
    axisX->setGridLineVisible(false);
    axisX->setMinorGridLineVisible(false);

    //Y
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 100);
    axisY->setTickCount(6);
    axisY->setLabelFormat("%.0f");
    axisY->setMinorTickCount(0);

    QPen axisPen;
    axisPen.setColor(QColor(231,238,251));
    axisPen.setStyle(Qt::DotLine);
    axisPen.setWidth(2);
    axisY->setGridLinePen(axisPen);

    chart->setAxisX(axisX, series0);
    chart->setAxisY(axisY, series0);

    chart->setAxisX(axisX, series1);
    chart->setAxisY(axisY, series1);

    chart->setAxisX(axisX, series2);
    chart->setAxisY(axisY, series2);

    chart->setAxisX(axisX, series3);
    chart->setAxisY(axisY, series3);


}


void Charts::prepareData()
{
    //QLineSeries *series0 = (QLineSeries *)ui->chartView->chart()->series().at(0);
    //QScatterSeries *series2 = (QScatterSeries *)ui->chartView->chart()->series().at(2);
    //QScatterSeries *series3 = (QScatterSeries *)ui->chartView->chart()->series().at(3);
    QLineSeries *series0 = (QLineSeries *)ui->chartView->chart()->series().at(0);
    QScatterSeries *series2 = (QScatterSeries *)ui->chartView->chart()->series().at(2);
    QScatterSeries *series3 = (QScatterSeries *)ui->chartView->chart()->series().at(3);

    series0->clear();
    series2->clear();
    series3->clear();

    qsrand(QTime::currentTime().second());

    qreal t=0, y1, intv=1;
    qreal rd;
    int cnt=16;
    for (int i=0; i<cnt; i++)
    {
        rd = (qrand() % 100);
        y1=rd;
        series0->append(t, y1);
        series2->append(t, y1);
        series3->append(t, y1);

        t+=intv;
    }

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


