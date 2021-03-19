#include <iostream>
#include <GL/glut.h>
using namespace std;

int WindowPositionX = 1100;
int WindowPositionY = 100;
int WindowWidth = 512;
int WindowHeight = 512;
char WindowTitle[] = "RK4";

const double mass = 1.0;				
const double gravity = 9.80665;	
const double dt = 0.001;				
const double k = 10.0;				
const double natural_length = 0.0;	
const double first_accel = gravity;	
const double first_velocity = 0.0;
const double first_position = 1.0;	

double R_x = first_position;	
double R_v = first_velocity;
double t = 0;
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

double F1( double t, double x, double v ){
	return -k/mass*R_x;
}

double F2(double t,double x,double v)
{
    return v;
}

void Runge_Kutta(){
	t += dt;

	double k1_x = F2(t, R_x, R_v);	
	double k1_v = F1(t, R_x, R_v);
    double k2_x = F2(t + dt/2, R_x + k1_x*dt/2, R_v + k1_v*dt/2);   
	double k2_v = F1(t + dt/2, R_x + k1_x*dt/2, R_v + k1_v*dt/2);
    double k3_x = F2(t + dt/2, R_x + k2_x*dt/2, R_v + k2_v*dt/2);
	double k3_v = F1(t + dt/2, R_x + k2_x*dt/2, R_v + k2_v*dt/2);
	double k4_x = F2(t + dt  , R_x + k3_x*dt,   R_v + k3_v*dt/2);
	double k4_v = F1(t + dt  , R_x + k3_x*dt,   R_v + k3_v*dt/2);
		
	R_v += dt / 6 *(k1_v + 2*k2_v + 2*k3_v + k4_v );
	R_x += dt / 6 *(k1_x + 2*k2_x + 2*k3_x + k4_x );
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

    Runge_Kutta();

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
	glVertex3d(0.0, R_x, 0.0);
	glEnd();

	glPushMatrix();
	glColor4fv(object_color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, object_color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, object_color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, object_color);
	glTranslated(0.0, R_x, 0.0);
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