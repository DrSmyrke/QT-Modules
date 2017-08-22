#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setWindowTitle("UDP client v0.1");

	m_pSocketIn=new QUdpSocket(this);
	m_pSocketOut=new QUdpSocket(this);

	m_pSendB=new QPushButton("Send");
		connect(m_pSendB,&QPushButton::clicked,this,&MainWindow::slot_sendMessage);
	m_pLogView=new QTextEdit();
		m_pLogView->setReadOnly(true);
	m_pIP=new QLineEdit();
		m_pIP->setPlaceholderText("IP");
		m_pIP->setText("127.0.0.1");
	m_pMess=new QLineEdit();
		m_pMess->setPlaceholderText("Input data");
		connect(m_pMess,&QLineEdit::returnPressed,this,&MainWindow::slot_sendMessage);
	m_pPort=new QSpinBox();
		m_pPort->setMinimum(1);
		m_pPort->setMaximum(65535);
		m_pPort->setValue(3700);
		//connect(m_pPort,static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),this,[this](int val){Q_UNUSED(val);slot_connect();});

	QWidget* centrWidget=new QWidget();
		QVBoxLayout* vBox=new QVBoxLayout();
			QHBoxLayout* topPanel=new QHBoxLayout();
			topPanel->addWidget(m_pIP);
			topPanel->addWidget(m_pPort);
		vBox->addLayout(topPanel);
		vBox->addWidget(m_pLogView);
			QHBoxLayout* sendPanel=new QHBoxLayout();
			sendPanel->addWidget(m_pMess);
			sendPanel->addWidget(m_pSendB);
		vBox->addLayout(sendPanel);
	centrWidget->setLayout(vBox);
	setCentralWidget(centrWidget);


	m_pSocketIn->bind(QHostAddress::Any,50000);
	connect(m_pSocketIn,&QUdpSocket::readyRead,this,&MainWindow::slot_readMessage);
}
MainWindow::~MainWindow()
{

}
void MainWindow::slot_readMessage()
{
	while (m_pSocketIn->hasPendingDatagrams()) {
		QByteArray buffer;
		buffer.resize(m_pSocketIn->pendingDatagramSize());
		QHostAddress sender;
		quint16 senderPort;
		m_pSocketIn->readDatagram(buffer.data(), buffer.size(),&sender, &senderPort);
		m_pLogView->append(">:"+QString(buffer.data())+"["+buffer.toHex()+"]");
	}
}
void MainWindow::slot_sendMessage()
{
	QString word=m_pMess->text();
	m_pMess->clear();
	m_pLogView->append("<:"+word);
	QByteArray buffer;
	if(word=="0xAA"){
		buffer[0] = 0xAA;
		buffer[1] = 0x55;

		buffer[2] = 0x00;
		buffer[3] = 0x00;
		buffer[4] = 0x00;
		buffer[5] = 0x00;
		buffer[6] = 0x82;
		buffer[7] = 0x00;
		buffer[8] = 0x00;
		qDebug() << "Data() return:" << buffer.toHex();
	}else{
		buffer=word.toUtf8();
	}
	QHostAddress sender;
		sender.setAddress(m_pIP->text());
	m_pSocketOut->writeDatagram(buffer.data(),buffer.length(),sender,m_pPort->value());
	m_pSocketOut->waitForBytesWritten(3000);

aa 55 00 00 01 00 port 00 00 value
}
