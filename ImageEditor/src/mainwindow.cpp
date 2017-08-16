#include "mainwindow.h"
#include <QLayout>

#include <QDebug>
#include <QTimer>

MainWindow::MainWindow(const char *filename, QWidget *parent)
	: QMainWindow(parent)
	, m_fileName(filename)
{
	m_pEditor=new ImageEditor(this);

	setWindowTitle("Image editor v0.1");
	setMinimumSize(640,480);

	QWidget* centrWidget=new QWidget(this);
		QVBoxLayout* vBox=new QVBoxLayout();
			vBox->addWidget(m_pEditor);
		centrWidget->setLayout(vBox);
	setCentralWidget(centrWidget);


	QTimer::singleShot(100,this,[this](){
		m_pEditor->setImage(m_fileName);
		m_pEditor->setNewSize(QSize(600,440));
	});

}

MainWindow::~MainWindow()
{

}
