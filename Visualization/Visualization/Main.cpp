#include "MD_Graph.h"


MD_Graph graph;
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 400;

void Display();
void Init(void);
void dataFit(int);


int main(int argc, char** argv) {
	MD_Graph g("breast-cancer-wisconsin-data.txt");
	graph = g;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Visualization");
	Init();
	glutDisplayFunc(Display);
	glutMainLoop();
	//~
	return 0;
}

void dataFit(int numPlanes) {
	std::cout << numPlanes << "\n";
	int x = (numPlanes * 300) + 100;
	//glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	//gluOrtho2D(0.0, x, 0.0, SCREEN_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Display() {
	glClear(GL_COLOR_BUFFER_BIT);
	//glViewport(0.0, 0.0, SCREEN_WIDTH, SCREEN_HEIGHT);
	//gluOrtho2D(0.0, (graph.getGraph().getNumPlanes() * 300) + 100, 0.0, 400);
	//dataFit(graph.getGraph().getNumPlanes());
	graph.getGraph().buildGraph(true, true, true, false);
}

void Init(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, SCREEN_WIDTH, 0.0, SCREEN_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPointSize(4.0);
	glLineWidth(2.0);
}
