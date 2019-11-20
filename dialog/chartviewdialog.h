#ifndef CHARTVIEWDIALOG_H
#define CHARTVIEWDIALOG_H
#include <QtCharts>
#include<vector>
using namespace std;
namespace Ui {
class ChartViewDialog;
}

class ChartViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChartViewDialog(QWidget *parent = nullptr);
    ~ChartViewDialog();
    vector<vector<double>> res;
    void getdata(vector<vector<double>> res0);
    void draw();

private:
    Ui::ChartViewDialog *ui;
};

#endif // CHARTVIEWDIALOG_H
