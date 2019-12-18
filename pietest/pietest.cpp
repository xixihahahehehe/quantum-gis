#include "pietest.h"


pietest::pietest(QWidget *parent, vector<double> *list)
    : QMainWindow(parent)
{
	ui.setupUi(this);

    piew = new CPieWidget(this, list);
	ui.gridLayout_2->addWidget(piew, 0, 0, 1, 1);

}

pietest::~pietest()
{

}

void pietest::on_comboBox_currentIndexChanged( int index )
{
	piew->setPieStyleSheet((PieStyle)index);
	piew->update();
}

void pietest::on_checkBox_stateChanged( int state )
{
	if (state==2)
		piew->setExplodedIndex(ui.comboBox->currentIndex()+1);
	else
		piew->setExplodedIndex(-1);
	piew->update();

}

void pietest::on_allcheckBox_stateChanged( int state )
{
	if (state==2)
		piew->setExplodedAll(true);
	else
		piew->setExplodedAll(false);
	piew->update();

}
