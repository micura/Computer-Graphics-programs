#include "freeglut/include/GL/glut.h"
#include <bevgrafmath2017.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include <iostream>

/*==========GLOBÁLIS_ADATOK==========*/
GLsizei winWidth = 800, winHeight = 800;
vec3 feny = { -20,0,20 };

/*==========TRANSZFORMÁCIÓHOZ==========*/
float center = 15;
vec3 vetites = { 0,0,center };
mat4 wTv, projection;

/*==========Interakciókhoz==========*/
float forgatx = 2, forgaty = 0, forgatz = 0;
vec3 eltolas;

struct Lap {
	//egy lap koordinátáit, annak normálvektorát, középpontját, a középpont távolságát a vetítési centrumtól, valamint
	//a lapot érő fény intenzitását.
	vec3 P0, P1, P2, P3;
	vec3 normalvector;
	vec3 kozeppont;
	vec3 distance;
	vec3 fenyintenzitas;
}; std::vector<Lap> lapok;

bool rendezes(Lap &a, Lap &b) {
	return a.distance.z < b.distance.z;
}

void initMatrices() {
	vec2 windowPosition = { -2.5,-2.5 };
	vec2 windowSize = { 5,5 };
	vec2 viewportPosition = { 0, 0 };
	vec2 viewportSize = { 800,800 };
	wTv = windowToViewport3(windowPosition, windowSize, viewportPosition, viewportSize);
	projection = perspective(center);
}

void init() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, winWidth, 0.0, winHeight);
	glShadeModel(GL_FLAT);
	glEnable(GL_POINT_SMOOTH);
	glLineWidth(2.0);
	glPointSize(5.0);
	initMatrices();
}

void initBowl() {
	float deltafi = pi() / 16;
	float deltatheta = pi() / 20;

	int lapszamlalo = 0;
	//Hosszúsági körök
	for (float fi = 0; fi < pi() / 2; fi += deltafi) {
		//Szélességi körök
		for (float theta = 0; theta < 2 * pi(); theta += deltatheta) {
			lapok.push_back(Lap());
			lapok[lapszamlalo].P0 = {
				cos(theta)*sin(fi),
				sin(theta)*sin(fi),
				cos(fi)
			};
			lapok[lapszamlalo].P1 = {
				cos(theta)*sin(fi + deltafi),
				sin(theta)*sin(fi + deltafi),
				cos(fi + deltafi)
			};
			lapok[lapszamlalo].P2 = {
				cos(theta + deltatheta)*sin(fi + deltafi),
				sin(theta + deltatheta)*sin(fi + deltafi),
				cos(fi + deltafi)
			};
			lapok[lapszamlalo].P3 = {
				cos(theta + deltatheta)*sin(fi),
				sin(theta + deltatheta)*sin(fi),
				cos(fi)
			};
			lapszamlalo++;
		}
	}
}
void arnyalas() {
	for (int i = 0; i < lapok.size(); i++) {
		/*
		 * <C-P0, n>
		 */
		float lapvizsgalat = dot(vetites - lapok[i].P0, lapok[i].normalvector);
			/*  Hátsólap vagy nem
			 * I = <ne, fe>		+1
			 *		   2
			 */
		if (lapvizsgalat > 0) {   // ... Akkor a lap felénk néz
			lapok[i].fenyintenzitas = (dot(normalize(lapok[i].normalvector), normalize(feny)) + 1) / 2;
		}
		else {
			lapok[i].fenyintenzitas = (dot(-normalize(lapok[i].normalvector), normalize(feny)) + 1) / 2;
		}
	}
}
void transform() {
	mat4 M = translate(eltolas) * rotateX(forgatx) * rotateY(forgaty) * rotateZ(forgatz);

	for (int i = 0; i < lapok.size(); i++) {
		vec4 pointH = ihToH(lapok[i].P0);
		vec4 transformedPoint = M * pointH;
		if (transformedPoint.w != 0) {
			lapok[i].P0 = hToIh(transformedPoint);
		}

		pointH = ihToH(lapok[i].P1);
		transformedPoint = M * pointH;
		if (transformedPoint.w != 0) {
			lapok[i].P1 = hToIh(transformedPoint);
		}

		pointH = ihToH(lapok[i].P2);
		transformedPoint = M * pointH;
		if (transformedPoint.w != 0) {
			lapok[i].P2 = hToIh(transformedPoint);
		}

		pointH = ihToH(lapok[i].P3);
		transformedPoint = M * pointH;
		if (transformedPoint.w != 0) {
			lapok[i].P3 = hToIh(transformedPoint);
		}

		lapok[i].kozeppont = (lapok[i].P0 + lapok[i].P1 + lapok[i].P2 + lapok[i].P3) / 4;
		lapok[i].distance = dist2(lapok[i].kozeppont, vetites);
		lapok[i].normalvector = cross(lapok[i].P1 - lapok[i].P0, lapok[i].P2 - lapok[i].P0);
	}

	std::sort(lapok.begin(), lapok.end(), rendezes);
	arnyalas();
	M = wTv * projection;

	for (int i = 0; i < lapok.size(); i++) {
		vec4 pointH = ihToH(lapok[i].P0);
		vec4 transformedPoint = M * pointH;
		if (transformedPoint.w != 0) {
			lapok[i].P0 = hToIh(transformedPoint);
		}

		pointH = ihToH(lapok[i].P1);
		transformedPoint = M * pointH;
		if (transformedPoint.w != 0) {
			lapok[i].P1 = hToIh(transformedPoint);
		}

		pointH = ihToH(lapok[i].P2);
		transformedPoint = M * pointH;
		if (transformedPoint.w != 0) {
			lapok[i].P2 = hToIh(transformedPoint);
		}

		pointH = ihToH(lapok[i].P3);
		transformedPoint = M * pointH;
		if (transformedPoint.w != 0) {
			lapok[i].P3 = hToIh(transformedPoint);
		}
	}

	glutPostRedisplay();
}

void drawBowl() {
	lapok.erase(lapok.begin(), lapok.end());
	initBowl();
	transform();

	for (int i = 0; i < lapok.size(); i++) {
		glColor3f(lapok[i].fenyintenzitas.x, lapok[i].fenyintenzitas.y, lapok[i].fenyintenzitas.z);
		glBegin(GL_QUADS);
		glVertex2f(lapok[i].P0.x, lapok[i].P0.y);
		glVertex2f(lapok[i].P1.x, lapok[i].P1.y);
		glVertex2f(lapok[i].P2.x, lapok[i].P2.y);
		glVertex2f(lapok[i].P3.x, lapok[i].P3.y);
		glEnd();
	}
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	drawBowl();
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 27:
			exit(0);
			break;
		case 'q':
			forgatx += pi() / 60;
			break;
		case 'w':
			forgaty += pi() / 60;
			break;
		case 'e':
			forgatz += pi() / 60;
			break;
		case 'a':
			forgatx -= pi() / 60;
			break;
		case 's':
			forgaty -= pi() / 60;
			break;
		case 'd':
			forgatz -= pi() / 60;
			break;

		case 'r':
			eltolas.x += 0.1;
			break;
		case 'f':
			eltolas.x -= 0.1;
			break;

		transform();
		glutPostRedisplay();
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(300, 0);
	glutCreateWindow("The bowl");
	init();
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}