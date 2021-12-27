#pragma once

#include <QWidget>
#include "ui_slaverwindow.h"
class SlaverWindow : public QWidget
{
	Q_OBJECT

public:
	SlaverWindow(QWidget* parent = Q_NULLPTR);
	~SlaverWindow();
	void createBrowserWindow();
private:
	Ui::SlaverWindow ui;
};
