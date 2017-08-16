#include "imageeditor.h"

#include <QPainter>

ImageEditor::ImageEditor(QWidget *parent) : QWidget(parent)
{
	setNewSize(QSize(100,100));
	//
	m_resizeBox.setWidth(20);
	m_resizeBox.setHeight(20);
	this->setMouseTracking(true);
}
void ImageEditor::setImage(const QString &file)
{
	m_img.load(file);
	m_img=m_img.scaledToHeight(this->height());
	if(m_img.width()>this->width()){
		m_img=m_img.scaledToWidth(this->width());
	}
	chkResise();
}
QImage ImageEditor::getImage()
{
	QImage copy;
	int rx=(this->width()/2)-(m_img.width()/2);
	int ry=(this->height()/2)-(m_img.height()/2);
	copy=m_img.copy(m_ramkaPos.x()-rx,m_ramkaPos.y()-ry,m_ramkaSize.width(),m_ramkaSize.height());
	return copy;
}

void ImageEditor::setNewSize(const QSize &size)
{
	if(!m_img.isNull()){
		unsigned int rw=0;
		unsigned int rh=0;
		if(size.width()>m_img.width()) rw = size.width()-m_img.width();
		if(size.height()>m_img.height()) rh = size.height()-m_img.height();
		m_ramkaSize.setWidth(size.width()-rw-rh);
		m_ramkaSize.setHeight(size.height()-rw-rh);
	}else{
		m_ramkaSize.setWidth(10);
		m_ramkaSize.setHeight(10);
	}
	m_r=(float)m_ramkaSize.width()/(float)m_ramkaSize.height();
	m_ramkaPos.setX((this->width()/2)-(m_ramkaSize.width()/2));
	m_ramkaPos.setY((this->height()/2)-(m_ramkaSize.height()/2));
}
void ImageEditor::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	QPainter p(this);

	int rx=(this->width()/2)-(m_img.width()/2);
	int ry=(this->height()/2)-(m_img.height()/2);
	p.drawImage(rx,ry,m_img);

	//чтоб рамка за границы изображения не уползла
	if(m_ramkaPos.x()<rx) m_ramkaPos.setX(rx);
	if(m_ramkaPos.y()<ry) m_ramkaPos.setY(ry);
	if(m_ramkaPos.y()+m_ramkaSize.height()>m_img.height()+ry) m_ramkaPos.setY(m_img.height()-m_ramkaSize.height()+ry);
	if(m_ramkaPos.x()+m_ramkaSize.width()>m_img.width()+rx) m_ramkaPos.setX(m_img.width()-m_ramkaSize.width()+rx);

	//draw shadow
	p.setPen(Qt::NoPen);
	p.setBrush(QBrush(QColor(100,100,100,200)));
	p.drawRect(rx,ry,m_img.width(),m_ramkaPos.y()-ry);
	p.drawRect(rx,m_ramkaPos.y()+m_ramkaSize.height(),m_img.width(),m_img.height()-(m_ramkaPos.y()+m_ramkaSize.height())+ry);
	p.drawRect(rx,m_ramkaPos.y(),m_ramkaPos.x()-rx,m_ramkaSize.height());
	p.drawRect(m_ramkaPos.x()+m_ramkaSize.width(),m_ramkaPos.y(),m_img.width()-(m_ramkaPos.x()+m_ramkaSize.width())+rx,m_ramkaSize.height());

	//draw resize boxes
	int r=255;
	int g=255;
	int b=255;
	int a=100;
	int rBoxW2=m_resizeBox.width()/2;
	int rBoxH2=m_resizeBox.height()/2;
	p.setPen(Qt::NoPen);

	//отрисовка ресайз боксов
	p.setBrush(QBrush(QColor(r,g,b,a)));
	if(m_mx>m_ramkaPos.x()-rBoxW2 and m_mx<m_ramkaPos.x()+rBoxW2 and m_my>m_ramkaPos.y()-rBoxH2 and m_my<m_ramkaPos.y()+rBoxH2){
		p.setBrush(QBrush(QColor(255,147,0,50)));
	}
	p.drawRect(m_ramkaPos.x()-rBoxW2,m_ramkaPos.y()-rBoxH2,m_resizeBox.width(),m_resizeBox.height());

	p.setBrush(QBrush(QColor(r,g,b,a)));
	if(m_mx>m_ramkaPos.x()-rBoxW2 and m_mx<m_ramkaPos.x()+rBoxW2 and m_my>m_ramkaPos.y()+m_ramkaSize.height()-rBoxH2 and m_my<m_ramkaPos.y()+m_ramkaSize.height()+rBoxH2){
		p.setBrush(QBrush(QColor(255,147,0,50)));
	}
	p.drawRect(m_ramkaPos.x()-rBoxW2,m_ramkaPos.y()+m_ramkaSize.height()-rBoxH2,m_resizeBox.width(),m_resizeBox.height());

	p.setBrush(QBrush(QColor(r,g,b,a)));
	if(m_mx>m_ramkaPos.x()+m_ramkaSize.width()-rBoxW2 and m_mx<m_ramkaPos.x()+m_ramkaSize.width()+rBoxW2 and m_my>m_ramkaPos.y()+m_ramkaSize.height()-rBoxH2 and m_my<m_ramkaPos.y()+m_ramkaSize.height()+rBoxH2){
		p.setBrush(QBrush(QColor(255,147,0,50)));
	}
	p.drawRect(m_ramkaPos.x()+m_ramkaSize.width()-rBoxW2,m_ramkaPos.y()+m_ramkaSize.height()-rBoxH2,m_resizeBox.width(),m_resizeBox.height());

	p.setBrush(QBrush(QColor(r,g,b,a)));
	if(m_mx>m_ramkaPos.x()+m_ramkaSize.width()-rBoxW2 and m_mx<m_ramkaPos.x()+m_ramkaSize.width()+rBoxW2 and m_my>m_ramkaPos.y()-rBoxH2 and m_my<m_ramkaPos.y()+rBoxH2){
		p.setBrush(QBrush(QColor(255,147,0,50)));
	}
	p.drawRect(m_ramkaPos.x()+m_ramkaSize.width()-rBoxW2,m_ramkaPos.y()-rBoxH2,m_resizeBox.width(),m_resizeBox.height());

	//draw ramka
	p.setBrush(Qt::NoBrush);
	p.setPen(Qt::white);
	p.drawRect(m_ramkaPos.x(),m_ramkaPos.y(),m_ramkaSize.width(),m_ramkaSize.height());
}
void ImageEditor::mousePressEvent(QMouseEvent *event)
{
	int mx=event->pos().x();
	int my=event->pos().y();
	int rx=m_ramkaPos.x()+m_ramkaSize.width();
	int ry=m_ramkaPos.y()+m_ramkaSize.height();
	if(mx>m_ramkaPos.x() and mx<rx and my>m_ramkaPos.y() and my<ry){
		m_ramkaMove=true;
		m_pressPos=event->pos();
	}
	int rBoxW2=m_resizeBox.width()/2;
	int rBoxH2=m_resizeBox.height()/2;
	if(m_mx>m_ramkaPos.x()-rBoxW2 and m_mx<m_ramkaPos.x()+rBoxW2 and m_my>m_ramkaPos.y()-rBoxH2 and m_my<m_ramkaPos.y()+rBoxH2){
		m_resizeLeftTop=true;
		m_ramkaMove=false;
		m_pressPos=event->pos();
	}
	if(m_mx>m_ramkaPos.x()-rBoxW2 and m_mx<m_ramkaPos.x()+rBoxW2 and m_my>m_ramkaPos.y()+m_ramkaSize.height()-rBoxH2 and m_my<m_ramkaPos.y()+m_ramkaSize.height()+rBoxH2){
		m_resizeLeftBottom=true;
		m_ramkaMove=false;
		m_pressPos=event->pos();
	}
	if(m_mx>m_ramkaPos.x()+m_ramkaSize.width()-rBoxW2 and m_mx<m_ramkaPos.x()+m_ramkaSize.width()+rBoxW2 and m_my>m_ramkaPos.y()+m_ramkaSize.height()-rBoxH2 and m_my<m_ramkaPos.y()+m_ramkaSize.height()+rBoxH2){
		m_resizeRightBottom=true;
		m_ramkaMove=false;
		m_pressPos=event->pos();
	}
	if(m_mx>m_ramkaPos.x()+m_ramkaSize.width()-rBoxW2 and m_mx<m_ramkaPos.x()+m_ramkaSize.width()+rBoxW2 and m_my>m_ramkaPos.y()-rBoxH2 and m_my<m_ramkaPos.y()+rBoxH2){
		m_resizeRightTop=true;
		m_ramkaMove=false;
		m_pressPos=event->pos();
	}
	QWidget::mousePressEvent(event);
}
void ImageEditor::mouseReleaseEvent(QMouseEvent *event)
{
	Q_UNUSED(event);
	m_ramkaMove=false;
	m_resizeLeftTop=false;
	m_resizeLeftBottom=false;
	m_resizeRightBottom=false;
	m_resizeRightTop=false;
}
void ImageEditor::mouseMoveEvent(QMouseEvent *event)
{
	m_mx=event->pos().x();
	m_my=event->pos().y();
	QPoint r(m_pressPos-event->pos());
	if(m_ramkaMove){
		m_ramkaPos-=r;
		m_pressPos=event->pos();

	}
	if(m_resizeLeftTop){
		int tmpW=m_ramkaSize.width();
		int tmpH=m_ramkaSize.height();
		m_ramkaSize.setWidth(m_ramkaSize.width()+r.y());
		chkResise();
		tmpW-=m_ramkaSize.width();
		tmpH-=m_ramkaSize.height();
		m_ramkaPos.setX(m_ramkaPos.x()+tmpW);
		m_ramkaPos.setY(m_ramkaPos.y()+tmpH);
		m_pressPos=event->pos();
	}
	if(m_resizeLeftBottom){
		int tmpW=m_ramkaSize.width();
		m_ramkaSize.setWidth(m_ramkaSize.width()+r.x());
		chkResise();
		tmpW-=m_ramkaSize.width();
		m_ramkaPos.setX(m_ramkaPos.x()+tmpW);
		m_pressPos=event->pos();
	}
	if(m_resizeRightTop){
		int tmp=m_ramkaSize.height();
		m_ramkaSize.setWidth(m_ramkaSize.width()-r.x());
		chkResise();
		tmp-=m_ramkaSize.height();
		m_ramkaPos.setY(m_ramkaPos.y()+tmp);
		m_pressPos=event->pos();
	}
	if(m_resizeRightBottom){
		m_ramkaSize.setWidth(m_ramkaSize.width()-r.x());
		chkResise();
		m_pressPos=event->pos();
	}
	update();
	QWidget::mouseMoveEvent(event);
}
void ImageEditor::chkResise()
{
	if(m_ramkaSize.width()<25) m_ramkaSize.setWidth(25);
	if(m_ramkaSize.width()>m_img.width()) m_ramkaSize.setWidth(m_img.width());
	m_ramkaSize.setHeight(m_ramkaSize.width()/m_r);

	if(m_ramkaSize.height()>m_img.height()){
		m_ramkaSize.setHeight(m_img.height());
		m_ramkaSize.setWidth(m_ramkaSize.height()*m_r);
	}
}
