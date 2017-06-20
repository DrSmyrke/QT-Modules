#include "mychart.h"

//TODO: remove debug
#include <QDebug>

MyChart::MyChart(QWidget *parent) : QWidget(parent)
{
	m_pTimer=new QTimer();
		m_pTimer->setInterval(10);
		connect(m_pTimer,&QTimer::timeout,this,&MyChart::slot_timer);


	this->setMouseTracking(true);
}
void MyChart::addChart(const std::vector<double> &yData)
{
	GraphData newData;
		newData.xyData.clear();
		newData.title="График №"+QString::number(graphics.size());
		int i=0;
		for(auto yVal:yData){
			std::pair<double,double> pair;
				pair.first=i;
				pair.second=yVal;
				if(yVal>newData.predel.maxY){newData.predel.maxY=yVal;}
				if(i>newData.predel.maxX){newData.predel.maxX=i;}
				if(yVal<newData.predel.minY){newData.predel.minY=yVal;}

				changePredel(i,yVal);
			newData.xyData.push_back(pair);
			newData.predel.minX=0;
			i++;
		}
		graphics.push_back(newData);
		update();
}


void MyChart::addChart(const std::vector<double> &xData, const std::vector<double> &yData)
{
	GraphData newData;
		newData.xyData.clear();
		newData.title="График №"+QString::number(graphics.size());
		int i=0;
		for(auto yVal:yData){
			std::pair<double,double> pair;
				double x=xData.at(i);
				pair.first=x;
				pair.second=yVal;
				if(yVal>newData.predel.maxY){newData.predel.maxY=yVal;}
				if(x>newData.predel.maxX){newData.predel.maxX=x;}
				if(yVal<newData.predel.minY){newData.predel.minY=yVal;}
				if(x<newData.predel.minX){newData.predel.minX=x;}

				changePredel(x,yVal);
			newData.xyData.push_back(pair);
			i++;
		}
		graphics.push_back(newData);
}

void MyChart::setGraphColor(const unsigned int graphNum, const QColor &color)
{
	if(graphNum>=graphics.size()) return;
	graphics.at(graphNum).color=color;
}
void MyChart::setGraphLineStyle(const unsigned int graphNum, const Qt::PenStyle lineStyle)
{
	if(graphNum>=graphics.size()) return;
	graphics.at(graphNum).lineStyle=lineStyle;
}
void MyChart::setGraphLineWidth(const unsigned int graphNum, const int width)
{
	if(graphNum>=graphics.size()) return;
	graphics.at(graphNum).lineWidth=width;
}
void MyChart::setGraphDotView(const unsigned int graphNum, bool view)
{
	if(graphNum>=graphics.size()) return;
	graphics.at(graphNum).showDots=view;
}

void MyChart::setGraphValsView(const unsigned int graphNum, bool view)
{
	if(graphNum>=graphics.size()) return;
	graphics.at(graphNum).showVals=view;
}
void MyChart::setGraphShowInLegendView(const unsigned int graphNum, bool view)
{
	if(graphNum>=graphics.size()) return;
	graphics.at(graphNum).showInLegend=view;
}
void MyChart::setChartName(const unsigned int graphNum, const QString &name)
{
	if(graphNum>=graphics.size()) return;
	graphics.at(graphNum).title=name;
}
void MyChart::addGraphData(const unsigned int graphNum, const std::vector<double> &xData, const std::vector<double> &yData)
{
	if(xData.size()!=yData.size()) return;
	if(graphNum>=graphics.size()) return;

	int i=0;
	for(auto xVal:xData){
		std::pair<double,double> pair;
			double yVal=yData.at(i);
			pair.first=xVal;
			pair.second=yVal;
			if(yVal>graphics.at(graphNum).predel.maxY){graphics.at(graphNum).predel.maxY=yVal;}
			if(xVal>graphics.at(graphNum).predel.maxX){graphics.at(graphNum).predel.maxX=xVal;}
			if(yVal<graphics.at(graphNum).predel.minY){graphics.at(graphNum).predel.minY=yVal;}
			if(xVal<graphics.at(graphNum).predel.minX){graphics.at(graphNum).predel.minX=xVal;}

			changePredel(xVal,yVal);
		graphics.at(graphNum).xyData.push_back(pair);
		i++;
	}

	update();
}
void MyChart::setMouseTarget(bool enable)
{
	m_mouseTracking=enable;
	update();
}
void MyChart::setLegendView(bool enable)
{
	m_showLegend=enable;
	update();
}
void MyChart::clear()
{
	graphics.clear();
	m_mouseTracking=false;
	m_showLegend=true;
	update();
}
void MyChart::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	QPainter p(this);
	QPen pen;
	QPen penSetka;
		penSetka.setColor(QColor(100,100,100,100));
		penSetka.setStyle(Qt::DotLine);
	p.setPen(Qt::NoPen);
	//p.setBrush(QBrush(QColor(100,100,100,200)));
	p.setBrush(Qt::white);
	p.drawRect(0,0,this->width(),this->height());

	if(m_firstInit){
		m_offset=QPoint(this->width()/2,this->height()/2);
		m_firstInit=false;
	}
	int osyWidth=graphics.size()*40;

	//drawOSI
	p.setPen(Qt::gray);
	p.setBrush(Qt::NoBrush);
	if(m_offset.x()>=osyWidth and m_offset.x()<this->width()) p.drawLine(m_offset.x(),0,m_offset.x(),this->height());
	if(m_offset.y()>=osyWidth and m_offset.y()<this->height()) p.drawLine(0,m_offset.y(),this->width(),m_offset.y());

	//draw Graphs
	for(auto graph:graphics){
		bool first=true;
		double x,y;
		for(auto iter=graph.xyData.begin();iter!=graph.xyData.end();iter++){
			if(first){
				x=(*iter).first;
				y=(*iter).second;
				if(m_offset.x()-osyWidth+x*m_ChagX*graph.chagX<0) continue;
			}
			if(m_offset.x()+x*m_ChagX*graph.chagX>this->width()) continue;
			if(first){
				x=(*iter).first;
				y=(*iter).second;
				first=false;
				continue;
			}
			//show dots
			if(graph.showDots){
				p.setPen(Qt::NoPen);
				p.setBrush(graph.color);
				p.drawEllipse(m_offset.x()+x*m_ChagX*graph.chagX-graph.dotSize,m_offset.y()-y*m_ChagY*graph.chagY-graph.dotSize,graph.dotSize*2,graph.dotSize*2);
			}
			if(graph.showVals){
				p.setPen(graph.color);
				p.drawText(m_offset.x()-30+x*m_ChagX*graph.chagX,m_offset.y()-10-y*m_ChagY*graph.chagY,60,10,Qt::AlignCenter,QString::number(y));
			}


			QPoint prew(m_offset.x()+x*m_ChagX*graph.chagX,m_offset.y()-y*m_ChagY*graph.chagY);

			double newx=(*iter).first;
			double newy=(*iter).second;

			QPoint next(m_offset.x()+newx*m_ChagX*graph.chagX,m_offset.y()-newy*m_ChagY*graph.chagY);

			pen.setColor(graph.color);
			pen.setWidth(graph.lineWidth);
			pen.setStyle(graph.lineStyle);
			p.setPen(pen);

			p.drawLine(prew,next);

			x=newx;
			y=newy;
		}
	}
	//draw OSI
	p.setPen(Qt::NoPen);
	p.setBrush(Qt::white);
	pen.setWidth(1);
	//p.drawRect(0,0,osyWidth,this->height());
	p.drawRect(0,this->height()-40,this->width(),40);

	int graphNum=0;
	for(auto graph:graphics){
		pen.setColor(graph.color);
		pen.setStyle(Qt::SolidLine);
		p.setPen(pen);

		int px=(osyWidth/graphics.size())*graphNum;
		//int py=this->height()-osyWidth+(osyWidth/graphics.size())*graphNum;
		p.drawLine(px,20,px,this->height()-20);

		if(graphNum==0){
			p.drawLine(20,this->height()-30,this->width()-20,this->height()-30);

			int w=this->width();
			int max=(w-m_offset.x())/(m_ChagX*graph.chagX);
			int min=(osyWidth-m_offset.x())/(m_ChagX*graph.chagX);
			int r=100-m_ChagY*graph.chagY;
			int chag=abs((r*min-r*max)/w);
			if(chag<1) chag=1;
			for(double i=min;i<=max;i+=chag){
				if(m_offset.x()+i*m_ChagX*graph.chagX<osyWidth) continue;
				if(m_offset.x()+i*m_ChagX*graph.chagX>this->width()-40) continue;

				p.setPen(pen);
				p.drawText(m_offset.x()-10+i*m_ChagX*graph.chagX,this->height()-20,60,10,Qt::AlignLeft | Qt::AlignVCenter,QString::number(i));
				p.drawLine(m_offset.x()+i*m_ChagX*graph.chagX,this->height()-30-5,m_offset.x()+i*m_ChagX*graph.chagX,this->height()-30+5);

				p.setPen(penSetka);
				p.drawLine(m_offset.x()+i*m_ChagX*graph.chagX,0,m_offset.x()+i*m_ChagX*graph.chagX,this->height()-30+5);
			}
		}

		int w=this->width();
		int max=(m_offset.y()-20)/(m_ChagY*graph.chagY);
		int min=(m_offset.y()-this->height()-40)/(m_ChagY*graph.chagY);
		int r=100-m_ChagY*graph.chagY;
		int chag=abs((r*min-r*max)/w);
		if(chag<1) chag=1;
		for(double i=min;i<=max;i+=chag){
			if(m_offset.y()-i*m_ChagY*graph.chagY<30) continue;
			if(m_offset.y()-i*m_ChagY*graph.chagY>this->width()-200) continue;

			p.setPen(pen);
			p.drawText(px+10,m_offset.y()-5-i*m_ChagY*graph.chagY,60,10,Qt::AlignLeft | Qt::AlignVCenter,QString::number(i));
			p.drawLine(px-5,m_offset.y()-i*m_ChagY*graph.chagY,px+5,m_offset.y()-i*m_ChagY*graph.chagY);

			p.setPen(penSetka);
			p.drawLine(px+20,m_offset.y()-i*m_ChagY*graph.chagY,this->width()-px,m_offset.y()-i*m_ChagY*graph.chagY);
		}


		graphNum++;
	}


	if(m_mouseTracking){
		pen.setColor(QColor(50,50,50,100));
		pen.setStyle(Qt::SolidLine);
		pen.setWidth(1);
		p.setPen(pen);
		p.setBrush(Qt::NoBrush);
		p.drawLine(0,m_mousePoint.y(),this->width(),m_mousePoint.y());
		p.drawLine(m_mousePoint.x(),0,m_mousePoint.x(),this->height());

		double x,y;
		x=(m_mousePoint.x()-m_offset.x())/m_ChagX;
		y=(m_offset.y()-m_mousePoint.y())/m_ChagY;
		p.drawText(m_mousePoint.x()+3,5,60,10,Qt::AlignLeft | Qt::AlignVCenter,QString::number(x));
		p.drawText(3,m_mousePoint.y()+5,60,10,Qt::AlignLeft | Qt::AlignVCenter,QString::number(y));
	}

	//drawLegend
	if(m_showLegend){
		p.setPen(Qt::black);
		p.setBrush(Qt::white);

		int height=0;
		for(auto graph:graphics){
			if(graph.showInLegend) height+=20;
		}
		int width=150;
		int xOffset=this->width()-160;
		int yOffset=10;
		int padding=10;
		p.drawRect(xOffset,yOffset,width,height);

		int graphNum=0;
		for(auto graph:graphics){
			if(!graph.showInLegend) continue;
			pen.setColor(graph.color);
			pen.setStyle(graph.lineStyle);
			pen.setWidth(graph.lineWidth);
			p.setPen(pen);
			int py=yOffset+padding+(height/graphics.size())*graphNum;
			p.drawLine(xOffset+padding,py,xOffset+padding+30,py);
			p.drawText(xOffset+padding+30+10,py-6,width-(padding*2)-30-10,12,Qt::AlignVCenter | Qt::AlignLeft,graph.title);
			graphNum++;
		}
	}
}
void MyChart::mousePressEvent(QMouseEvent *event)
{
	m_pressPos=event->pos();
	m_offsetOld=m_offset;
	m_press=true;
	QWidget::mousePressEvent(event);
}
void MyChart::mouseReleaseEvent(QMouseEvent *event)
{
	//if(m_offsetOld==m_offset and event->button()==Qt::LeftButton){m_mouseLeftClicked++;}else{m_mouseLeftClicked=0;}
	//if(m_offsetOld==m_offset and event->button()==Qt::RightButton){m_mouseRightClicked++;}else{m_mouseRightClicked=0;}
	if(m_offsetOld==m_offset and event->button()==Qt::MiddleButton){
		if(m_mouseTracking){setMouseTarget(false);}else{setMouseTarget(true);}
	}
	m_press=false;
	QWidget::mouseReleaseEvent(event);
}
void MyChart::mouseMoveEvent(QMouseEvent *event)
{
	bool upd=false;
	m_mousePoint=event->pos();

	if(m_mouseTracking) upd=true;
	if(m_press){
//		double oldRX=QPointF(m_offsetOld+event->pos()-m_pressPos).x();
//		double oldRY=QPointF(m_offsetOld+event->pos()-m_pressPos).y();
//		double rxMax=this->width()-m_MaxValsX*m_ChagX;
//		double rxMin=m_MinValsX*m_ChagX;
//		double ryMax=this->height()-m_MaxValsY*m_ChagY;
//		double ryMin=m_MinValsY*m_ChagY;

//		if(oldRY<abs(ryMin)+5) upd=false;
//		if(oldRY>ryMax and ryMax>0) upd=false;
//		if(oldRX>rxMax and rxMax>0) upd=false;
//		if(oldRX<rxMin+30) upd=false;
		m_offset=m_offsetOld+event->pos()-m_pressPos;
		upd=true;
	}
	if(upd) update();
	QWidget::mouseMoveEvent(event);
}

void MyChart::slot_timer()
{
	if(mChagXoffset==0 and mChagYoffset==0){
		m_pTimer->stop();
		return;
	}
	if(mChagXoffset!=0){
		if(mChagXoffset>0){
			m_ChagX+=mChagXoffset;
			mChagXoffset-=0.001;
			if(mChagXoffset<0.001) mChagXoffset=0;
		}else{
			m_ChagX+=mChagXoffset;
			mChagXoffset+=0.001;
			if(mChagXoffset>-0.001) mChagXoffset=0;
			if(m_ChagX<1){
				m_ChagX=1;
				mChagXoffset=0;
			}
		}
	}
	if(mChagYoffset!=0){
		if(mChagYoffset>0){
			m_ChagY+=mChagYoffset;
			mChagYoffset-=0.001;
			if(mChagYoffset<0.001) mChagYoffset=0;
		}else{
			m_ChagY+=mChagYoffset;
			mChagYoffset+=0.001;
			if(mChagYoffset>-0.001) mChagYoffset=0;
			if(m_ChagY<1){
				m_ChagY=1;
				mChagYoffset=0;
			}
		}
	}
	//qDebug()<<mChagXoffset<<mChagYoffset<<m_ChagX<<m_ChagY;
	update();
}

void MyChart::changePredel(const int x, const int y)
{
	if(y>chartPredel.maxY){chartPredel.maxY=y;}
	if(x>chartPredel.maxX){chartPredel.maxX=x;}
	if(y<chartPredel.minY){chartPredel.minY=y;}
	if(x<chartPredel.minX){chartPredel.minX=x;}
}

void MyChart::wheelEvent(QWheelEvent *event)
{
	mChagXoffset=0;
	mChagYoffset=0;

	int graphNum=-1;
	int width=graphics.size()*40;
	graphNum=m_mousePoint.x()/(width/graphics.size());
	if(graphNum>=static_cast<int>(graphics.size())) graphNum=-1;
	if(graphNum>=0){
		if(event->delta()>0){
			if(graphics.at(graphNum).chagY<7) graphics.at(graphNum).chagY+=0.05;
		}
		if(event->delta()<0){
			if(graphics.at(graphNum).chagY>0.1) graphics.at(graphNum).chagY-=0.05;
		}
		update();
		QWidget::wheelEvent(event);
		return;
	}

	if(event->delta()>0){
		if(m_ChagX<30) mChagXoffset+=0.1;
		if(m_ChagY<30) mChagYoffset+=0.1;
		if(!m_pTimer->isActive()) m_pTimer->start();
	}
	if(event->delta()<0){
		if(m_ChagX>0.2) mChagXoffset-=0.1;
		if(m_ChagY>0.2) mChagYoffset-=0.1;
		if(!m_pTimer->isActive()) m_pTimer->start();
	}
	QWidget::wheelEvent(event);
}
