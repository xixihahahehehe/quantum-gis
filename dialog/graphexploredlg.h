#ifndef GRAPHEXPLOREDLG_H
#define GRAPHEXPLOREDLG_H

#include <QMainWindow>

namespace Ui {
class GraphExploreDlg;
}

class GraphExploreDlg : public QMainWindow
{
    Q_OBJECT

public:
    explicit GraphExploreDlg(QWidget *parent = nullptr);
    ~GraphExploreDlg();

private:
    Ui::GraphExploreDlg *ui;
};

#endif // GRAPHEXPLOREDLG_H
