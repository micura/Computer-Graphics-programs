#include "freeglut/include/GL/glut.h"
#include "bevgrafmath2017.h"
#include <math.h>

GLsizei winWidth = 900, winHeight = 750;
float t1 = -2.0;
float t2 = 0.5;
float t3 = 1.5;
float z = 0.5;

#define N 11 // Pontok száma

vec2 Points[N] = {
	{ 100, 600 },   //P0
	{ 400, 650 },   //P1
	{ 600, 550 },   //P2
	{ 150, 725 },   //P3 - e0
					//P4 -- a program számolja
	{ 0, 0 },
	{ 750, 400 },   //P5
	{ 500, 380 },   //P6
					//P7 -- a program számolja
	{ 0, 0 },
	{ 150, 250 },   //P8
	{ 400, 100 },   //P9
	{ 750, 150 },   //P10
};
GLint dragged = -1;

class De_casteljau
{
public:
	GLfloat x, y;
	float vegpont_x = Points[6].x + 0.75*(Points[6].x - Points[5].x);
	float vegpont_y = Points[6].y + 0.75*(Points[6].y - Points[5].y);
	vec2 D[5] =
	{
		{ Points[6].x, Points[6].y },
		{ vegpont_x, vegpont_y },
		{ Points[8].x, Points[8].y },
		{ Points[9].x, Points[9].y },
		{ Points[10].x, Points[10].y },
	};
	vec2 iv(float t)
	{
		vec2 V[5];
		for (int i = 0; i < 5; i++) {
			V[i] = D[i];
		}
		vec2 tomb[1];
		int n = 4;

		for (int r = 0; r < 4; r++)
		{
			for (int i = 0; i < n; i++)
			{
				V[i].x = tomb[0].x = (1 - t) * V[i].x + t * V[i + 1].x;
				V[i].y = tomb[0].y = (1 - t) * V[i].y + t * V[i + 1].y;
			}
			n--;
		}
		return tomb[0];
	}
	void vaz()
	{
		vec2 C[5];
		for (int i = 0; i < 5; i++) {
			C[i] = D[i];
		}
		int n = 4;
		for (int r = 0; r < 4; r++)
		{
			glBegin(GL_LINE_STRIP);
			for (int i = 0; i < n; i++)
			{
				C[i].x = x = (1 - z) * C[i].x + z * C[i + 1].x;
				C[i].y = y = (1 - z) * C[i].y + z * C[i + 1].y;
				glVertex2f(x, y);
			}
			glEnd();
			n--;
		}
	}
	void pontok()
	{
		int n = 4;
		for (int r = 0; r < 4; r++)
		{
			glBegin(GL_POINTS);
			for (int i = 0; i < n; i++)
			{
				if (n == 1)
				{
					glColor3f(0.5f, 0.0f, 0.3f);
				}
				D[i].x = x = (1 - z) * D[i].x + z * D[i + 1].x;
				D[i].y = y = (1 - z) * D[i].y + z * D[i + 1].y;
				glVertex2i(x, y);
			}
			glEnd();
			n--;
		}
	}
};
void ControlPoints()
{
	GLint i;

	glColor3f(1.0f, 0.0f, 1.0f);
	glBegin(GL_POINTS);
	for (i = 0; i < N; i++)
	{
		glVertex2f(Points[i].x, Points[i].y);
	}
	glEnd();
	glFlush();
}
void gorbek()
{
	//===========================Hermite iv===========================//
	glLineWidth(2.0);
	glColor3f(0.0f, 0.0f, 0.0f);
	GLfloat H[4];
	GLfloat e2x;
	GLfloat e2y;

	glBegin(GL_LINE_STRIP);
	for (float t = t1; t <= t3; t += 0.01) {
		//Ez a G
		mat24 G = {
			Points[0].x,    Points[1].x,    Points[2].x,    Points[3].x - Points[0].x,
			Points[0].y,    Points[1].y,    Points[2].y,    Points[3].y - Points[0].y,
		};
		//Ez az M
		mat4 M = {
			t1*t1*t1,   t2*t2*t2,   t3*t3*t3,   3 * t1*t1,
			t1*t1,      t2*t2,      t3*t3,      2 * t1,
			t1,         t2,         t3,         1.0,
			1.0,        1.0,        1.0,        0.0
		};
		//Ez a T
		GLfloat u1 = t;
		GLfloat u2 = u1*u1;
		GLfloat u3 = u2*u1;

		//C = G * M //2x4
		mat24 C = G*inverse(M);

		GLfloat x = 0.0;
		GLfloat y = 0.0;
		e2x = C[0][0] * (3 * t * t) + C[0][1] * (2 * t) + C[0][2] * 1.0 + C[0][3] * 0.0;
		e2y = C[1][0] * (3 * t * t) + C[1][1] * (2 * t) + C[1][2] * 1.0 + C[1][3] * 0.0;

		x += C[0][0] * u3;
		y += C[1][0] * u3;

		x += C[0][1] * u2;
		y += C[1][1] * u2;

		x += C[0][2] * u1;
		y += C[1][2] * u1;

		x += C[0][3] * 1.0;
		y += C[1][3] * 1.0;

		glVertex2f(x, y);
	}
	glEnd();

	//Hermite iv-hez tartozó érintők
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_POINTS);
	glVertex2i(Points[2].x + e2x, Points[2].y + e2y);
	glEnd();

	glColor3f(1.0f, 0.0f, 0.0f);
	glLineWidth(2.0);
	glBegin(GL_LINES);
	glVertex2i(Points[0].x, Points[0].y);
	glVertex2i(Points[3].x, Points[3].y);

	glVertex2i(Points[2].x, Points[2].y);
	glVertex2i(Points[2].x + e2x, Points[2].y + e2y);
	glEnd();

	//===========================Bezier görbe===========================//
	glLineWidth(2.0);
	glColor3f(0.0, 0.0, 1.0);
	GLfloat B[4];
	mat24 GB;
	glBegin(GL_LINE_STRIP);
	for (float t = 0; t <= 1.0; t += 0.01) {
		GLfloat u1 = t;
		GLfloat u2 = u1*u1;
		GLfloat u3 = u2*u1;
		//Ez a GB
		GB = {
			Points[2].x,    Points[2].x + (e2x / 3),  Points[5].x,  Points[6].x,
			Points[2].y,    Points[2].y + (e2y / 3),  Points[5].y,  Points[6].y,
		};
		//Ez az MB
		mat4 MB = {
			-1.0,   3.0,    -3.0,   1.0,
			3.0,    -6.0,   3.0,    0.0,
			-3.0,   3.0,    0.0,    0.0,
			1.0,    0.0,    0.0,    0.0
		};
		//MB*T
		B[0] = MB[0][0] * u3 + MB[0][1] * u2 + MB[0][2] * u1 + MB[0][3] * 1.0;
		B[1] = MB[1][0] * u3 + MB[1][1] * u2 + MB[1][2] * u1 + MB[1][3] * 1.0;
		B[2] = MB[2][0] * u3 + MB[2][1] * u2 + MB[2][2] * u1 + MB[2][3] * 1.0;
		B[3] = MB[3][0] * u3 + MB[3][1] * u2 + MB[3][2] * u1 + MB[3][3] * 1.0;

		GLfloat x = 0.0;
		GLfloat y = 0.0;

		for (int i = 0; i<4; i++) {
			x += B[i] * GB[0][i];
			y += B[i] * GB[1][i];
		}
		glVertex2f(x, y);
	}

	glEnd();
	glFlush();
	//Bezier iv-hez tartozó érintők, és pontok
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_POINTS);
	glVertex2f(GB[0][1], GB[1][1]);
	glVertex2f(Points[2].x + e2x, Points[2].y + e2y);

	//Végpontbeli
	glVertex2f(Points[6].x + 0.75*(Points[6].x - Points[5].x),
		Points[6].y + 0.75*(Points[6].y - Points[5].y)),
		glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);
	glLineWidth(2.0);
	glBegin(GL_LINES);
	glVertex2f(GB[0][1], GB[1][1]);
	glVertex2i(Points[5].x, Points[5].y);

	glVertex2f(Points[5].x, Points[5].y);
	glVertex2f(Points[6].x, Points[6].y);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(Points[6].x, Points[6].y);
	glVertex2f(Points[6].x + 0.75*(Points[6].x - Points[5].x),
		Points[6].y + 0.75*(Points[6].y - Points[5].y)),
		glEnd();

	//===========================Öt kontrollpontos Bezier===========================//
	De_casteljau casteljau;         //Példányosítunk
	GLfloat x, y = 0;

	//Kössük össze a pontokat(konvex burok)
	glColor3f(0.2f, 0.2f, 0.2f);
	glBegin(GL_LINES);
	for (int j = 0; j < 4; j++) {
		glVertex2f(casteljau.D[j].x, casteljau.D[j].y);
		glVertex2f(casteljau.D[j + 1].x, casteljau.D[j + 1].y);
	}
	glEnd();

	//Rajzoljuk meg az ívet
	glColor3f(0.6f, 0.0f, 0.5f);
	glBegin(GL_LINE_STRIP);
	for (float t = 0; t <= 1.0; t += 0.01) {
		x = casteljau.iv(t).x;
		y = casteljau.iv(t).y;
		glVertex2f(x, y);
	}
	glEnd();

	//Rajzoljuk meg a vázat
	glColor3f(0.6f, 0.0f, 0.5f);
	casteljau.vaz();

	glColor3f(0.9f, 0.0f, 0.1f);
	casteljau.pontok();
}
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(0);
		break;
	case '+':
		if (z < 0.99) {
			z += 0.01;
		}
		break;
	case '-':
		if (z > 0.01) {
			z -= 0.01;
		}
		break;
		glutPostRedisplay();
	}
}
void init() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, winWidth, 0.0, winHeight);
	glShadeModel(GL_FLAT);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(8.0);
	glLineWidth(5.0);
	glLineStipple(1, 0xFF00);
}
void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	gorbek();
	ControlPoints();
	glutSwapBuffers();
}

GLint getActivePoint1(vec2 p[], GLint size, GLint sens, GLint x, GLint y) {
	GLint i, s = sens * sens;
	vec2 P = { (float)x, (float)y };

	for (i = 0; i < size; i++)
		if (dist2(p[i], P) < s)
			return i;
	return -1;
}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse) {
	GLint i;
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
		if ((i = getActivePoint1(Points, 11, 20, xMouse, winHeight - yMouse)) != -1)
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
	glutCreateWindow("Curves");
	init();
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutMouseFunc(processMouse);
	glutMotionFunc(processMouseActiveMotion);
	glutTimerFunc(10, update, 0);
	glutMainLoop();
	return 0;
}