#include "mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	m_pChart=new MyChart(this);
	setCentralWidget(m_pChart);
	setWindowTitle("347tb378tg4b7tbgw475th45y");
	setMinimumSize(640,480);

	int xmax=16;
	std::vector<double> outYValues;
	for(double i=0;i<xmax;i+=0.4){
		double res=sin(i)*60;
		outYValues.push_back(res);
		//qDebug()<<res;
	}
	std::vector<double> outYValues2;
	for(double i=0;i<xmax;i+=0.32){
		double res=sin(i)*40;
		outYValues2.push_back(res);
		//qDebug()<<res;
	}
	m_pChart->addChart(outYValues);
	m_pChart->addChart(outYValues2);
	std::vector<double> blablabla;
		blablabla.push_back(0);
	m_pChart->addChart(blablabla);
	m_pChart->setGraphColor(2,QColor(120,52,175));
	m_pChart->setGraphLineWidth(2,2);

	m_pChart->setGraphColor(0,QColor(255,140,0));
	m_pChart->setGraphColor(1,QColor(0,52,175));

	m_pChart->setGraphDotView(0,true);
	m_pChart->setGraphLineStyle(1,Qt::DashLine);
	//m_pChart->setGraphShowInLegendView(0,false);

	m_pChart->setMouseTarget(true);

	m_pTimer=new QTimer();
		m_pTimer->setInterval(2);
		connect(m_pTimer,&QTimer::timeout,this,&MainWindow::slot_timer);
		m_pTimer->start();
}
MainWindow::~MainWindow()
{

}
void MainWindow::slot_timer()
{
	std::vector<double> outYVal;
	std::vector<double> outXVal;
		outXVal.push_back(x);
		outYVal.push_back(sin(x)*20);
	m_pChart->addGraphData(2,outXVal,outYVal);
	x+=0.1;
	if(x>=40){
		std::vector<double> outYVal2;
		std::vector<double> outXVal2;
			outXVal2.push_back(x);
			outYVal2.push_back(sin(x)*80);
		m_pChart->addGraphData(0,outXVal2,outYVal2);
	}
}
