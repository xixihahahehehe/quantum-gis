#ifndef PIETEST_H
#define PIETEST_H

#include "ui_pietest.h"
#include <QPainter>
#include <QRadialGradient>
#include "cpiewidget.h"

class pietest : public QMainWindow
{
	Q_OBJECT

public:
    pietest(QWidget *parent = nullptr, vector<double> *list= new vector <double>);
	~pietest();

private slots:
	void on_comboBox_currentIndexChanged ( int index );
	void on_checkBox_stateChanged ( int state );
	void on_allcheckBox_stateChanged ( int state );

private:	
	CPieWidget *piew;
private:
	Ui::pietestClass ui;
};

#endif // PIETEST_H
