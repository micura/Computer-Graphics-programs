#include "freeglut/include/GL/glut.h"
#include <bevgrafmath2017.h>
#include <math.h>
#include <vector>

GLsizei winWidth = 800, winHeight = 750;
GLint dragged = -1;
typedef std::vector<double> vec;
#define N 11 // Pontok száma
vec2 Points[N] = {
	{ 175, 550 },   //Points0	/ P1
	{ 100, 500 },   //Points1	/ P2
	{ 200, 400 },   //Points2	/ P3
	{ 250, 200 },	//Points3	/ P4-Q0
	{ 0.0, 0.0 },	//Points4	/ Q1
	{ 260, 230 },	//Points5	/ Q2
	{ 0.0, 0.0 },	//Points6	/ Q3
	{ 400, 400 },	//Points7	/ Q4-S1
	{ 475, 500 },	//Points8	/ S2
	{ 550, 400 },	//Points9	/ S3
	{ 475, 300 }	//Points10	/ S4
};
vec2 movablePoints[4];

void init() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, winWidth, 0.0, winHeight);
	glShadeModel(GL_FLAT);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(10.0);
	glLineWidth(2.0);
	glLineStipple(1, 0xFF00);
}
void ControlPoints() {
	GLint i;
	glColor3f(1.0f, 0.0f, 1.0f);
	glBegin(GL_POINTS);
	for (i = 0; i < N; i++) {
		glVertex2f(Points[i].x, Points[i].y);
	}
	glEnd();
	glFlush();
}
GLfloat fact(int n) {
	if (n > 1)
		return n * fact(n - 1);
	else
		return 1;
}
GLfloat Bersteinpolynom(int n, int i, GLfloat t) {
	GLfloat B = (fact(n) / (fact(i) * fact(n - i)) * pow(t, i) * pow(1 - t, n - i));
	return B;
}
void hermiteFor3point() {
	//===========================Hermite iv===========================//
	float t1 = -1.0;
	float t2 = 0.0;
	float t3 = 1.0;
	glColor3f(0.0f, 0.8f, 0.8f);
	glBegin(GL_LINE_STRIP);
	for (float t = t1; t <= t3; t += 0.01) {
		mat24 G = {
			Points[0].x,    Points[1].x,    Points[2].x,    Points[3].x - Points[2].x,
			Points[0].y,    Points[1].y,    Points[2].y,    Points[3].y - Points[2].y,
		};
		mat4 M = {
			t1*t1*t1,   t2*t2*t2,   t3*t3*t3,   3 * t3*t3,
			t1*t1,      t2*t2,      t3*t3,      2 * t3,
			t1,         t2,         t3,         1.0,
			1.0,        1.0,        1.0,        0.0
		};
		vec T;
		T.push_back(t*t*t);
		T.push_back(t * t);
		T.push_back(t);
		T.push_back(1.0);

		mat24 C = G * inverse(M);
		GLfloat x = 0.0;
		GLfloat y = 0.0;
		for (int i = 0; i < 4; i++) {
			x += C[0][i] * T[i];
			y += C[1][i] * T[i];
		}
		glVertex2f(x, y);
		T.clear();
	}
	glEnd();

	glColor3f(1.0f, 0.0f, 0.8f);
	//Érintő vektor hermite
	glBegin(GL_LINES);
		glVertex2f(Points[2].x, Points[2].y);
		glVertex2f(Points[3].x, Points[3].y);
	glEnd();

	//Q1 pont számolasa
	Points[4] = (4 * Points[2] + Points[3] - Points[2]) / 4;
}
void bezier() {
	//===========================Bezier görbe===========================//
	glColor3f(0.0, 0.0, 1.0);
	vec2 Q[5] = {
		{Points[2].x, Points[2].y},
		{Points[4].x, Points[4].y},
		{Points[5].x, Points[5].y},
		{Points[6].x, Points[6].y},
		{Points[7].x, Points[7].y}
	};

	GLfloat x = 0.0;
	GLfloat y = 0.0;
	glColor3f(0.0, 0.0, 1.0);

	glBegin(GL_LINE_STRIP);
	for (GLfloat t = 0; t <= 1; t += 0.01f) {
		GLfloat x = 0.0, y = 0.0;
		for (int i = 0; i < 5; i++) {
			x += Q[i].x * Bersteinpolynom(4, i, t);
			y += Q[i].y * Bersteinpolynom(4, i, t);
		}
		glVertex2f(x, y);
	}
	glEnd();

	//Kösszük össze a pontokat
	glBegin(GL_LINE_STRIP);
		for (int i = 0; i < 5; i++){
			glVertex2f(Q[i].x, Q[i].y);
		}
	glEnd();
}
void hermiteFor4point() {
	float t1 = -1.0;
	float t2 = 0.0;
	float t3 = 1.0;
	float t4 = 2.0;
	vec2 e;
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	for (float t = t1; t <= t4; t += 0.01) {
		mat24 G = {
			Points[7].x,    Points[8].x,    Points[9].x,    Points[10].x,
			Points[7].y,    Points[8].y,    Points[9].y,    Points[10].y,
		};
		mat4 M = {
			t1*t1*t1,   t2*t2*t2,   t3*t3*t3,   t4*t4*t4,
			t1*t1,      t2*t2,      t3*t3,      t4*t4,
			t1,         t2,         t3,         t4,
			1.0,        1.0,        1.0,        1.0
		};
		vec T;
		T.push_back(t*t*t);
		T.push_back(t * t);
		T.push_back(t);
		T.push_back(1.0);

		//Érintő
		vec T_;
		T_.push_back(3 * t1*t1);
		T_.push_back(2 * t1);
		T_.push_back(1.0);
		T_.push_back(0.0);

		mat24 C = G * inverse(M);
		e.x = C[0][0] * T_[0] + C[0][1] * T_[1] + C[0][2] * T_[2] + C[0][3] * T_[3];
		e.y = C[1][0] * T_[0] + C[1][1] * T_[1] + C[1][2] * T_[2] + C[1][3] * T_[3];
		

		GLfloat x = 0.0;
		GLfloat y = 0.0;
		for (int i = 0; i < 4; i++) {
			x += C[0][i] * T[i];
			y += C[1][i] * T[i];
		}
		glVertex2f(x, y);

		T.clear();
		T_.clear();
		vec2 e2 = 2 * (e / 4);
		Points[6] = Points[7] - e2;
	}
	glEnd();


}
void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	hermiteFor3point();
	bezier();
	hermiteFor4point();
	ControlPoints();
	glutSwapBuffers();
}
GLint getActivePoint1(vec2 p[], GLint size, GLint sens, GLint x, GLint y) {
	GLint i, s = sens * sens;
	vec2 P = { (float)x, (float)y };
	for (i = 0; i < size; i++)
		if (dist(p[i], P) < s)
			return i;
	return -1;
}
void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse) {
	GLint i;
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
		if ((i = getActivePoint1(Points, N, 5, xMouse, winHeight - yMouse)) != -1)
			dragged = i;
	if (button == GLUT_LEFT_BUTTON && action == GLUT_UP)
		dragged = -1;
}
void processMouseActiveMotion(GLint xMouse, GLint yMouse) {
	GLint i;
	if (dragged >= 0) {
		Points[dragged].x = xMouse;
		Points[dragged].y = winHeight - yMouse;
		glutPostRedisplay();
	}
}
void update(int n) {
	glutPostRedisplay();
	glutTimerFunc(10, update, 0);
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(300, 0);
	glutCreateWindow("Curves v2");
	init();
	glutDisplayFunc(display);
	glutMouseFunc(processMouse);
	glutMotionFunc(processMouseActiveMotion);
	glutTimerFunc(10, update, 0);
	glutMainLoop();
	return 0;
}
