#include "freeglut/include/GL/glut.h"
#include <bevgrafmath2017.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <time.h> 

using namespace std;
GLsizei winWidth = 800, winHeight = 750;
vec2 currentSquare[4];
vec2 mainSquareCenter = { 0.0, 0.0 };
GLfloat r = 20;
GLint step = 0;

/*Transzformációhoz */
mat3 M;
vec2 scaleValue = { 1.0f, 1.0f };
GLfloat scaleChange = 0.0005;
GLfloat t = pi()/5;
std::vector<vec2> listOfPointsAfterTransformation = std::vector<vec2>();

void initSquare(GLint xMouse, GLint yMouse) {
	mainSquareCenter.x = xMouse;
	mainSquareCenter.y = yMouse;
	r = 15;
	int a = 0;
	for (GLdouble t = pi() / 4; t <= 2 * pi(); t += pi() / 2) {
		currentSquare[a].x = r * cos(t) + mainSquareCenter.x;
		currentSquare[a].y = r * sin(t) + mainSquareCenter.y;
		listOfPointsAfterTransformation.push_back(currentSquare[a]);
		step++;
		a++;
	}
}

void init() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(-(winWidth / 2), (winWidth / 2), -(winHeight / 2), (winHeight / 2));
	glShadeModel(GL_FLAT);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(5.0);
	glLineWidth(1.0);
	M =  mat3();
}
void TransformSquarePoint() {
	mat3 transToO = translate(-1 * mainSquareCenter);
	mat3 rotated = rotate(t);
	mat3 scaled = scale(scaleValue);
	mat3 transToC = translate(mainSquareCenter);
	M = transToC * scaled * rotated * transToO;

	for (int i = 0; i < 4; i++) {
		vec3 pointH = ihToH(currentSquare[i]);
		vec3 rotatedPointH = M * pointH;
		if (rotatedPointH.z != 0) {
			vec2 result = hToIh(rotatedPointH);
			listOfPointsAfterTransformation.push_back(result);
		}
		currentSquare[i] = listOfPointsAfterTransformation[step];
		step++;
	}
}
void drawSquare() {
	srand(time(0));
	float red = (rand() % 256) / (float)255;
	float green = (rand() % 256) / (float)255;
	float blue = (rand() % 256) / (float)255;

		for (int i = step - 1; i >= 0; i=i-4) {
			glColor3f(red, green, blue);
			glBegin(GL_QUADS);
			for (int j = 0; j<4; j++) {
				glVertex2f(listOfPointsAfterTransformation[i - j].x, listOfPointsAfterTransformation[i - j].y);
			}
			glEnd();
			glBegin(GL_LINE_LOOP);
			for (int j = 0; j<4; j++) {
				glColor3f(0.0, 0.0, 0.0);
				glVertex2f(listOfPointsAfterTransformation[i - j].x, listOfPointsAfterTransformation[i - j].y);
			}
			glEnd();
		}
		scaleValue += scaleChange;
}
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(step != 0)
		drawSquare();
	glutSwapBuffers();
}

void update(int v) {
	if (r <= 150 && step != 0) {
		TransformSquarePoint();
	}
	if (r >= 150) {
		scaleValue = { 1.0f, 1.0f };
	}
	//Vizsgáljuk mennyi az r
	r = dist(currentSquare[0], currentSquare[2]) / 2;
	glutPostRedisplay();
	glutTimerFunc(10, update, 0);
}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
		vec2 mouse;
		vec2 newMouse;
		mouse.x = xMouse;
		mouse.y = fabs(yMouse - winHeight);

		//++ részek
		if ((mouse.x > winWidth / 2) && (mouse.y > winHeight / 2) ) {
			//std::cout << "Jobb felso sarok" << std::endl;
			newMouse.x = 0 + mouse.x - (winWidth / 2);
			newMouse.y = 0 + mouse.y - (winHeight / 2);
		}
		//+- részek
		if ((mouse.x > winWidth / 2) && (mouse.y < winHeight / 2)) {
			//std::cout << "Jobb also sarok" << std::endl;
			newMouse.x = 0 + mouse.x - (winWidth / 2);
			newMouse.y = 0 + mouse.y - (winHeight / 2);
		}
		//-+ részek
		if ((mouse.x < winWidth / 2) && (mouse.y > winHeight / 2)) {
			//std::cout << "Bal felso sarok" << std::endl;
			newMouse.x = 0 + mouse.x - (winWidth / 2);
			newMouse.y = 0 + mouse.y - (winHeight / 2);
		}
		//-- részek
		if ((mouse.x < winWidth / 2) && (mouse.y < winHeight / 2)) {
			//std::cout << "Bal also sarok" << std::endl;
			newMouse.x = 0 + mouse.x - (winWidth / 2);
			newMouse.y = 0 + mouse.y - (winHeight / 2);
		}
		initSquare(newMouse.x, newMouse.y);
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 0);
	glutCreateWindow("Cube flower");
	init();
	glutDisplayFunc(display);
	glutTimerFunc(10, update, 0);
	glutMouseFunc(processMouse);
	glutMainLoop();
	return 0;
}