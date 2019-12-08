#ifndef GRAPHEXPLOREDLG_H
#define GRAPHEXPLOREDLG_H

#include <QMainWindow>
#include<QtCharts>
QT_CHARTS_USE_NAMESPACE
#include<vector>
#include <QBarSeries>
#include <QBarSet>
#include "E:/GitHub/quantum-gis/method/histogram.h"

using namespace std;

namespace Ui {
class GraphExploreDlg;
}

class GraphExploreDlg : public QMainWindow
{
    Q_OBJECT

public:
    explicit GraphExploreDlg(QWidget *parent = nullptr);
    ~GraphExploreDlg();
	void GetData(vector<int>data);
	void InitChart();
	Histogram *histogram = nullptr;
	QChartView *mChartView = nullptr;
	QLabel*   m_tooltip = nullptr;
	QChart* createBarChart() const;
	void setGraphsList(QStringList qlist);
private:
    Ui::GraphExploreDlg *ui;
    QStringList IntervalList;
	QStringList ClassesList;
	QStringList GraphsList;
	QStringList PropertyList;
    bool hasreset = 0;


private slots:
	void sltTooltip(bool status, int index, QBarSet * barset);


    void on_classes_comboBx_currentIndexChanged(const QString &arg1);
    void on_interval_comboBx_currentIndexChanged(const QString &arg1);
};

#endif // GRAPHEXPLOREDLG_H
