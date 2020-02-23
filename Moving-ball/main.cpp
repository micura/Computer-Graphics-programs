#include "freeglut/include/GL/glut.h"
#include <bevgrafmath2017.h>
#include <time.h> 
#include <cmath>

//Globalis adatok
GLfloat radius = 20.0;
GLint width = 800, height = 750;
vec2 movablePoints[4] = {
	{ 150,150 },		//Egyik sarokpont
	{ 650,650 },		//Másik sarokpont
	{ 250, 250},		//Egyik pont az egyenesen
	{ 500, 450}			//Másik pont az egyenesen
};
vec2 circle = { 400, 300 };
vec2 direction = { 0.5, 0.5 };
GLint dragged = -1;

void init() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, width, 0.0, height);
	glShadeModel(GL_FLAT);
	glPointSize(8.0);
	glLineWidth(2.0);
	glEnable(GL_POINT_SMOOTH);
	glColor3f(0.0, 0.0, 1.0);
}
 
//két vektor skalárszorzatának kiszámolása
float scalar(vec2 a, vec2 b) {
	float s;
	s = (a.x * b.x) + (a.y * b.y);
	return s;
}

//Pattanás számolása
void lineTest(vec2 d, float a, float b, float c) {
	float bt;
	direction.x *= -1;
	direction.y *= -1;

	//Normálvektor
	vec2 normalvector;
	normalvector.x = a;
	normalvector.y = b;

	bt = 2 * (scalar(normalvector, direction) / scalar(normalvector, normalvector));

	normalvector.x *= bt;
	normalvector.y *= bt;

	direction.x = normalvector.x - direction.x;
	direction.y = normalvector.y - direction.y;
}
void testRules(float a, float b, float c) {
	//Vizsgáljuk meg hogy mikor éri el az oldalakat
	if (circle.x + radius > movablePoints[1].x || circle.x - radius < movablePoints[0].x) {
		direction.x *= -1;
	}
	if (circle.y + radius > movablePoints[1].y || circle.y - radius < movablePoints[0].y) {
		direction.y *= -1;
	}
	float distance = (a*circle.x + b*circle.y + c) / sqrt(a*a + b*b); // Labda távolsága az egyenestől
	//Vizsgáljuk meg hogy mikor éri el a labda az egyenest
	if (fabs(distance)-radius < 1) {
		lineTest(direction, a, b, c);
	}
}
void drawBall() {
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	for (GLdouble t = 0; t <= 2 * pi(); t += 0.01)
		glVertex2d(circle.x + radius * cos(t), circle.y + radius * sin(t));
	glEnd();
}
void drawPoints(float a, float b, float c) {
	vec2 currentPoint; float d = 0;
	int increaser = 20;
	for (int i = 0; i < width; i += increaser) {
		for (int j = 0; j < height; j += increaser) {
			//Ha páros sor kezdődjön beljebb. 
			int number = j / 20;
			if (number % 2 == 1) {
				currentPoint = { (float)i,(float)j };
			}
			else {
				currentPoint = { (float)i + increaser / 2 ,(float)j };
			}
			glBegin(GL_POINTS);
			d = (a*currentPoint.x + b*currentPoint.y + c) / sqrt(a*a + b*b); // Pontok távolsága az egyenestől
			//Ha a pont a téglalapon kivülre esik
			if ((currentPoint.x < movablePoints[0].x || currentPoint.x > movablePoints[1].x) ||
				(currentPoint.y < movablePoints[0].y || currentPoint.y > movablePoints[1].y)
				) {
				glColor3f(1.0, 1.0, 1.0);
				glVertex2f(currentPoint.x, currentPoint.y);
			}
			else if (d > 0) {
				glColor3f(1.0, 0.0, 0.0);
				glVertex2f(currentPoint.x, currentPoint.y);
			}
			else if (d < 0) {
				glColor3f(0.0, 1.0, 0.0);
				glVertex2f(currentPoint.x, currentPoint.y);
			}

			if (dist(circle, currentPoint) < radius) {				//Ha a pont a körön belül van legyen sárga
				glColor3f(1.0, 1.0, 0.0);
				glVertex2f(currentPoint.x, currentPoint.y);
			}
			glEnd();
		}
	}
}
float linearEquationSolvery(float a, float b, float c, float delta_y) {
	float x = (-b * delta_y - c) / a;
	return x;
}
float linearEquationSolverx(float a, float b, float c, float delta_x) {
	float y = (-a * delta_x - c) / b;
	return y;
}

void line(float x0, float y0, float x1, float y1) {
	//Egyenes egyenlete ekkor:		 (y0−y1)x + (x1−x0)y + (y1−y0)x0 − (x1−x0)y0 =0 <==> ax+by+c = 0
	float a, b, c;
	a = (y0 - y1);
	b = (x1 - x0);
	c = (x0*y1 - x1*y0);
	/* Egyenes kirajzolása
	for (float x = 0; x < 800; x++) {
		for (float y = 0; y < 800; y++) {
			if (a*x + b*y + c == 0) {
			glBegin(GL_POINTS);
			glVertex2f(x, y);
			glEnd();
			}
		}
	}*/
	vec2 metszespontok[2];
	metszespontok[0].x = linearEquationSolvery(a, b, c, movablePoints[0].y);
	metszespontok[0].y = linearEquationSolverx(a, b, c, movablePoints[0].x);
	metszespontok[1].x = linearEquationSolvery(a, b, c, movablePoints[1].y);
	metszespontok[1].y = linearEquationSolverx(a, b, c, movablePoints[1].x);

	/* Az össszes metszéspont kirajzolása
	glBegin(GL_POINTS); 
		glVertex2f(movablePoints[0].x, metszespontok[0].y);
		glVertex2f(movablePoints[1].x, metszespontok[1].y);
		glVertex2f(metszespontok[0].x, movablePoints[0].y);
		glVertex2f(metszespontok[1].x, movablePoints[1].y);
	glEnd(); */
	
	vec2 linePoints[2];
	if (metszespontok[0].y > movablePoints[0].y && metszespontok[0].y < movablePoints[1].y) {
		linePoints[0].x = movablePoints[0].x;
		linePoints[0].y = metszespontok[0].y;
	} else if (metszespontok[0].y > movablePoints[1].y) {
		linePoints[0].x = metszespontok[1].x;
		linePoints[0].y = movablePoints[1].y;
	} else  {
		linePoints[0].x = metszespontok[0].x;
		linePoints[0].y = movablePoints[0].y;
	}
	
	if (metszespontok[1].y > movablePoints[1].y) {
		linePoints[1].x = metszespontok[1].x;
		linePoints[1].y = movablePoints[1].y;
	}  else if (metszespontok[1].y < movablePoints[0].y) {
		linePoints[1].x = metszespontok[0].x;
		linePoints[1].y = movablePoints[0].y;
	} else {
		linePoints[1].x = movablePoints[1].x;
		linePoints[1].y = metszespontok[1].y;
	}

	drawPoints(a, b, c);	//Adjuk át a pointsnak az egyenes paramétereit
	testRules(a, b, c);
	//Rajzoljuk ki az egyenest majd a metszéspontjait
	glBegin(GL_LINES);
		glColor3f(0.0, 0.0, 0.0);
		glVertex2f(linePoints[0].x, linePoints[0].y);
		glVertex2f(linePoints[1].x, linePoints[1].y);
	glEnd();
	glBegin(GL_POINTS);
		glColor3f(1.0, 0.0, 1.0);
		glVertex2f(linePoints[0].x, linePoints[0].y);
		glVertex2f(linePoints[1].x, linePoints[1].y);
	glEnd();

}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	line(movablePoints[2].x, movablePoints[2].y, movablePoints[3].x, movablePoints[3].y);
	drawBall();
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
		glVertex2f(movablePoints[0].x, movablePoints[0].y); //Bal alsó sarok
		glVertex2f(movablePoints[1].x, movablePoints[0].y); //jobb alsó sarok
		glVertex2f(movablePoints[1].x, movablePoints[1].y); //Jobb felső sarok
		glVertex2f(movablePoints[0].x, movablePoints[1].y); //bal felső sarok
	glEnd();

	//Mozgatható pontokhoz rajzoljuk...
	glBegin(GL_POINTS);
		glColor3f(0.0, 0.0, 1.0); //kék
		glVertex2f(movablePoints[0].x, movablePoints[0].y); //Bal alsó sarok
		glVertex2f(movablePoints[1].x, movablePoints[1].y); //Jobb felső sarok
		glVertex2f(movablePoints[2].x, movablePoints[2].y); //Egyik pont az egyenesen
		glVertex2f(movablePoints[3].x, movablePoints[3].y); //Másik pont az egyenesen
	glEnd();

	glutSwapBuffers();
}
void update(int n)
{
	circle += direction;
	glutTimerFunc(5, update, 0);
	glutPostRedisplay();
}
GLint getActivePoint1(vec2 p[], GLint size, GLint sens, GLint x, GLint y)
{
	GLint i, s = sens * sens;
	vec2 P = { (float)x, (float)y };

	for (i = 0; i < size; i++)
		if (dist2(p[i], P) < s)
			return i;
	return -1;
}
GLint getActivePoint2(vec2 *p, GLint size, GLint sens, GLint x, GLint y)
{
	GLint i;
	for (i = 0; i < size; i++)
		if (fabs((*(p + i)).x - x) < sens && fabs((*(p + i)).y - y) < sens)
			return i;
	return -1;
}
void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
	GLint i;
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
		if ((i = getActivePoint1(movablePoints, 4, 8, xMouse, height - yMouse)) != -1)
			dragged = i;
	if (button == GLUT_LEFT_BUTTON && action == GLUT_UP)
		dragged = -1;
}
void processMouseActiveMotion(GLint xMouse, GLint yMouse)
{
	GLint i;
	if (dragged >= 0) {
		movablePoints[dragged].x = xMouse;
		movablePoints[dragged].y = height - yMouse;
		glutPostRedisplay();
	}
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(200, 0);
	glutCreateWindow("Második beadandó!");
	glutMouseFunc(processMouse);
	glutMotionFunc(processMouseActiveMotion);
	glutDisplayFunc(display);
	init();
	glutTimerFunc(5, update, 0);
	glutMainLoop();
	return 0;
}