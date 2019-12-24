#ifndef OPEN_ODSHP_H
#define OPEN_ODSHP_H

#include <QDialog>
#include <QFileDialog>


namespace Ui {
class open_odshp;
}

class open_odshp : public QDialog
{
    Q_OBJECT

public:
    explicit open_odshp(QWidget *parent = nullptr);
    ~open_odshp();

    QString get_od_filename();
    QString get_shp_filename();
    int threadnum;

private slots:
    void on_od_pushButton_clicked();

    void on_shp_pushButton_clicked();

    void on_pushButton_clicked();

    void on_buttonBox_accepted();

private:
    Ui::open_odshp *ui;

    QString _od_filename;
    QString _shp_filename;
};

#endif // OPEN_ODSHP_H
