#include "graphexploredlg.h"
#include "ui_graphexploredlg.h"

GraphExploreDlg::GraphExploreDlg(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GraphExploreDlg)
{
    ui->setupUi(this);
	QSplitter *splitterMain = new QSplitter(Qt::Vertical);
	splitterMain->addWidget(ui->groupBox);
	splitterMain->addWidget(ui->chartwidget);
	splitterMain->setStretchFactor(1, 1);
	setCentralWidget(splitterMain);
    ui->graph_comboBx->clear();
    QStringList qlist;
    qlist<<"bar"<<"line";
    ui->graph_comboBx->addItems(qlist);


}

GraphExploreDlg::~GraphExploreDlg()
{
	if (m_tooltip != nullptr)
		delete m_tooltip;
	//if (histogram != nullptr)
	//	delete histogram;
	for (int i = 0; i < n_properties; i++)
		delete Histogram_list[i];
	delete[] Histogram_list;
	if (mChartView != nullptr)
		delete mChartView;
    delete ui;
}

void GraphExploreDlg::setGraphsList(QStringList qlist)
{
	GraphsList = qlist;
	ui->graph_comboBx->clear();
	ui->graph_comboBx->addItems(GraphsList);
}


void GraphExploreDlg::sltTooltip(bool status, int index, QBarSet *barset)
{
	//Prompt for numeric text when mouse points to chart column
	QChart* pchart = mChartView->chart();
	if (m_tooltip == nullptr)
	{
		m_tooltip = new  QLabel(mChartView);    //Bar mouse tooltips
		m_tooltip->setStyleSheet("background: rgba(95,166,250,185);color: rgb(248, 248, 255);"
			"border:0px groove gray;border-radius:10px;padding:2px 4px;"
			"border:2px groove gray;border-radius:10px;padding:2px 4px");
		m_tooltip->setVisible(false);
	}
	if (status)
	{
		double val = barset->at(index);
		QPointF point(index, barset->at(index));
		QPointF pointLabel = pchart->mapToPosition(point);
        double binwidth = histogram->getBinWidth();
        QString sText = QString("degree: [%1,%3)\nfrequency: %2").arg(index*binwidth).arg(val).arg(index*binwidth + binwidth);

		m_tooltip->setText(sText);
		m_tooltip->move(pointLabel.x(), pointLabel.y() - m_tooltip->height()*1.5);
		m_tooltip->show();
	}
	else
	{
		m_tooltip->hide();
	}
}
void GraphExploreDlg::GetData(flowgraph& od_graph)
{
    n_properties = 5;
	Histogram_list = new Histogram*[n_properties];
    vector<int>data;
	od_graph.get_degree(data);

	Histogram_list[0] = new Histogram(data);
	od_graph.get_strength(data);
	Histogram_list[1] = new Histogram(data);
    vector<double>data2;
    od_graph.get_betweenness(data2);
    Histogram_list[2] = new Histogram(data2);
    od_graph.get_closeness(data2);
    Histogram_list[3] = new Histogram(data2);
    od_graph.get_pagerank(data2);
    Histogram_list[4] = new Histogram(data2);

	PropertyList.clear();
    PropertyList << "Degree" << "Strength"<<"Betweeness"<<"Closeness"<<"PageRank";
	ui->property_comboBx->clear();
	ui->property_comboBx->addItems(PropertyList);

	//if (histogram != nullptr)
	//{
	//	delete histogram;
	//	histogram = new Histogram(data);
	//}
	//else
	//	histogram = new Histogram(data);
	


    //on_classes_comboBx_currentIndexChanged(QString::number(init_classes));
    //ui->classes_comboBx->currentIndexChanged(histogram->bins()-1);
}

void GraphExploreDlg::InitChart()
{
	QGridLayout *baseLayout = new QGridLayout();
	mChartView = new QChartView(createBarChart());  //QChartView *mChartView;
	baseLayout->addWidget(mChartView, 1, 0);
	//ui->groupBox->setLayout(baseLayout);
	//ui->centralwidget->setLayout(baseLayout);
	ui->chartwidget->setLayout(baseLayout);
	has_initchart = 1;
}
QChart* GraphExploreDlg::createBarChart() 
{
	QChart *chart = new QChart();
	chart->setTitle(ui->property_comboBx->currentText()+" Distribution");
    chart->layout()->setContentsMargins(0, 0, 0, 0);
    //   chart->setMargins(QMargins(0, 0, 0, 0));//
    chart->setBackgroundRoundness(0);
	QBarSeries *series = new QBarSeries(chart);
	vector<int> v_hist;
	//histogram->ResetHistogram(-1, 3);
	histogram = (Histogram_list[ui->property_comboBx->currentIndex()]);
	histogram->getHistVec(v_hist);
	QBarSet *set0 = new QBarSet(ui->property_comboBx->currentText());
	for (int i = 0; i < v_hist.size(); i++)
	{
		*set0 << v_hist[i];
	}


	series->append(set0);

	chart->addSeries(series);
	connect(series, SIGNAL(hovered(bool, int, QBarSet*)), this, SLOT(sltTooltip(bool, int, QBarSet*)));
	//chart->createDefaultAxes();
	QValueAxis *axisX = new QValueAxis;
	chart->addAxis(axisX, Qt::AlignBottom);
	QValueAxis *axisY = new QValueAxis;
	chart->addAxis(axisY, Qt::AlignLeft);
	series->attachAxis(axisX);
	series->attachAxis(axisY);

	chart->setAnimationOptions(QChart::SeriesAnimations);
	chart->legend()->setVisible(false);
	return chart;
}
void GraphExploreDlg::on_classes_comboBx_currentIndexChanged(const QString &arg1)
{
    if(!hasreset)
    {
        hasreset=1;
        int classes = arg1.toInt();
       // int classes=ui->classes_comboBx->currentData().toInt();
        histogram ->ResetHistogram(classes,-1);
        QChart *chart = mChartView->chart();
        chart->removeAllSeries();
        int graph_type = ui->graph_comboBx->currentIndex();
        vector<int> v_hist;
        histogram->getHistVec(v_hist);
        if(graph_type==0)
        {
            QBarSeries *series = new QBarSeries(chart);
            QBarSet *set0 = new QBarSet(ui->property_comboBx->currentText());
            for (int i = 0; i < v_hist.size(); i++)
            {
                *set0 << v_hist[i];
            }
            series->append(set0);
            chart->addSeries(series);
            connect(series, SIGNAL(hovered(bool, int, QBarSet*)), this, SLOT(sltTooltip(bool, int, QBarSet*)));
            ui->interval_comboBx->setCurrentText(QString::number((int)histogram->getBinWidth()));
         }
        else if(graph_type==1)
        {
            QLineSeries *series = new QLineSeries();
            series->setName(ui->property_comboBx->currentText());
            for (int i = 0; i < v_hist.size(); i++)
            {
                series->append(QPoint(i, v_hist[i]));
            }
            chart->addSeries(series);
        }

        int maxValue = *max_element(v_hist.begin(), v_hist.end());
        chart->axisY()->setRange(0, maxValue);
        qDebug()<<histogram->getBinWidth();
        hasreset=0;


    }
}

void GraphExploreDlg::on_interval_comboBx_currentIndexChanged(const QString &arg1)
{
    if(!hasreset)
    {
         hasreset=1;

        int interval = arg1.toInt();
       // int classes=ui->classes_comboBx->currentData().toInt();
        histogram ->ResetHistogram(-1,interval);
        QChart *chart = mChartView->chart();
        chart->removeAllSeries();
        int graph_type = ui->graph_comboBx->currentIndex();
        vector<int> v_hist;
        histogram->getHistVec(v_hist);
        if(graph_type==0)
        {
            QBarSeries *series = new QBarSeries(chart);
            QBarSet *set0 = new QBarSet(ui->property_comboBx->currentText());
            for (int i = 0; i < v_hist.size(); i++)
            {
                *set0 << v_hist[i];
            }
            series->append(set0);
            chart->addSeries(series);
            connect(series, SIGNAL(hovered(bool, int, QBarSet*)), this, SLOT(sltTooltip(bool, int, QBarSet*)));
        }
       else if(graph_type==1)
       {
           QLineSeries *series = new QLineSeries();
           series->setName(ui->property_comboBx->currentText());
           for (int i = 0; i < v_hist.size(); i++)
           {
               *series<<QPoint(i, v_hist[i]);
           }
           chart->addSeries(series);
       }

		int maxValue = *max_element(v_hist.begin(), v_hist.end());
		chart->axisY()->setRange(0, maxValue);

        ui->classes_comboBx->setCurrentText(QString::number(histogram->bins()));
        hasreset=0;
    }
}

void GraphExploreDlg::on_property_comboBx_currentIndexChanged(const QString &arg1)
{
    histogram = Histogram_list[ui->property_comboBx->currentIndex()];
    if(ui->property_comboBx->currentIndex()<=1)
    {
        ClassesList.clear();
        int init_classes = histogram->upperBound() - histogram->lowerBound()+1;
        int interval = 1;
        int range = init_classes<2000?init_classes:2000;
        for (int i = 1; i <= range + 1; i++)
            ClassesList << QString("%1").arg(i);
        IntervalList.clear();
        for (int i = 1; i <= histogram->upperBound() - histogram->lowerBound(); i+=(init_classes < 100?interval:(init_classes / 100 )))
            IntervalList << QString("%1").arg(i);

        if (!has_initchart)
        {
            InitChart();
        }

        hasreset = 1;
        ui->classes_comboBx->clear();
        ui->classes_comboBx->addItems(ClassesList);
        ui->classes_comboBx->setCurrentIndex(init_classes - 1);
        hasreset = 0;
        if (init_classes > 100)
            interval *= init_classes / 100;
        ui->interval_comboBx->clear();
        ui->interval_comboBx->addItems(IntervalList);
        ui->interval_comboBx->setCurrentIndex(interval - 1);
        mChartView->chart()->axisX()->setRange(histogram->lowerBound(), histogram->upperBound());
    }
    else
    {
        ClassesList.clear();
        int init_classes = 101;
        double interval = (histogram->upperBound() - histogram->lowerBound())/100;
        for (int i = 1; i <= init_classes + 1; i++)
            ClassesList << QString("%1").arg(i);
        IntervalList.clear();
        for (double i = interval; i <= histogram->upperBound() - histogram->lowerBound(); i+=interval)
            IntervalList << QString("%1").arg(i);//QString::number(i,'f',4);

        if (!has_initchart)
        {
            InitChart();
        }

        hasreset = 1;
        ui->classes_comboBx->clear();
        ui->classes_comboBx->addItems(ClassesList);
        ui->classes_comboBx->setCurrentIndex(init_classes - 1);
        hasreset = 0;
        if (init_classes > 101)
            interval *= init_classes / 100;
        ui->interval_comboBx->clear();
        ui->interval_comboBx->addItems(IntervalList);
        ui->interval_comboBx->setCurrentIndex(interval - 1);
        mChartView->chart()->axisX()->setRange(histogram->lowerBound(), histogram->upperBound());
    }

}
void GraphExploreDlg::on_graph_comboBx_currentIndexChanged(const QString &arg1)
{

}
