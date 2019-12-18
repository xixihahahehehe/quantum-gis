#include <mainwindow.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include "cpiewidget.h"
using namespace std;


CPieWidget::CPieWidget(QWidget *parent, vector<double> *list)
	: QWidget(parent)
{
	
	resize(600,500);
//    setData();
    init(*list);
}

CPieWidget::~CPieWidget()
{

}
void CPieWidget::setData( QHash<QString,float> datamap )
{
    m_datamap.clear();
    m_datamap = datamap;


}

void CPieWidget::addData( QString title,float value )
{
    m_datamap.insertMulti(title,value);
}

void CPieWidget::removeData( QString title )
{
	m_datamap.remove(title);
}

void CPieWidget::removeAll()
{
	m_datamap.clear();
}


QHash<QString,float> CPieWidget::getDataMap()
{
	return m_datamap;

}

void CPieWidget::setColors( QList<QColor> colorlist )
{
	for (int i = colorlist.count()-1; i > -1; --i)
	{
		m_colorlist.push_front(colorlist.at(i));
	}
}


void CPieWidget::setPieStyleSheet( PieStyle piestyle /*= Default*/ )
{
	m_piestyle = piestyle;
}

void CPieWidget::setExplodedAll( bool isExploded /*= true*/ )
{
	m_isexploded = isExploded;
}

void CPieWidget::setExplodedIndex( int index )
{
	m_explodedindex = index;
}


void CPieWidget::init(vector<double> list)
{
	setPieStyleSheet(RingShadingDount);
	m_explodedindex = -1;
	m_isexploded = false;

	
    vector<double> vec=list;
    //sort(vec.begin(), vec.end());
    int maxValue = *max_element(vec.begin(),vec.end());
    int minValue = *min_element(vec.begin(),vec.end());
    pmin = minValue;
    pmax = maxValue;


    PieCount = 10;
    lowerOutlierCount = 0;
    upperOutlierCount = 0;

    if(pmax-pmin<=PieCount)
    {
        PieWidth=1;
        PieCount=(maxValue-minValue)/PieWidth+1;

    }
    else
    {
         PieWidth = (maxValue-minValue)/PieCount;
    }

    map<double, int> num_count;// empty

    for (int i = 0; i <list.size(); i++)
    {
        int stat=ceil((list[i]-minValue)/PieWidth)+1;
        if(stat < 0)
        {
            lowerOutlierCount++;
        }
        else if(stat > PieCount)
        {
            upperOutlierCount++;
        }
        else
        {
            int res=stat*PieWidth;
             ++num_count[res];
        }

    }

    map<double, int>::iterator iter;

//    double minx=1000000, miny=1000000, maxx=0, maxy=0;

    for (iter = num_count.begin(); iter != num_count.end(); iter++)
    {
        cout << "[" << iter->first << "] = " << iter->second << endl;//iter->first:key,iter->second:value
        m_datamap.insertMulti(QString::number(iter->first,'f',10),iter->second);

        //       if(iter->first<minx)
        //       {
        //           minx=iter->first;
        //       }
        //       if(iter->first>maxx)
        //       {
        //           maxx=iter->first;
        //       }
        //       if(iter->second<miny)
        //       {
        //           miny=iter->second;
        //       }
        //       if(iter->second>maxy)
        //       {
        //           maxy=iter->second;
        //       }

   }

    m_colorlist << QColor(77,82,175);
    m_colorlist << QColor(158,102,155);
    m_colorlist << QColor(168,209,207);
    m_colorlist << QColor(172,126,126);
    m_colorlist << QColor(90,179,86);
    m_colorlist << QColor(238,187,34);
    m_colorlist << QColor(102,170,238);
    m_colorlist << QColor(136,68,255);
    m_colorlist << QColor(116,169,26);
    m_colorlist << QColor(201,64,73);


}


void CPieWidget::paintEvent( QPaintEvent *e )
{
	QPainter *painter = new QPainter(this);
	painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::TextAntialiasing, true) ;
	painter->setRenderHint(QPainter::SmoothPixmapTransform);
	
	drawEllipse(painter);

	
}

void CPieWidget::drawEllipse( QPainter *painter )
{
	switch (m_piestyle)
	{
		case DefaultPie: drawDefaultPie(painter); break;
		case ConcaveShading:drawConcaveShading(painter);break;
		case RadialGradient:drawRadialGradient(painter);break;
		case DountDefault: drawDountDefault(painter);break;
		case RingShadingDount: drawRingShadingDount(painter);break;
		case ConcaveShadingDount: drawConcaveShadingDount(painter);break;
		//case ExplodedDount: drawExplodedDount(painter);break;
		default:drawDefaultPie(painter);break;
	}
}

void CPieWidget::drawDefaultPie( QPainter *painter )
{
	qreal sum = getSumValue();
	int w = width();
	int h = height();
    int radius = qMin(w,h)*0.8; //
	QRect rect(w/2-radius/2,h/2-radius/2,radius,radius);

	painter->save();
	painter->setPen(Qt::NoPen);
	QHashIterator<QString, float> i(m_datamap);
	
    qreal index = 30;  //
	int colorindex=0;
	QStringList keylist = m_datamap.keys();
	for (int i = 0; i < keylist.count(); ++i)
	{
		qreal v = m_datamap.value(keylist.at(i));
		v =v/sum*(360);
		QRect newrect = rect;
		if (m_explodedindex == i || m_isexploded)
		{
			QPoint newcenter = newrect.center();
			int midangel = index+v/2;
			QPoint tp = getMovePoint(midangel);
			newcenter += tp;
			newrect.moveCenter(newcenter);
		}
        QFont font;
        font.setFamily("Microsoft YaHei");
        font.setBold(true);
        font.setPointSize(20);
        int spacing=10;
        font.setLetterSpacing(QFont::AbsoluteSpacing, spacing);
        painter->setFont(font);
        painter->drawText(rect,Qt::AlignTop,"piechart");
        painter->begin(this);
        QPoint centerPoint = newrect.center();
	 	QColor firstColor = m_colorlist.at(colorindex);
	 	QRadialGradient firstGradient(centerPoint, radius/2);
 	 	firstGradient.setColorAt(0, firstColor.lighter(80));
	 	firstGradient.setColorAt(1.0, firstColor.darker(80));
	 	painter->setBrush(firstGradient);
		painter->drawPie(newrect, index * 16, v * 16);

		index+=v;
		colorindex++;
		if (colorindex==m_colorlist.count())
		{
			colorindex = 0;
		}
	}
	painter->restore();
}


void CPieWidget::drawConcaveShading( QPainter *painter )
{
	qreal sum = getSumValue();
	int w = width();
	int h = height();
    int radius = qMin(w,h)*0.8; //
	QRect rect(w/2-radius/2,h/2-radius/2,radius,radius);

	painter->save();
	painter->setPen(Qt::NoPen);
	QHashIterator<QString, float> i(m_datamap);

    qreal index = 30;  //start location
	int colorindex=0;
	QStringList keylist = m_datamap.keys();
	for (int i = 0; i < keylist.count(); ++i)
	{
		qreal v = m_datamap.value(keylist.at(i));
		v =v/sum*(360);
		QRect newrect = rect;
		if (m_explodedindex == i || m_isexploded)
		{
			QPoint newcenter = newrect.center();
			int midangel = index+v/2;
			QPoint tp = getMovePoint(midangel);
			newcenter += tp;
			newrect.moveCenter(newcenter);
		}
        QFont font;
        font.setFamily("Microsoft YaHei");
        font.setBold(true);
        font.setPointSize(20);
        int spacing=10;
        font.setLetterSpacing(QFont::AbsoluteSpacing, spacing);
        painter->setFont(font);
        painter->drawText(rect,Qt::AlignTop,"piechart");
        painter->begin(this);
		QPoint centerPoint = newrect.center();
		QColor firstColor = m_colorlist.at(colorindex);
		QRadialGradient firstGradient(centerPoint, radius/2);
		firstGradient.setColorAt(0, firstColor.darker(80));
	 	firstGradient.setColorAt(0.90, firstColor.lighter(100));
	 	firstGradient.setColorAt(1.0, firstColor.darker(150));
		painter->setBrush(firstGradient);
		painter->drawPie(newrect, index * 16, v * 16);
		index+=v;
		colorindex++;
		if (colorindex==m_colorlist.count())
		{
			colorindex = 0;
		}
	}
	painter->restore();
}

void CPieWidget::drawRadialGradient( QPainter *painter )
{
	qreal sum = getSumValue();
	int w = width();
	int h = height();
    int radius = qMin(w,h)*0.8; //
	QRect rect(w/2-radius/2,h/2-radius/2,radius,radius);

	painter->save();
	painter->setPen(Qt::NoPen);
	QHashIterator<QString, float> i(m_datamap);

    qreal index = 30;  //start location
	int colorindex=0;
	QStringList keylist = m_datamap.keys();
	for (int i = 0; i < keylist.count(); ++i)
	{
		qreal v = m_datamap.value(keylist.at(i));
		v =v/sum*(360);
		QRect newrect = rect;
		if (m_explodedindex == i || m_isexploded)
		{
			QPoint newcenter = newrect.center();
			int midangel = index+v/2;
			QPoint tp = getMovePoint(midangel);
			newcenter += tp;
			newrect.moveCenter(newcenter);
		}
        QFont font;
        font.setFamily("Microsoft YaHei");
        font.setBold(true);
        font.setPointSize(20);
        int spacing=10;
        font.setLetterSpacing(QFont::AbsoluteSpacing, spacing);
        painter->setFont(font);
        painter->drawText(rect,Qt::AlignTop,"piechart");
        painter->begin(this);
		QPoint centerPoint = newrect.center();
		QColor firstColor = m_colorlist.at(colorindex);
		QRadialGradient firstGradient(centerPoint, radius/2);
		firstGradient.setColorAt(0, firstColor.lighter(80));
		firstGradient.setColorAt(0.61, firstColor.lighter(90));
		firstGradient.setColorAt(0.80, firstColor.lighter(100));
		firstGradient.setColorAt(1.0, firstColor.darker(150));
		painter->setBrush(firstGradient);
		painter->drawPie(newrect, index * 16, v * 16);
		index+=v;
		colorindex++;
		if (colorindex==m_colorlist.count())
		{
			colorindex = 0;
		}
	}
	painter->restore();
}

void CPieWidget::drawDountDefault( QPainter *painter )
{
	qreal sum = getSumValue();
	int w = width();
	int h = height();
    int radius = qMin(w,h)*0.8; //
	QRect rect(w/2-radius/2,h/2-radius/2,radius,radius);

	painter->save();
	painter->setPen(Qt::NoPen);
	QHashIterator<QString, float> i(m_datamap);

    qreal index = 30;  //start location
	int colorindex=0;
	QStringList keylist = m_datamap.keys();
	for (int i = 0; i < keylist.count(); ++i)
	{
		qreal v = m_datamap.value(keylist.at(i));
		v =v/sum*(360);
		QRect newrect = rect;
		if (m_explodedindex == i || m_isexploded)
		{
			QPoint newcenter = newrect.center();
			int midangel = index+v/2;
			QPoint tp = getMovePoint(midangel);
			newcenter += tp;
			newrect.moveCenter(newcenter);
		}
        QFont font;
        font.setFamily("Microsoft YaHei");
        font.setBold(true);
        font.setPointSize(20);
        int spacing=10;
        font.setLetterSpacing(QFont::AbsoluteSpacing, spacing);
        painter->setFont(font);
        painter->drawText(rect,Qt::AlignTop,"piechart");
        painter->begin(this);
		QPoint centerPoint = newrect.center();
		QColor firstColor = m_colorlist.at(colorindex);
		QRadialGradient firstGradient(centerPoint, radius/2);
		firstGradient.setColorAt(0, Qt::transparent);
		firstGradient.setColorAt(0.6, Qt::transparent);
		firstGradient.setColorAt(0.61, firstColor.lighter(100));
		firstGradient.setColorAt(0.80, firstColor.lighter(100));
		firstGradient.setColorAt(1.0, firstColor.lighter(100));
		painter->setBrush(firstGradient);
		painter->drawPie(newrect, index * 16, v * 16);
		index+=v;
		colorindex++;
		if (colorindex==m_colorlist.count())
		{
			colorindex = 0;
		}
	}
	painter->restore();
}

void CPieWidget::drawRingShadingDount( QPainter *painter )
{
	qreal sum = getSumValue();
	int w = width();
	int h = height();
    int radius = qMin(w,h)*0.8; //
	QRect rect(w/2-radius/2,h/2-radius/2,radius,radius);

	painter->save();
	painter->setPen(Qt::NoPen);
	QHashIterator<QString, float> i(m_datamap);

    qreal index = 30;  //start location
	int colorindex=0;
	QStringList keylist = m_datamap.keys();
	for (int i = 0; i < keylist.count(); ++i)
	{
		qreal v = m_datamap.value(keylist.at(i));
		v =v/sum*(360);
		QRect newrect = rect;
		if (m_explodedindex == i || m_isexploded)
		{
			QPoint newcenter = newrect.center();
			int midangel = index+v/2;
			QPoint tp = getMovePoint(midangel);
			newcenter += tp;
			newrect.moveCenter(newcenter);
		}
        QFont font;
        font.setFamily("Microsoft YaHei");
        font.setBold(true);
        font.setPointSize(20);
        int spacing=10;
        font.setLetterSpacing(QFont::AbsoluteSpacing, spacing);
        painter->setFont(font);
        painter->drawText(rect,Qt::AlignTop,"piechart");
        painter->begin(this);
		QPoint centerPoint = newrect.center();
		QColor firstColor = m_colorlist.at(colorindex);
		QRadialGradient firstGradient(centerPoint, radius/2);
		firstGradient.setColorAt(0, Qt::transparent);
		firstGradient.setColorAt(0.6, Qt::transparent);
		firstGradient.setColorAt(0.61, firstColor.dark(150));
		firstGradient.setColorAt(0.80, firstColor.lighter(100));
		firstGradient.setColorAt(1.0, firstColor.dark(150));
		painter->setBrush(firstGradient);
		painter->drawPie(newrect, index * 16, v * 16);
		index+=v;
		colorindex++;
		if (colorindex==m_colorlist.count())
		{
			colorindex = 0;
		}
	}
	painter->restore();
}

void CPieWidget::drawConcaveShadingDount( QPainter *painter )
{
	qreal sum = getSumValue();
	int w = width();
	int h = height();
    int radius = qMin(w,h)*0.8; //
	QRect rect(w/2-radius/2,h/2-radius/2,radius,radius);

	painter->save();
	painter->setPen(Qt::NoPen);
	QHashIterator<QString, float> i(m_datamap);

    qreal index = 30;  //start location
	int colorindex=0;
	QStringList keylist = m_datamap.keys();
	for (int i = 0; i < keylist.count(); ++i)
	{
		qreal v = m_datamap.value(keylist.at(i));
		v =v/sum*(360);
		QRect newrect = rect;
		if (m_explodedindex == i || m_isexploded)
		{
			QPoint newcenter = newrect.center();
			int midangel = index+v/2;
			QPoint tp = getMovePoint(midangel);
			newcenter += tp;
			newrect.moveCenter(newcenter);
		}
        QFont font;
        font.setFamily("Microsoft YaHei");
        font.setBold(true);
        font.setPointSize(20);
        int spacing=0;
        font.setLetterSpacing(QFont::AbsoluteSpacing, spacing);
        painter->setFont(font);
        painter->drawText(rect,Qt::AlignTop,"piechart");
        painter->begin(this);
		QPoint centerPoint = newrect.center();
		QColor firstColor = m_colorlist.at(colorindex);
		QRadialGradient firstGradient(centerPoint, radius/2);
		firstGradient.setColorAt(0, Qt::transparent);
		firstGradient.setColorAt(0.6, Qt::transparent);
		firstGradient.setColorAt(0.61, firstColor.dark(100));
		firstGradient.setColorAt(0.90, firstColor.lighter(100));
		firstGradient.setColorAt(1.0, firstColor.dark(180));
		painter->setBrush(firstGradient);
		painter->drawPie(newrect, index * 16, v * 16);
		index+=v;
		colorindex++;
		if (colorindex==m_colorlist.count())
		{
			colorindex = 0;
		}
	}
	painter->restore();
}

void CPieWidget::drawExplodedDount( QPainter *painter )
{
	qreal sum = getSumValue();
	int w = width();
	int h = height();
    int radius = qMin(w,h)*0.8; //
	QRect rect(w/2-radius/2,h/2-radius/2,radius,radius);

	painter->save();
	painter->setPen(Qt::NoPen);
	QHashIterator<QString, float> i(m_datamap);
	
    qreal index = 30;  //start location
	int colorindex = 0;

	QStringList keylist = m_datamap.keys();
	for (int i = 0; i < keylist.count(); ++i)
	{
		qreal v = m_datamap.value(keylist.at(i));
		v =v/sum*(360);
		QRect newrect = rect;
		if (m_explodedindex == i || m_isexploded)
		{
			QPoint newcenter = newrect.center();
			int midangel = index+v/2;
			QPoint tp = getMovePoint(midangel);
			newcenter += tp;
			newrect.moveCenter(newcenter);
		}
        QFont font;
        font.setFamily("Microsoft YaHe");
        font.setBold(true);
        font.setPointSize(20);
        int spacing=10;
        font.setLetterSpacing(QFont::AbsoluteSpacing, spacing);
        painter->setFont(font);
        painter->drawText(rect,Qt::AlignTop,"piechart");
        painter->begin(this);
		QPoint centerPoint = newrect.center();
		QColor firstColor = m_colorlist.at(colorindex);
		QRadialGradient firstGradient(centerPoint, radius/2);
		firstGradient.setColorAt(0, Qt::transparent);
		firstGradient.setColorAt(0.6, Qt::transparent);
		firstGradient.setColorAt(0.61, firstColor.dark(150));
		firstGradient.setColorAt(0.80, firstColor.lighter(100));
		firstGradient.setColorAt(1.0, firstColor.dark(150));
		painter->setBrush(firstGradient);
		painter->drawPie(newrect, index * 16, v * 16);
		index+=v;		
		colorindex++;
		if (colorindex==m_colorlist.count())
		{
			colorindex = 0;
		}
	}

	painter->restore();
}

qreal CPieWidget::getSumValue()
{
	qreal sum = 0;
	QHashIterator<QString, float> i(m_datamap);
	while (i.hasNext()) {
		i.next();
		sum+= i.value();
	}
	if (sum == 0.0)
		sum = 1;
	return sum;
}

QPoint CPieWidget::getMovePoint( qreal angel )
{
	int wl = 0;
	int hl = 0;
	qreal ang = (angel)*3.14/180;
	wl = 20*qCos(ang);
	hl = 20*qSin(ang);

	if (angel > 90 && angel < 270)
		wl = qAbs(wl)*-1;
	else
		wl = qAbs(wl);

	if (angel < 180)
		hl = qAbs(hl)*-1;
	else
		hl = qAbs(hl);

	return QPoint(wl,hl);
}







