#ifndef PROPERTYTABLE_H
#define PROPERTYTABLE_H

#include <QDockWidget>
#include <QTableWidget>
#include "collection/flowcollection.h"

namespace Ui {
class PropertyTable;
}

class PropertyTable : public QDockWidget
{
    Q_OBJECT

public:
    explicit PropertyTable(QWidget *parent = nullptr);
    ~PropertyTable();
    void update_table(OGRLayer *_ogrlayer);


private:
    Ui::PropertyTable *ui;
    QTableWidget *_table;
    OGRLayer *polayer;
};

#endif // PROPERTYTABLE_H
