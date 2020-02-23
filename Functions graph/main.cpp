#include "freeglut/include/GL/glut.h"
#include <bevgrafmath2017.h>
#include <math.h>
#include <iostream>

using namespace std;

GLsizei winWidth = 800, winHeight = 800;

vec2 viewportSize = { 350, 350 };

vec2 first_viewportPosition = { 25, 425 };
vec2 first_windowSize = { 2, 2 };
vec2 first_windowPosition = { -1, -1 };
mat3 first_wTv;
vec2 firstGraph[201]; 	int i = 0;
vec2 firstGraphafterTrans[201];

vec2 second_viewportPosition = { 425, 425 };
vec2 second_windowSize = { 10, 10 };
vec2 second_windowPosition = { -5, -5 };
mat3 second_wTv;
vec2 secondGraph[1000];
vec2 secondGraphafterTrans[1000];

vec2 third_viewportPosition = { 25, 25 };
vec2 third_windowSize = { 4*pi(), 4 * pi() };
vec2 third_windowPosition = { -2*pi(), -2*pi() };
mat3 third_wTv;
vec2 thirdGraph[360];
vec2 thirdGraphafterTrans[360];

vec2 fourth_viewportPosition = { 425, 25 };
vec2 fourth_windowSize = { 8 * pi(), 8 * pi() };
vec2 fourth_windowPosition = {  -4 *pi(), -4* pi() };
mat3 fourth_wTv;
vec2 fourthGraph[1500];
vec2 fourthGraphafterTrans[1500];


void initFirstGraph() {
	for (float x = -1; x < 1; x+=0.01) {
		firstGraph[i].x = x;
		firstGraph[i].y = x*x*x;
		i++;
	}
}
void initSecondGraph() {
	i = 0;
	for (float x = -5; x < 5; x += 0.01) {
		secondGraph[i].x = 1/x;
		secondGraph[i].y = x;
		i++;
	}
}
void initThirdGraph() {
	i = 0;
	for (float x = -2*pi(); x < 2*pi(); x += pi()/45) {
		thirdGraph[i].x = x;
		thirdGraph[i].y = sin(x);
		i++;
	}
	for (float x = -2 * pi(); x < 2 * pi(); x += pi() / 45) {
		thirdGraph[i].x = x;
		thirdGraph[i].y = cos(x);
		i++;
	}
}
void initFourthGraph() {
	i = 0;
	for (float u = -1.5 * pi(); u < 2.5 * pi(); u += pi() / 180) {
		fourthGraph[i].x = u - 1.6 * cos(24*u);
		fourthGraph[i].y = u - 1.6 * sin(25*u);
		i++;
	}
}
void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, winWidth, 0.0, winHeight);
	glShadeModel(GL_FLAT);
	glEnable(GL_POINT_SMOOTH);
	glLineWidth(2.0);
	glPointSize(15.0);
	first_wTv = windowToViewport2(first_windowPosition, first_windowSize, first_viewportPosition, viewportSize);
	second_wTv = windowToViewport2(second_windowPosition, second_windowSize, second_viewportPosition, viewportSize);
	third_wTv = windowToViewport2(third_windowPosition, third_windowSize, third_viewportPosition, viewportSize);
	fourth_wTv = windowToViewport2(fourth_windowPosition, fourth_windowSize, fourth_viewportPosition, viewportSize);
	initFirstGraph();
	initSecondGraph();
	initThirdGraph();
	initFourthGraph();
}
void drawViewport() {
	glColor3f(1.0, 1.0, 1.0);
	glLineWidth(1.0);
	//Bal felső kirajzolása
	glBegin(GL_LINE_LOOP); //{ 0, 400 };
		glVertex2f(first_viewportPosition.x, first_viewportPosition.y); // left bottom
		glVertex2f(first_viewportPosition.x + viewportSize.x, first_viewportPosition.y); // right bottom
		glVertex2f(first_viewportPosition.x + viewportSize.x, first_viewportPosition.y + viewportSize.y); // right top
		glVertex2f(first_viewportPosition.x, first_viewportPosition.y + viewportSize.y); // left top
	glEnd();
	
	//Jobb felső kirajzolása
	glBegin(GL_LINE_LOOP); //{ 400, 400 };
		glVertex2f(second_viewportPosition.x, second_viewportPosition.y); // left bottom
		glVertex2f(second_viewportPosition.x + viewportSize.x, second_viewportPosition.y); // right bottom
		glVertex2f(second_viewportPosition.x + viewportSize.x, second_viewportPosition.y + viewportSize.y); // right top
		glVertex2f(second_viewportPosition.x, second_viewportPosition.y + viewportSize.y); // left top
	glEnd();

	//Bal alsó kirajzolása
	glBegin(GL_LINE_LOOP); //{ 400, 400 };
		glVertex2f(third_viewportPosition.x, third_viewportPosition.y); // left bottom
		glVertex2f(third_viewportPosition.x + viewportSize.x, third_viewportPosition.y); // right bottom
		glVertex2f(third_viewportPosition.x + viewportSize.x, third_viewportPosition.y + viewportSize.y); // right top
		glVertex2f(third_viewportPosition.x, third_viewportPosition.y + viewportSize.y); // left top
	glEnd();

	//Jobb alsó kirajzolása
	glBegin(GL_LINE_LOOP); //{ 400, 400 };
		glVertex2f(fourth_viewportPosition.x, fourth_viewportPosition.y); // left bottom
		glVertex2f(fourth_viewportPosition.x + viewportSize.x, fourth_viewportPosition.y); // right bottom
		glVertex2f(fourth_viewportPosition.x + viewportSize.x, fourth_viewportPosition.y + viewportSize.y); // right top
		glVertex2f(fourth_viewportPosition.x, fourth_viewportPosition.y + viewportSize.y); // left top
	glEnd();

	//Rácsok berajzolása
	glColor3f(0.5, 0.5, 0.5);
	glBegin(GL_LINES);
		glVertex2f(first_viewportPosition.x + viewportSize.x / 2, first_viewportPosition.y);
		glVertex2f(first_viewportPosition.x + viewportSize.x / 2, first_viewportPosition.y + viewportSize.y);
		glVertex2f(first_viewportPosition.x, first_viewportPosition.y + viewportSize.x / 2);
		glVertex2f(first_viewportPosition.x + viewportSize.x, first_viewportPosition.y + viewportSize.x / 2);

		glVertex2f(second_viewportPosition.x + viewportSize.x / 2, second_viewportPosition.y);
		glVertex2f(second_viewportPosition.x + viewportSize.x / 2, second_viewportPosition.y + viewportSize.y);
		glVertex2f(second_viewportPosition.x, second_viewportPosition.y + viewportSize.x / 2);
		glVertex2f(second_viewportPosition.x + viewportSize.x, second_viewportPosition.y + viewportSize.x / 2);

		glVertex2f(third_viewportPosition.x + viewportSize.x / 2, third_viewportPosition.y);
		glVertex2f(third_viewportPosition.x + viewportSize.x / 2, third_viewportPosition.y + viewportSize.y);
		glVertex2f(third_viewportPosition.x, third_viewportPosition.y + viewportSize.x / 2);
		glVertex2f(third_viewportPosition.x + viewportSize.x, third_viewportPosition.y + viewportSize.x / 2);

		glVertex2f(fourth_viewportPosition.x + viewportSize.x / 2, fourth_viewportPosition.y);
		glVertex2f(fourth_viewportPosition.x + viewportSize.x / 2, fourth_viewportPosition.y + viewportSize.y);
		glVertex2f(fourth_viewportPosition.x, fourth_viewportPosition.y + viewportSize.x / 2);
		glVertex2f(fourth_viewportPosition.x + viewportSize.x, fourth_viewportPosition.y + viewportSize.x / 2);
	glEnd();
}

void transformation() {
	for (int l = 0; l < 201; l++) {
		vec3 pointH = ihToH(firstGraph[l]);
		vec3 rotatedPointH = first_wTv * pointH;
		if (rotatedPointH.z != 0) {
			vec2 result = hToIh(rotatedPointH);
			firstGraphafterTrans[l] = { result.x, result.y };
		}
	}
	for (int k = 0; k < 1000; k++) {
		vec3 pointH = ihToH(secondGraph[k]);
		vec3 rotatedPointH = second_wTv * pointH;
		if (rotatedPointH.z != 0) {
			vec2 result = hToIh(rotatedPointH);
			secondGraphafterTrans[k] = { result.x, result.y };
		}
	}
	for (int j = 0; j < 360; j++) {
		vec3 pointH = ihToH(thirdGraph[j]);
		vec3 rotatedPointH = third_wTv * pointH;
		if (rotatedPointH.z != 0) {
			vec2 result = hToIh(rotatedPointH);
			thirdGraphafterTrans[j] = { result.x, result.y };
		}
	}
	for (int j = 0; j < 721; j++) {
		vec3 pointH = ihToH(fourthGraph[j]);
		vec3 rotatedPointH = fourth_wTv * pointH;
		if (rotatedPointH.z != 0) {
			vec2 result = hToIh(rotatedPointH);
			fourthGraphafterTrans[j] = { result.x, result.y };
		}
	}
}
void drawGraph() {
	/********Első grafikon kirajzolása*******/
	glLineWidth(0.5);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 201; i++) {
		glVertex2f(firstGraphafterTrans[i].x, firstGraphafterTrans[i].y);
	}
	glEnd();

	/********Második grafikon kirajzolása*******/
	glColor3f(1.0, 0.8, 0.0);
	glBegin(GL_LINE_STRIP);
	for (int a = 0; a < 500; a++) {
		if (secondGraphafterTrans[a].x+1 >= second_viewportPosition.x && secondGraphafterTrans[a].x <= second_viewportPosition.y +350)
			glVertex2f(secondGraphafterTrans[a].x, secondGraphafterTrans[a].y);
	}
	glEnd();
	glBegin(GL_LINE_STRIP);
	for (int a = 501; a < 1000; a++) {
		if (secondGraphafterTrans[a].x >= second_viewportPosition.x && secondGraphafterTrans[a].x <= second_viewportPosition.y + 350)
			glVertex2f(secondGraphafterTrans[a].x, secondGraphafterTrans[a].y);
	}
	glEnd();
	
	/********Harmadik grafikon kirajzolása******/
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINE_STRIP);
	for (int c = 0; c < 180; c++) {
			glVertex2f(thirdGraphafterTrans[c].x, thirdGraphafterTrans[c].y);
	}
	glEnd(); 
	glColor3f(1.0, 0.2, 0.5);
	glBegin(GL_LINE_STRIP);
	for (int c = 180; c < 360; c++) {
		glVertex2f(thirdGraphafterTrans[c].x, thirdGraphafterTrans[c].y);
	}
	glEnd();
	/********Negyedik grafikon kirajzolása******/
	glColor3f(0.0, 0.6, 1.0);
	glBegin(GL_LINE_STRIP);
	for (int c = 0; c < 721; c++) {
		glVertex2f(fourthGraphafterTrans[c].x, fourthGraphafterTrans[c].y);
	}
	glEnd();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	drawViewport();
	drawGraph();
	glutSwapBuffers();
}
 
void update(int v) {
	transformation();
	glutPostRedisplay();
	glutTimerFunc(10, update, 0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 0);
	glutCreateWindow("Functions graph");
	init();
	glutDisplayFunc(display);
	glutTimerFunc(10, update, 0);
	glutMainLoop();
	return 0;
}



