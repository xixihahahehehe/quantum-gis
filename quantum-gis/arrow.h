#ifndef ARROW_H
#define ARROW_H

#include <QLineF>
#include <QPointF>
#include <QColor>

class QPainter;

class Arrow : public QLineF
{
private:
    QPointF start_point;
    QPointF end_point;

public:
    Arrow(QPointF startPoint,QPointF endPoint);
    ~Arrow();
    void paint(QPainter *painter,QColor color);
};

#endif // ARROW_H
