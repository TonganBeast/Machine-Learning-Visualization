#include "QViewport.h"
#include <gl/GLU.h>

QViewport::QViewport(QWidget* parent) : QGLWidget(parent) {
	//OpenGL parameters
	this->pointSize = 4.0f;
	this->lineWidth = 2.0f;

	this->clearColorR = 1.0f;
	this->clearColorG = 1.0f;
	this->clearColorB = 1.0f;
	//create the graph which will be displayed
	MD_Graph g("iris-data.txt");
	graph = g;

}

QViewport::~QViewport() {

}

//OpenGL setup
void QViewport::initializeGL() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	gluOrtho2D(0.0, SCREEN_WIDTH, 0.0, SCREEN_HEIGHT);
	glClearColor(this->clearColorR, this->clearColorG, this->clearColorB, 0.0f);
	glPointSize(this->pointSize);
	glLineWidth(this->lineWidth);
}

//render function outputs visualization to the OpenGL widget
void QViewport::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT);
	graph.buildGraph();
}

void QViewport::resizeGL(int width, int height) {
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, SCREEN_WIDTH, 0.0, SCREEN_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT);
}