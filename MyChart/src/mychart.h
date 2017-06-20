#ifndef MYCHART_H
#define MYCHART_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QMouseEvent>

struct DataPredel{
	int maxX=0;
	int maxY=0;
	int minX=0;
	int minY=0;
};
struct GraphData{
	double chagX=1;
	double chagY=1;
	DataPredel predel;
	std::vector<std::pair<double,double>> xyData;
	bool showInLegend=true;
	bool showDots=false;
	bool showVals=false;
	int dotSize=2;
	QColor color=QColor(130,10,60);
	int lineWidth=1;
	Qt::PenStyle lineStyle=Qt::SolidLine;
	QString title;
};

class MyChart : public QWidget
{
	Q_OBJECT
public:
	explicit MyChart(QWidget *parent = 0);
	void addChart(const std::vector<double> &yData);
	void addChart(const std::vector<double> &xData,const std::vector<double> &yData);
	void setGraphColor(const unsigned int graphNum,const QColor &color);
	void setGraphLineStyle(const unsigned int graphNum,const Qt::PenStyle lineStyle);
	void setGraphLineWidth(const unsigned int graphNum,const int width);
	void setGraphDotView(const unsigned int graphNum,bool view);
	void setGraphValsView(const unsigned int graphNum,bool view);
	void setGraphShowInLegendView(const unsigned int graphNum,bool view);
	void setChartName(const unsigned int graphNum,const QString &name);
	void addGraphData(const unsigned int graphNum,const std::vector<double> &xData,const std::vector<double> &yData);
	void setMouseTarget(bool enable);
	void setLegendView(bool enable);
	void clear();
protected:
	void paintEvent(QPaintEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;
private slots:
	void slot_timer();
private:
	QTimer* m_pTimer;
	std::vector<GraphData> graphics;
	QPoint m_offset;
	QPoint m_offsetOld;
	QPoint m_pressPos;
	bool m_press=false;
	bool m_firstInit=true;
	double m_ChagX=1;
	double m_ChagY=1;
	//int m_mouseLeftClicked=0;
	//int m_mouseRightClicked=0;
	double mChagXoffset=0;
	double mChagYoffset=0;
	bool m_mouseTracking=false;
	QPointF m_mousePoint;
	DataPredel chartPredel;
	bool m_showLegend=true;

	void changePredel(const int x,const int y);
};

#endif // MYCHART_H
