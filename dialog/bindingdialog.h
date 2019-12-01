#ifndef BINDINGDIALOG_H
#define BINDINGDIALOG_H

#include <QDialog>

namespace Ui {
class BindingDialog;
}

class BindingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BindingDialog(QWidget *parent = nullptr);
    ~BindingDialog();
    void setFlowColList(QStringList qlist);
    void updateFlowColList();
    int getFlowColIdx();
    void setLayerList(QStringList qlist);
    void updateLayerList();
    int getLayerIdx();

private:
    Ui::BindingDialog *ui;
    QStringList FlowColList;
    QStringList LayerList;
};

#endif // BINDINGDIALOG_H
