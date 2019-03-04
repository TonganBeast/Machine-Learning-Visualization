/*

#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtOpenGL.h"

class QtOpenGL : public QMainWindow
{
	Q_OBJECT

public:
	QtOpenGL(QWidget *parent = Q_NULLPTR);

private:
	Ui::QtOpenGLClass ui;
};

*/

#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtOpenGL.h"

class QtOpenGL : public QMainWindow 
{
	Q_OBJECT

public:
	QtOpenGL(QWidget *parent = nullptr);

public slots:
	void onPushButton();

private:
	Ui::QtOpenGLClass ui;
};
