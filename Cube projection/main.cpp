#include "freeglut/include/GL/glut.h"
#include <bevgrafmath2017.h>
#include <math.h>

GLsizei winWidth = 800, winHeight = 600;
mat4 wTv, projectionM, projectionC;
vec3 cube[8] = {};
vec2 listOfPointsAfterTransformationForOrtho[8] = {};
vec2 listOfPointsAfterTransformationForPerspective[8] = {};
vec2 mouse, mouseOldPosition;
vec3 shiftValue, oldShiftValue;

float rotX, rotY, rotZ = 0;
float center = 400;
GLint keyStates[256];

vec2 windowSize = { 200, 200 };
vec2 windowPosition = { -100, -100 };
vec2 viewportSize = { 200, 200 };
vec2 viewportPosition = { winWidth / 2 - viewportSize.x / 2, winHeight / 2 - viewportSize.y / 2 };
vec3 eltolas = {};

void initCube() {
	cube[0] = vec3(-50.0f, -50.0f, -50.0f);
	cube[1] = vec3(50.0f, -50.0f, -50.0f);
	cube[2] = vec3(50.0f, 50.0f, -50.0f);
	cube[3] = vec3(-50.0f, 50.0f, -50.0f);
	cube[4] = vec3(-50.0f, -50.0f, 50.0f);
	cube[5] = vec3(50.0f, -50.0f, 50.0f);
	cube[6] = vec3(50.0f, 50.0f, 50.0f);
	cube[7] = vec3(-50.0f, 50.0f, 50.0f);
}

void init() {
	glClearColor(0.0, 1.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, winWidth, 0.0, winHeight);
	glShadeModel(GL_FLAT);
	glEnable(GL_POINT_SMOOTH);

	glPointSize(5.0);
	glLineWidth(1.0);
	wTv = windowToViewport3(windowPosition, windowSize, viewportPosition, viewportSize);
	initCube();
	projectionM = ortho();
	eltolas = (eltolas - mouse)*-1;
}

void drawCube() {
	
	glEnable(GL_SCISSOR_TEST);

	glScissor(0, 0, 400, 800);
		glBegin(GL_LINE_LOOP);
		glVertex2f(listOfPointsAfterTransformationForOrtho[0].x, listOfPointsAfterTransformationForOrtho[0].y);
		glVertex2f(listOfPointsAfterTransformationForOrtho[1].x, listOfPointsAfterTransformationForOrtho[1].y);
		glVertex2f(listOfPointsAfterTransformationForOrtho[2].x, listOfPointsAfterTransformationForOrtho[2].y);
		glVertex2f(listOfPointsAfterTransformationForOrtho[3].x, listOfPointsAfterTransformationForOrtho[3].y);
		glEnd();

		//Hátsó oldal
		glBegin(GL_LINE_LOOP);
		glVertex2f(listOfPointsAfterTransformationForOrtho[4].x, listOfPointsAfterTransformationForOrtho[4].y);
		glVertex2f(listOfPointsAfterTransformationForOrtho[5].x, listOfPointsAfterTransformationForOrtho[5].y);
		glVertex2f(listOfPointsAfterTransformationForOrtho[6].x, listOfPointsAfterTransformationForOrtho[6].y);
		glVertex2f(listOfPointsAfterTransformationForOrtho[7].x, listOfPointsAfterTransformationForOrtho[7].y);
		glEnd();

		for (int i = 0; i < 4; i++) {
			glBegin(GL_LINES);
			glVertex2f(listOfPointsAfterTransformationForOrtho[i].x, listOfPointsAfterTransformationForOrtho[i].y);
			glVertex2f(listOfPointsAfterTransformationForOrtho[i + 4].x, listOfPointsAfterTransformationForOrtho[i + 4].y);
			glEnd();
		}
	glDisable(GL_SCISSOR_TEST);
	

	glEnable(GL_SCISSOR_TEST);
	glScissor(400, 0, 800, 600);
		glBegin(GL_LINE_LOOP);
		glVertex2f(listOfPointsAfterTransformationForPerspective[0].x, listOfPointsAfterTransformationForPerspective[0].y);
		glVertex2f(listOfPointsAfterTransformationForPerspective[1].x, listOfPointsAfterTransformationForPerspective[1].y);
		glVertex2f(listOfPointsAfterTransformationForPerspective[2].x, listOfPointsAfterTransformationForPerspective[2].y);
		glVertex2f(listOfPointsAfterTransformationForPerspective[3].x, listOfPointsAfterTransformationForPerspective[3].y);
		glEnd();

		//Hátsó oldal
		glBegin(GL_LINE_LOOP);
		glVertex2f(listOfPointsAfterTransformationForPerspective[4].x, listOfPointsAfterTransformationForPerspective[4].y);
		glVertex2f(listOfPointsAfterTransformationForPerspective[5].x, listOfPointsAfterTransformationForPerspective[5].y);
		glVertex2f(listOfPointsAfterTransformationForPerspective[6].x, listOfPointsAfterTransformationForPerspective[6].y);
		glVertex2f(listOfPointsAfterTransformationForPerspective[7].x, listOfPointsAfterTransformationForPerspective[7].y);
		glEnd();

	for (int i = 0; i < 4; i++) {
		glBegin(GL_LINES);
		glVertex2f(listOfPointsAfterTransformationForPerspective[i].x, listOfPointsAfterTransformationForPerspective[i].y);
		glVertex2f(listOfPointsAfterTransformationForPerspective[i + 4].x, listOfPointsAfterTransformationForPerspective[i + 4].y);
		glEnd();
	}
	glDisable(GL_SCISSOR_TEST);
}
void transform() {
	mat4 rotationX = rotateX(rotX);
	mat4 rotationY = rotateY(rotY);
	mat4 rotationZ = rotateZ(rotZ);
	mat4 transToMouse = translate(eltolas);

	mat4 leftM = wTv * transToMouse *  projectionM * rotationX * rotationY * rotationZ;
	mat4 rightM = wTv * transToMouse *  projectionC * rotationX * rotationY * rotationZ;

	for (int j = 0; j < 8; j++) {
		vec4 pointH = ihToH(cube[j]);
		vec4 transformedPoint = leftM * pointH;
		if (transformedPoint.w != 0) {
			vec3 result = hToIh(transformedPoint);
			if (result.z == 0) {
				listOfPointsAfterTransformationForOrtho[j] = { result.x, result.y };
			}
		}
	}

	for (int j = 0; j < 8; j++) {
		vec4 pointH = ihToH(cube[j]);
		vec4 transformedPoint = rightM * pointH;
		if (transformedPoint.w != 0) {
			vec3 result = hToIh(transformedPoint);
			if (result.z == 0) {
				listOfPointsAfterTransformationForPerspective[j] = { result.x, result.y };
			}
		}
	}
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0, 1, 1);
	glRecti(400, 0, 800, 600);
	glColor3f(0, 0, 0);
	drawCube(),
	glBegin(GL_LINES);
		glVertex2f(winWidth/2, 0);
		glVertex2f(winWidth/2, winHeight);
	glEnd();
	glutSwapBuffers();
}
void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse) {
	vec2 mouse;
	mouse.x = xMouse;
	mouse.y = fabs(yMouse - winHeight);

	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
		//Ha lenyomom az egeret elmentem az egér pocicióját
		mouseOldPosition = mouse;
		oldShiftValue = eltolas;
	}
}
void processMouseActiveMotion(GLint xMouse, GLint yMouse) {
	mouse.x = xMouse;
	mouse.y = fabs(yMouse - winHeight);

	shiftValue = (mouseOldPosition - mouse)*-1;
	eltolas = shiftValue + oldShiftValue;
}
void keyPressed(unsigned char key, int x, int y) {
	keyStates[key] = 1;
}

void keyUp(unsigned char key, int x, int y) {
	keyStates[key] = 0;
}

void keyOperations(int value) {
	if (keyStates['x']) { rotX += pi() / 30; }
	if (keyStates['y']) { rotY += pi() / 30; }
	if (keyStates['z']) { rotZ += pi() / 30; }

	if (keyStates['c']) { center +=  1;}
	if (keyStates['v']) { center -= 1; }

	glutPostRedisplay();
	glutTimerFunc(15, keyOperations, 0);
}
void update(int v) {
	projectionC = perspective(center);
	transform();
	glutPostRedisplay();
	glutTimerFunc(10, update, 0);
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 0);
	glutCreateWindow("Cube projection");
	init();
	glutDisplayFunc(display);
	glutMouseFunc(processMouse);
	glutMotionFunc(processMouseActiveMotion);
	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyUp);
	glutTimerFunc(15, keyOperations, 0);
	glutTimerFunc(10, update, 0);
	glutMainLoop();
	return 0;
}