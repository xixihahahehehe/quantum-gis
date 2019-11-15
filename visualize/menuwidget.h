#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QDockWidget>
#include <QListWidget>
#include <QStringList>
#include "collection/flowcollection.h"

namespace Ui {
class MenuWidget;
}

class MenuWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit MenuWidget(QWidget *parent = nullptr);
    ~MenuWidget();
    void update_menu(const QStringList &od_list,const QStringList &flow_list,const QStringList &base_list);

private:
    Ui::MenuWidget *ui;
    QListWidget *_menu;
};

#endif // MENUWIDGET_H
