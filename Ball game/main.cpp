#include "freeglut/include/GL/glut.h"
#include <bevgrafmath2017.h>
#include <time.h> 

//Globalis adatok
GLfloat radius = 30.0;
GLint width = 800, height = 750, increaser = 2;
GLboolean gameover = false;
vec2 center = { 400, 300 };
vec2 d = { 0.5, 0.5 };      //Ezzel mindkét kordinátát tudom változtatni
vec2 currentPoint;

vec2 randomPointGenerator() {
	vec2 currectPoint;
	srand(time(0));
	// Az új pont ne generálódjon a körön belül. vagy a sarkokba
	do {
		//A generálható pontok tartománya 151-649-ig...
		currectPoint.x = rand() % 499 + 151;		//generate 151-650
		currectPoint.y = rand() % 499 + 151;		//generate 151-650
	} while ((dist(currectPoint, center) < radius) ||
		((currectPoint.x < 150 + radius) && (currectPoint.y < 150 + radius)) ||	//Bal alsó sarok kizárása
		((currectPoint.x > 650 - radius) && (currectPoint.y < 150 + radius)) ||	//Jobb alsó sarok kizárása
		((currectPoint.x > 650 - radius) && (currectPoint.y > 650 - radius)) ||	//Jobb felső sarok kizárása
		((currectPoint.x < 150 + radius) && (currectPoint.y > 650 - radius)) 	//Bal felső sarok kizárása
		);

	//Számoljuk hány pontot generáltunk eddig. 
	printMathObject(gameover);
	if (gameover ==10) {
		exit(0);
	}
	gameover++;
	return currectPoint;
}
void testRules() {
	//Vizsgáljuk meg hogy mikor éri el az oldalakat
	if (center.x + radius > 650 || center.x - radius < 150) {
		d.x *= -1;
	}
	if (center.y + radius > 650 || center.y - radius < 150) {
		d.y *= -1;
	}
	//Amikor a kör széle eléri a pontot...
	if (dist(currentPoint, center) <= radius) {
		currentPoint = randomPointGenerator();  //Hozzunk létre új pontot
		radius += increaser; //Növeljük az r-t

		//Amikor a pont túl közel van a falhoz
		if ((center.x - radius) < 150.0) {		//bal sávba esik
			center.x += increaser;	//Tolja el jobbra annyivaal amennyivel nőni fog
		}
		if ((center.x + radius) > 650.0) {		//Jobb sávba esik
			center.x -= increaser;  //Tolja el balra annyival amennyivel nőni fog
		}
		if ((center.y - radius) < 150.0) {		//Alsó sávba esik
			center.y += increaser;  //Tolja el felfele annyival amennyivel nőni fog
		}
		if ((center.y + radius) > 650.0) {		//Felső sávba esik
			center.y -= increaser;  //Tolja el lefele annyival amennyivel nőni fog
		}
	}
}
void init() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, width, 0.0, height);
	glShadeModel(GL_FLAT);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(5.0);
	glLineWidth(3.0);
	glColor3f(0.0, 0.0, 1.0);
	currentPoint = randomPointGenerator();  //Adjunk meg egy kezdőpontot
}
void drawBall() {
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	for (GLdouble t = 0; t <= 2 * pi(); t += 0.01)
		glVertex2d(center.x + radius * cos(t), center.y + radius * sin(t));
	glEnd();

	glColor3f(0.2, 0.1, 1.0);
	glBegin(GL_POLYGON);
	for (GLdouble t = 0; t <= 2 * pi(); t += 0.01)
		glVertex2d(center.x + radius * cos(t), center.y + radius * sin(t));
	glEnd();
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);
	//Itt rajzoljuk ki a négyzetet, amiben a labda mozog
	glBegin(GL_LINE_LOOP);
		glVertex2f(150.0, 150.0); //Bal alsó sarok
		glVertex2f(650.0, 150.0); //jobb alsó sarok
		glVertex2f(650.0, 650.0); //Jobb felső sarok
		glVertex2f(150.0, 650.0); //bal felső sarok
	glEnd();
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POINTS);
		glVertex2f(currentPoint.x, currentPoint.y);
	glEnd();
	drawBall();
	glutSwapBuffers();
}

void update(int n)
{
	center += d;
	testRules();
	glutKeyboardFunc;
	glutTimerFunc(5, update, 0);
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(0);
		break;
	case 'a':
		d.x *= -1;
		break;
	case 'd':
		d.y *= -1;
		break;
	}
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(200, 0);
	glutCreateWindow("Ball game");
	glutDisplayFunc(display);
	init();
	glutKeyboardFunc(keyboard);
	glutSetKeyRepeat(ILLUMINANT_DEVICE_DEFAULT);
	glutTimerFunc(5, update, 0);
	glutMainLoop();
	return 0;
}