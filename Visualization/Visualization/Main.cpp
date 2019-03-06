#include "MD_Graph.h"

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 400;

void Display();
void Init(void);


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Visualization");
	glutDisplayFunc(Display);
	Init();
	glutMainLoop();

	//~
	return 0;
}

void Display() {
	glClear(GL_COLOR_BUFFER_BIT);
	MD_Graph graph("breast-cancer-wisconsin-data.txt");
	graph.printPlanePts(0);
}

void Init(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glPointSize(4.0);
	glLineWidth(2.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	gluOrtho2D(0.0, SCREEN_WIDTH, 0.0, SCREEN_HEIGHT);
}