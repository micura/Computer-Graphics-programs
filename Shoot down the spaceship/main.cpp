#include "freeglut/include/GL/glut.h"
#include <bevgrafmath2017.h>
#include <time.h> 
#include <vector>
#include <cmath>
#include <iostream>

//Globalis adatok
vec2 mouse;
GLfloat radius = 20.0;
GLint width = 800, height = 750;
vec2 centerCircle = { 400, 713 };
vec2 centerEllipse = { 400, 700 };
vec2 urhajo_sebbesseg = { 1.0, 0.0 };
vec2 normalvector;
vec2 iranyvektor;
int talalatok_szama = 0;
int i = 55, j = 55;
float Red = 1.0, Green = 1.0; //Az űrhajó szinéhez
bool gameover = false;

struct Point
{
	float x, y;
	int index;
	vec2 irany;				//Legyen minden pontnak külön iránya
	vec2 coord = { x, y };
	bool flyable;

	Point(float xv, float yv) :
		x(xv),
		y(yv)
	{}

	const float& operator[](size_t ind) const
	{
		assert(ind >= 0 && ind <= 1);
		return (&x)[ind];
	}

	void update()  {
		coord += irany;
	}

};
std::vector<Point> points = std::vector<Point>();
std::vector<vec2> torlo = std::vector<vec2>();
void kezdo_allapot() {
	gameover = false;
	glClearColor(1.0, 1.0, 1.0, 1.0);
	urhajo_sebbesseg = { 1.0, 0.0 };
	talalatok_szama = 0, i = 55, j =55, Red = 1.0, Green = 1.0;

	/*---------------------Pontok feltöltése--------------------*/
	float  y = 5, indexer = 7;
	//Jobb oldali lövekédek - jobbról balra kell feltölteni
	points.clear();
	for (float a = 0; a < 7; a++) {
		float x = width - 5;
		for (float b = 0; b < indexer; b++) {
			Point a = { x,y };
			x -= 12;	// Növeljük az x-et.
			points.push_back(a);
		}
		y += 12;		// Növeljük az y-t;
		indexer--;
	}
	
	indexer = 7, y = 5;
	//balról jobbra kell feltölteni
	for (float i = 0; i < 7; i++) {
		float x = 5;
		for (float j = 0; j < indexer; j++) {
			Point a = { x,y };
			x += 12;	// Növeljük az x-et.
			points.push_back(a);
		}
		y += 12;		// Növeljük az y-t;
		indexer--;
	}
}
void init() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, width, 0.0, height);
	glShadeModel(GL_FLAT);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(10.0);
	kezdo_allapot();
}
void update(int n)
{
	for (Point &point : points) {
		float ellipszis_pont_tavolsag = (pow((point.coord.x - centerEllipse.x), 2) / pow(50, 2) + pow((point.coord.y - centerEllipse.y), 2) / pow(15, 2));
		if (point.flyable == true) {
			if (point.coord.x > 800 || point.coord.x < 0 || point.coord.y > 800) {			//Ha a lövedék kimegy az ablakból
				point.coord = { -50, -50 };
				//std::cout << "KIMENT" << std::endl;
			}
			else if ((dist(point.coord, centerCircle) <= radius) ||								//Ha a kört éri el
				(ellipszis_pont_tavolsag <= 1)) {											//Ha az ellipszist éri el
				Red -= 0.1; Green -= 0.1;													//Változtassuk az űrhajó szinét
				point.coord = { -50, -50 };
				talalatok_szama++;
				//std::cout << "Belement az urhajoba;" << std::endl;
			}
			else if (point.coord.x < 800 || point.coord.x > 0 || point.coord.y < 800) {
				point.update();
			}
		}
	}
	centerCircle += urhajo_sebbesseg;
	centerEllipse += urhajo_sebbesseg;
	glutTimerFunc(5, update, 0);
	glutPostRedisplay();
}
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(0);
		break;
	case ' ':
		if (gameover == true) {
			kezdo_allapot();
		}
		break;
	}
}
void testRules(){
	//Vizsgáljuk meg hogy mikor éri el az oldalakat az űrhajó
	if (centerEllipse.x + 50 > width) {
		urhajo_sebbesseg.x *= -1;
		if (urhajo_sebbesseg.x >= -4) {
			urhajo_sebbesseg.x -= 0.1;
		}
	}
	if (centerEllipse.x - 50 < 0) {
		urhajo_sebbesseg.x *= -1;
		if (urhajo_sebbesseg.x <= 4) {
			urhajo_sebbesseg.x += 0.1;
		}
	}

	if (i == -1 && talalatok_szama < 10) {
		gameover = true;
		glClearColor(1.0, 0.0, 0.0, 0.0);
		urhajo_sebbesseg = { 0.0, 0.0 };
		for (Point &point : points) {
			point.irany = { 0,0 };
		}
		points.clear();
		glutKeyboardFunc(keyboard);
	}
	else if (talalatok_szama == 10) {
		gameover = true;
		glClearColor(0.0, 1.0, 0.0, 0.0);
		urhajo_sebbesseg = { 0.0, 0.0 };
		for (Point &point : points) {
			point.irany = { 0,0 };
		}
		points.clear();
		glutKeyboardFunc(keyboard);
	}
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);
	testRules();
	glLineWidth(2.0);
	//Kor
	glBegin(GL_LINE_LOOP);	glColor3f(0.0, 0.0, 0.0);
	for (GLdouble t = 0; t <= pi(); t += 0.01)
		glVertex2d(centerCircle.x + radius * cos(t), centerCircle.y + radius * sin(t));
	glEnd(); 
	glBegin(GL_POLYGON); 	glColor3f(Red, Green, 1.0);
	for (GLdouble t = 0; t <= pi(); t += 0.01)
		glVertex2d(centerCircle.x + radius  * cos(t), centerCircle.y + radius  * sin(t));
	glEnd();
	//Elipszis 
	glBegin(GL_LINE_LOOP); 	glColor3f(0.0, 0.0, 0.0);
	for (GLdouble t = 0; t <= 2 * pi(); t += 0.01)
		glVertex2d(centerEllipse.x + 50 * cos(t), centerEllipse.y + 15 * sin(t));
	glEnd();
	
	glBegin(GL_POLYGON);	glColor3f(Red, Green, 1.0);
	for (GLdouble t = 0; t <= 2 * pi(); t += 0.01)
		
		glVertex2d(centerEllipse.x + 50 * cos(t), centerEllipse.y + 15 * sin(t));
	glEnd(); 
	/*--------------------------Ágyú és alkatrészei-------------------------*/
	glColor3f(0.0, 1.0, 0.0);
	glLineWidth(10.0);
	vec2 A = { 400, 25 };
	vec2 v = mouse - A;
	normalvector = normalize(v);
		iranyvektor = normalvector;	//Az aktuális irányhoz
	normalvector *= 50;
	normalvector += A;

	glBegin(GL_LINES);	glColor3f(0.0, 1.0, 0.0);		//Ágyú
	glVertex2f(400, 25);
		glVertex2f(normalvector.x, normalvector.y);
	glEnd();

	glColor3f(0.0, 0.0, 1.0);
	glRecti(370, 0, 430, 30);	//Épület

	//Itt rajzoljuk ki a lövedékeket
	for (Point &point : points) {
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_POINTS);
			glVertex2f(point.coord.x, point.coord.y);
		glEnd();
	}
	glutSwapBuffers();
}
void myMousePosition(int x, int y)
{
	mouse.x = x; 
	mouse.y = fabs(y-height);
}
void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse)
{

	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN && gameover == false) {
		points[i] = { normalvector.x, normalvector.y };	//Honnan induljon...
		points[i].flyable = true;						
		points[i].irany = iranyvektor;
		i--;
	}
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(200, 0);
	glutCreateWindow("Shoot down the spaceship");
	init();
	glutDisplayFunc(display);
	glutSetKeyRepeat(ILLUMINANT_DEVICE_DEFAULT);
	glutMouseFunc(processMouse);
	glutTimerFunc(5, update, 0);
	glutPassiveMotionFunc(myMousePosition);
	glutMainLoop();
	return 0;
}