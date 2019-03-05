
#ifndef QVIEWPORT_H
#define QVIEWPORT_H

#include "MD_Graph.h"
#include <QtOpenGL>

/*
 * This OpenGL Viewport class extends the provided QGLWidget class to
 * provide customized OpenGL rendering. The primary functions required
 * by Qt are the initializeGL, paintGL, and resizeGL functions. These
 * are functions you can redefine to provide your own customized drawing.
 * Note that you do not have to call these funcitons, the Qt event system
 * will internally handle when these functions need to be called.
 *
 * To use this class there are 2 main options: (1) manually create a new
 * instance of this object (dynamically allocated pointer) and add it to
 * an existing widget or layout, or (2) use the Qt Designer to promote a
 * QWidget to this QViewport class. This is an old link but the principle
 * remains the same:
 * http://doc.qt.io/qt-4.8/designer-using-custom-widgets.html
 */
class QViewport : public QGLWidget {
public:
	QViewport(QWidget* parent = nullptr);
	virtual ~QViewport();

	/* Called once during program initialization. Provides setup commands. */
	void initializeGL();

	/* Called continuously (per event or timer). This is the main rendering loop. */
	void paintGL();

	/* Called whenever the widget is resized. */
	void resizeGL(int width, int height);

protected:
	float clearColorR, clearColorG, clearColorB;
	float pointSize;
	float lineWidth;
	const int SCREEN_WIDTH = 1000;
	const int SCREEN_HEIGHT = 400;
	MD_Graph graph;
};

#endif
