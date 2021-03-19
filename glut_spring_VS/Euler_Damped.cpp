#include <iostream>
#include <GL/glut.h>
using namespace std;

int WindowPositionX = 100;
int WindowPositionY = 100;
int WindowWidth = 512;
int WindowHeight = 512;
char WindowTitle[] = "Spring";

int t =0;

const double mass = 1.0;				
const double gravity = 9.80665;	
const double dt = 0.0001;				
const double k = 10.0;				
const double natural_length = 0.0;	
const double first_accel = gravity;	
const double first_velocity = 0.0;
const double first_position = 5.0;	

double E_x = first_position;	
double E_v = first_velocity;	
double b = 0.5;
double accel = first_accel;	

GLfloat object_color[4] = { 0.0, 1.0, 0.0, 1.0 };	
GLfloat line_color[4] = { 1.0, 1.0, 1.0, 1.0 };	
GLfloat light_color[4] = { 1.0, 1.0, 1.0, 1.0 };	
GLfloat light_position[4] = { 100.0, 100.0, 100.0, 1.0 };	

void Initialize();
void Display();
void Idle();
void Reshape(int x, int y);
void Keyboard(unsigned char key, int x, int y);
void Euler_Method();	

int main( int argc, char **argv )
{
	glutInit(&argc, argv);
	glutInitWindowPosition(WindowPositionX, WindowPositionY);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow(WindowTitle);
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Idle);
	glutKeyboardFunc(Keyboard);
	
	Initialize();
	
	glutMainLoop();
	return 0;
}

void Euler_Method()
{
	double force = 0.0;

	force = -k*(E_x - natural_length) + mass * gravity - b*E_x/dt;		

	accel = force / mass;

	E_v += accel * dt;

	E_x += E_v * dt;	
}


void Initialize()
{
	glClearColor(0.3, 0.3, 0.5, 1.0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, light_color);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
}

void Idle()
{
	glutPostRedisplay();
}

void Reshape(int x, int y)
{
	WindowWidth = x;
	WindowHeight = y;
	if ( WindowWidth < 1 ) WindowWidth = 1;
	if ( WindowHeight < 1 ) WindowHeight = 1;
}

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, (double)WindowWidth/(double)WindowHeight, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, WindowWidth, WindowHeight);

	glPushMatrix();

	gluLookAt(0.0, 0.0, -50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	Euler_Method();

	glPushMatrix();
	glColor4fv(line_color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, line_color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, line_color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, line_color);
	glTranslated(0.0, 10.0, 0.0);
	glutSolidSphere(1.0, 20, 20);
	glPopMatrix();

	glLineWidth(5.0);
	glBegin(GL_LINES);	
	glColor4fv(line_color);
	glVertex3d(0.0, 10.0, 0.0);
	glVertex3d(0.0, E_x, 0.0);
	glEnd();

	glPushMatrix();
	glColor4fv(object_color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, object_color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, object_color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, object_color);
	glTranslated(0.0, E_x, 0.0);
	glutSolidSphere(2.0, 20, 20);
	glPopMatrix();

	glPopMatrix();

	glutSwapBuffers();
}

void Keyboard(unsigned char key, int x, int y)
{
	switch ( key )
	{
	case '\033':
		exit(0);
		break;

	default:
		break;
	}
}