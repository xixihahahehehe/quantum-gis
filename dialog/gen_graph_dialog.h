#ifndef GEN_GRAPH_DIALOG_H
#define GEN_GRAPH_DIALOG_H

#include <QDialog>

namespace Ui {
class Gen_Graph_Dialog;
}

class Gen_Graph_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Gen_Graph_Dialog(QWidget *parent = nullptr);
    ~Gen_Graph_Dialog();
    void setFlowColList(QStringList qlist);
    void updateComboBx();
    int getFlowColIdx();

private:
    Ui::Gen_Graph_Dialog *ui;
    QStringList FlowColList;
};

#endif // GEN_GRAPH_DIALOG_H
