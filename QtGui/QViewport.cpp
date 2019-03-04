#include "QViewport.h"
#include <gl/GLU.h>

QViewport::QViewport(QWidget* parent) : QGLWidget(parent) {
	/* Setup some arbitrary constants */
	this->pointSize = 2.0f;
	this->lineWidth = 1.0f;

	this->clearColorR = 0.0f;
	this->clearColorG = 0.0f;
	this->clearColorB = 0.0f;
}

QViewport::~QViewport() {

}

/* This function is only called once during program initialization. This
 * is where you can setup any required OpenGL settings. 
 */
void QViewport::initializeGL() {
	glClearColor(this->clearColorR, this->clearColorG, this->clearColorB, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glPointSize(this->pointSize);
	glLineWidth(this->lineWidth);
}

/* Render function called for each paint event update (frame). This function
 * is internally called using the QtApplication event queue, you do NOT have
 * to call this function. If you want the viewport to update on a fixed interval
 * rather than updates, see QTimer; Once you have the timer connected to this 
 * class with an onTimeout() slot, you can call the event-based update function:
 * this->updgateGL(). Do NOT call updateGL() from this function (this will result
 * in infinite event recursion).
 */
void QViewport::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	/* Setup the look at view matrix */
	gluLookAt(0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	/* Render a simple triangle */
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.5f, -0.25f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.5f, 0.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.5f, -0.25f, 0.0f);
	glEnd();

	glPopMatrix();
	glFlush();
}

void QViewport::resizeGL(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/* Resize behavior. This depends on the type of projection used.
	 * If you want an orthographic projection, then glOrtho or glOrtho2D will
	 * work, otherwise you can just leave this as a perspective projection.
	 */
	gluPerspective(45.0f, width / static_cast<float>(height), 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}
