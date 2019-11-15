#ifndef CHARTS_H
#define CHARTS_H
#include <QMainWindow>
#include <QWidget>
#include <QtCharts>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class Charts;
}

class Charts : public QMainWindow
{
    Q_OBJECT

public:
    explicit Charts(QWidget *parent = nullptr);
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
    void prepareData();

private slots:
    void slotPointHoverd(const QPointF &point, bool state);

private:
    Ui::Charts *ui;
    QLabel *m_valueLabel;
};

#endif // CHARTS_H
