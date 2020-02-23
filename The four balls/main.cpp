#include "freeglut/include/GL/glut.h"
#include <bevgrafmath2017.h>
#include <time.h> 
#include <cmath>
#include <iostream>

//Globalis adatok
GLfloat radius = 18.0;
GLint width = 600, height = 600;

vec2 balls[4] = {{400, 100},
				{500, 150},
				{50, 500},
				{400, 200}};
vec2 direction[4] = {{-0.8,-0.8},
				{-0.8,-0.8},
				{0.8,0.8},
				{0.8,0.8}};

vec3 elsoEgyenes;	//Ahol A,B,C parametereket tarolom
vec3 masodikEgyenes;
void init() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, width, 0.0, height);
	glShadeModel(GL_FLAT);
	glPointSize(10.0);
	glLineWidth(4.0);
	glEnable(GL_POINT_SMOOTH);
	glColor3f(0.0, 0.0, 1.0);
}
void drawBalls() {
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	for (GLdouble t = 0; t <= 2 * pi(); t += 0.01)
		glVertex2d(balls[0].x + radius * cos(t), balls[0].y + radius * sin(t));
	glEnd();
	glBegin(GL_LINE_LOOP);
	for (GLdouble t = 0; t <= 2 * pi(); t += 0.01)
		glVertex2d(balls[1].x + radius * cos(t), balls[1].y + radius * sin(t));
	glEnd();
	glBegin(GL_LINE_LOOP);
	for (GLdouble t = 0; t <= 2 * pi(); t += 0.01)
		glVertex2d(balls[2].x + radius * cos(t), balls[2].y + radius * sin(t));
	glEnd();
	glBegin(GL_LINE_LOOP);
	for (GLdouble t = 0; t <= 2 * pi(); t += 0.01)
		glVertex2d(balls[3].x + radius * cos(t), balls[3].y + radius * sin(t));
	glEnd();
}
void drawBackgroundPoints() {
	vec2 currentPoint;
	vec2 eredmeny;
	int step = 20;
	for (int i = 0; i < width; i += step) {
		for (int j = 0; j < height; j += step) {
			currentPoint = { (float)i,(float)j };
			eredmeny.x = elsoEgyenes.x * currentPoint.x + elsoEgyenes.y * currentPoint.y + elsoEgyenes.z;
			eredmeny.y = masodikEgyenes.x * currentPoint.x + masodikEgyenes.y * currentPoint.y + masodikEgyenes.z;
			if (eredmeny.x > 0 && eredmeny.y > 0)
				glColor3f(1.0, 0.0, 0.0);
			if (eredmeny.x < 0 && eredmeny.y < 0)
				glColor3f(0.0, 1.0, 0.0);
			if (eredmeny.x > 0 && eredmeny.y < 0)
				glColor3f(0.0, 0.0, 1.0);
			if (eredmeny.x < 0 && eredmeny.y > 0)
				glColor3f(1.0, 1.0, 0.0);

			glBegin(GL_POINTS);
				glVertex2f(currentPoint.x, currentPoint.y);
			glEnd();
		}
	}
}

void drawLine() {
	glBegin(GL_LINES);
		glVertex2d(balls[0].x, balls[0].y);
		glVertex2d(balls[2].x, balls[2].y);
		glVertex2d(balls[1].x, balls[1].y);
		glVertex2d(balls[3].x, balls[3].y);
	glEnd();
}
void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	drawBalls();
	drawLine();
	drawBackgroundPoints();
	glutSwapBuffers();
}
void testRules() {
	for (int i = 0; i < 4; i++) {
		if (balls[i].x + radius > width || balls[i].x - radius < 0) {
			direction[i].x *= -1;
		}
		if (balls[i].y + radius > height || balls[i].y - radius < 0) {
			direction[i].y *= -1;
		}
	}
}
void calculateLine() {
	vec2 iranyvektor[2] = { {balls[2] - balls[0]},
							{balls[3] - balls[1]} };
	vec2 normalvektor[2] = { {-iranyvektor[0].y, iranyvektor[0].x},
							 {-iranyvektor[1].y, iranyvektor[1].x} };
	/*
	 *	A = nx
	 *	B = ny
	 *	C = −A · x0 − B · y0,
	*/
	elsoEgyenes.x = normalvektor[0].x;
	elsoEgyenes.y = normalvektor[0].y;
	elsoEgyenes.z = -elsoEgyenes.x*balls[0].x - elsoEgyenes.y * balls[0].y;

	masodikEgyenes.x = normalvektor[1].x;
	masodikEgyenes.y = normalvektor[1].y;
	masodikEgyenes.z = -masodikEgyenes.x*balls[1].x - masodikEgyenes.y * balls[1].y;
}
void update(int n) {
	for (int i = 0; i < 4; i++)
		balls[i] += direction[i];
	testRules();
	calculateLine();
	glutTimerFunc(5, update, 0);
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(200, 0);
	glutCreateWindow("The four balls");
	glutDisplayFunc(display);
	init();
	glutTimerFunc(5, update, 0);
	glutMainLoop();
	return 0;
}