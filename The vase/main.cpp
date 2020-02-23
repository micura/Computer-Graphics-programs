#include "freeglut/include/GL/glut.h"
#include <bevgrafmath2017.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include <iostream>

/*==========GLOBÁLIS_ADATOK==========*/
GLsizei winWidth = 800, winHeight = 750;
GLint keyStates[256];
vec3 points[6] = { 
	{ 200.0f,	0.0f,		0.0f },  //0
	{ 350.0f,	100.0f,		0.0f },  //1
	{ 350.0f,	300.0f,		0.0f },  //2
	{ 300.0f,	300.0f,		0.0f },  //3
	{ 5.0f,		450.0f,		0.0f },  //4
	{ 200.0f,	450.0f,		0.0f },  //5
};

/*==========TRANSZFORMÁCIÓHOZ==========*/
float center = 700;
mat4 wTv, projection;

/*==========KAMERÁHOZ==========*/
vec3 centerPersp = { 0, 0, center };
mat4 camera;
vec3 cameraStartingPosition;
vec3 origo = { 0, 0, 0 };	//Ide néz
vec3 up = { 0, 1, 0};
float radius = 50;
float cylinderHigh = 100;
float alpha = 50;

struct Side {
	vec3 A, B, C, D;
	vec3 center;
	vec3 norma;

}; std::vector<Side> sides;

bool sorter(Side &a, Side &b) {
	//std::cout << a.center.z << std::endl;
	bool test = a.center.z < b.center.z;
	return test;
}
double binomialCoefficient(int n, int k) {
	if (k > n - k)
		k = n - k;
	double c = 1.0;
	for (int i = 0; i < k; i++) {
		c = c * (n - i);
		c = c / (i + 1);
	}
	return c;
}

float brezierX(float t) {
	vec2 brezier;
	int n = 5;
	for (int i = 0; i <= n; i++) {
		brezier.x += binomialCoefficient(n, i)*pow(t, i)*pow(1 - t, n - i)*points[i].x;
	}
	return brezier.x;
}
float brezierY(float t) {
	vec2 brezier;
	int n = 5;
	for (int i = 0; i <= n; i++) {
		brezier.y += binomialCoefficient(n, i)*pow(t, i)*pow(1 - t, n - i)*points[i].y;
	}
	return brezier.y;
}

void initMatrices() {
	vec2 windowSize = { 800,800 };
	vec2 windowPosition = { -400,-200 };
	vec2 viewportSize = { 300,300 };
	vec2 viewportPosition = { winWidth / 2 - viewportSize.x / 2, winHeight / 2 - viewportSize.x / 2 };
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
	initMatrices();
}

void initVase() {
	float deltatheta = pi() / 8;
	int numberofsteps = 15;
	float incValue = 1.0 / (float)numberofsteps;

	int j = 0;
	for (float theta = 0; theta <= 2 * pi(); theta += deltatheta) {
		float t = 0.0;
		sides.push_back(Side());
		sides[j].A = {
			cos(theta)*brezierX(t),
			brezierY(t),
			-sin(theta)*brezierX(t) };
		sides[j].B = {
			cos(theta + deltatheta)*brezierX(t),
			brezierY(t),
			-sin(theta + deltatheta)*brezierX(t) };
		sides[j].C = { 0,0,0 };
		sides[j].D = { 0,0,0 };
		j++;
		
		for (float t = 0.0; t < 1.0; t += incValue) {
			sides.push_back(Side());

			sides[j].A = { 
				cos(theta)*brezierX(t),
				brezierY(t),
				-sin(theta)*brezierX(t) };

			sides[j].B = { 
				cos(theta + deltatheta)*brezierX(t), 
				brezierY(t),
				-sin(theta + deltatheta)*brezierX(t) };

			sides[j].C = { 
				cos(theta + deltatheta)*brezierX(t + incValue), 
				brezierY(t + incValue),
				-sin(theta + deltatheta)*brezierX(t + incValue) };

			sides[j].D = { 
				cos(theta)*brezierX(t + incValue), 
				brezierY(t + incValue),
				-sin(theta)*brezierX(t + incValue) };

			j++;
		}
	}
}

void look() {
	//Kamera kezdőpoziciója
	cameraStartingPosition = { radius * cos(alpha),
		cylinderHigh,
		-radius*sin(alpha) };   
	vec3 newZ = normalize(cameraStartingPosition - origo);
	vec3 newX = normalize(cross(up, newZ));
	vec3 newY = normalize(cross(newZ, newX));
	camera = coordinateTransform(cameraStartingPosition, newX, newY, newZ);
}
void transform() {
	for (int i = 0; i < sides.size(); i++) {
		vec4 pointH = ihToH(sides[i].A);
		vec4 transformedPoint = camera * pointH;
		if (transformedPoint.w != 0) {
			sides[i].A = hToIh(transformedPoint);
		}
		
		pointH = ihToH(sides[i].B);
		transformedPoint = camera * pointH;
		if (transformedPoint.w != 0) {
			sides[i].B = hToIh(transformedPoint);
		}

		pointH = ihToH(sides[i].C);
		transformedPoint = camera * pointH;
		if (transformedPoint.w != 0) {
			sides[i].C = hToIh(transformedPoint);
		}

		pointH = ihToH(sides[i].D);
		transformedPoint = camera * pointH;
		if (transformedPoint.w != 0) {
			sides[i].D = hToIh(transformedPoint);
		}

		sides[i].norma = cross(sides[i].B - sides[i].A, sides[i].D - sides[i].A);
		sides[i].center = (sides[i].A + sides[i].B + sides[i].C + sides[i].D) / 4;
	}
	
	std::sort(sides.begin(), sides.end(), sorter);
	
	mat4 M = wTv * projection;

	for (int i = 0; i < sides.size(); i++) {
		vec4 pointH = ihToH(sides[i].A);
		vec4 transformedPoint = M *pointH;
		if (transformedPoint.w != 0) {
			sides[i].A = hToIh(transformedPoint);
		}

		pointH = ihToH(sides[i].B);
		transformedPoint = M*pointH;
		if (transformedPoint.w != 0) {
			sides[i].B = hToIh(transformedPoint);
		}

		pointH = ihToH(sides[i].C);
		transformedPoint = M*pointH;
		if (transformedPoint.w != 0) {
			sides[i].C = hToIh(transformedPoint);
		}

		pointH = ihToH(sides[i].D);
		transformedPoint =  M*pointH;
		if (transformedPoint.w != 0) {
			sides[i].D = hToIh(transformedPoint);
		}
	}
	glutPostRedisplay();
}

void drawVase() {
	sides.erase(sides.begin(), sides.end());
	initVase();
	transform();
	vec4 feny = { 0, 1, 0, 0 };
	feny = transpose(inverse(camera)) * feny;	//inverz transzparalt, hogy átvigyük az új koordináta rendszerbe

	vec3 ujfeny = { feny.x,feny.y ,feny.z };
	double color;

	for (int i = 0; i < sides.size(); i++) {
		//Szinezés
		if (dot(sides[i].norma, (centerPersp - sides[i].center)) > 0) {
			color = (dot(normalize(sides[i].norma), ujfeny) + 1) / 2;
		}
		else{
			color = (dot(-1 * (normalize(sides[i].norma)), ujfeny) + 1) / 2;
		}

		glColor3f(color, color, color);
		glBegin(GL_QUADS);
			glVertex2f(sides[i].A.x, sides[i].A.y);
			glVertex2f(sides[i].B.x, sides[i].B.y);
			glVertex2f(sides[i].C.x, sides[i].C.y);
			glVertex2f(sides[i].D.x, sides[i].D.y);
		glEnd();
		
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_LINE_STRIP);
			glVertex2f(sides[i].A.x, sides[i].A.y);
			glVertex2f(sides[i].B.x, sides[i].B.y);
			glVertex2f(sides[i].C.x, sides[i].C.y);
			glVertex2f(sides[i].D.x, sides[i].D.y);
		glEnd();
	}
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0, 0, 0);
	look();
	drawVase();
	glutSwapBuffers();
}

void keyPressed(unsigned char key, int x, int y) {
	keyStates[key] = 1;
}

void keyUp(unsigned char key, int x, int y) {
	keyStates[key] = 0;
}

void keyOperations(int value) {
	if (keyStates['a']) { radius -= 1; } 
	if (keyStates['q']) { radius += 1; }
	if (keyStates['s']) { cylinderHigh -= 1; }
	if (keyStates['w']) { cylinderHigh += 1; }
	if (keyStates['d']) { alpha -= 0.1; }
	if (keyStates['e']) { alpha += 0.1; }
	if (keyStates['f']) { center -= 1; }
	if (keyStates['r']) { center += 1; }
	
	glutPostRedisplay();
	glutTimerFunc(5, keyOperations, 0);
}

void update(int n) {
	projection = perspective(center);
	glutPostRedisplay();
	glutTimerFunc(5, update, 0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(300, 0);
	glutCreateWindow("The vase");
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyUp);
	glutTimerFunc(5, keyOperations, 0);
	glutTimerFunc(5, update, 0);
	glutMainLoop();
	return 0;
}