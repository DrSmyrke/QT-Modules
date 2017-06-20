#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "mychart.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
private slots:
	void slot_timer();
private:
	MyChart* m_pChart;
	QTimer* m_pTimer;
	double x=0;
};

#endif // MAINWINDOW_H
