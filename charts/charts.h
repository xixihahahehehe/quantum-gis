#ifndef CHARTS_H
#define CHARTS_H
#include <QMainWindow>
#include "ui_charts.h"
#include <QWidget>
#include <QtCharts>
#include <sstream>
#include <QString>

using namespace std;

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class Charts;
}

class Charts : public QMainWindow
{
    Q_OBJECT

public:
    explicit Charts(QWidget *parent = nullptr, vector<double> *list= new vector <double> );
    ~Charts();

private:
    /**
     * @brief initChart
     */
    void initChart();

    /**
     * @brief bulidChart
     */
    void bulidChart();

    /**
     * @brief prepareData
     */
    void prepareData(vector<double> list);
    double lmin,lmax;

    double ListWidth;
    int ListCount;
    int lowerOutlierCount, upperOutlierCount;
private slots:
    void slotPointHoverd(const QPointF &point, bool state);

private:
    Ui::Charts *ui;
    QLabel *m_valueLabel;
};

#endif // CHARTS_H
