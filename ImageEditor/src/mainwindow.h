#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "widgets/imageeditor.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(const char *filename, QWidget *parent = 0);
	~MainWindow();
private:
	ImageEditor* m_pEditor;
	QString m_fileName;
};

#endif // MAINWINDOW_H
