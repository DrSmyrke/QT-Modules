#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QSpinBox>
#include <QUdpSocket>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
private slots:
	void slot_readMessage();
	void slot_sendMessage();
private:
	QPushButton* m_pSendB;
	QTextEdit* m_pLogView;
	QLineEdit* m_pIP;
	QLineEdit* m_pMess;
	QSpinBox* m_pPort;
	QUdpSocket* m_pSocketIn;
	QUdpSocket* m_pSocketOut;
};

#endif // MAINWINDOW_H
