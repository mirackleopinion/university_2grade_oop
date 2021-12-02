// DASHA TEREKHOVA K-20 LAB 3 (Диаграмма Вороного алгоритм Форчуна)
//  полезные ссылки
// https://habr.com/ru/post/309252/
// https://neerc.ifmo.ru/wiki/index.php?title=%D0%94%D0%B8%D0%B0%D0%B3%D1%80%D0%B0%D0%BC%D0%BC%D0%B0_%D0%92%D0%BE%D1%80%D0%BE%D0%BD%D0%BE%D0%B3%D0%BE
// http://koi.tspu.ru/koi_books/dolganova/1.4.4.html

#include <GL/glew.h>
#include <GL/glut.h>
#include "Voronoi.h"
#include "VPoint.h"

void display(void);
void onEF(int n);
void reshape(int width, int height);

vor::Voronoi * v;
vor::Vertices * ver; // фокусы
vor::Vertices * dir; // скорости
vor::Edges * edg; // ребра

double w = 10000; // ограничения вокруг сцены
const int POINTS_COUNT = 100; // количество точек
const float SPEED_DIVIDER = 500; // делитель скорости, чем больше - тем медленнее

int main (int argc, char **argv) 
{
	using namespace vor;

	v = new Voronoi();
	ver = new Vertices();
	dir = new Vertices();

	srand ( time(NULL) );

	for(int i = 0; i < POINTS_COUNT; i++)
	{
		ver->push_back(
			new VPoint( 
				w * (double)rand()/(double)RAND_MAX , 
				w * (double)rand()/(double)RAND_MAX 
			)
		);

		dir->push_back(
			new VPoint(
				(double)rand()/(double)RAND_MAX - 0.5, 
				(double)rand()/(double)RAND_MAX - 0.5
			)
		);
	}
	
	glutInit(&argc, argv); // Initialize GLUT
	glutInitDisplayMode (GLUT_SINGLE); // Set up a basic display buffer (only single buffered for now)
	glutInitWindowSize (800, 800); // Set the width and height of the window
	glutInitWindowPosition (100, 100); // Set the position of the window
	glutCreateWindow ("Lab 03"); // Set the title for the window

	// anti aliasing
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH, GL_NICEST);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH, GL_NICEST);
	
	glutTimerFunc(100, onEF, 0); // вечный двигатель Форчуна
	glutDisplayFunc(display); // Tell GLUT to use the method "display" for rendering
	glutReshapeFunc(reshape); // Tell GLUT to use the method "reshape" for reshaping
	glutMainLoop(); // Enter GLUT's main loop
	return 0;
}

void drawVoronoi()
{
	vor::Vertices::iterator j = dir->begin();
	for(vor::Vertices::iterator i = ver->begin(); i != ver->end(); ++i)
	{
		(*i)->x += (*j)->x * w/SPEED_DIVIDER;
		(*i)->y += (*j)->y * w/SPEED_DIVIDER;
		if( (*i)->x > w || (*i)->x < 0) (*j)->x *= -1;
		if( (*i)->y > w || (*i)->y < 0) (*j)->y *= -1;
		++j;
	}
	
	int c = 10;

	edg = v->GetEdges(ver, w, w);  // рассчет ребер

	for(vor::Vertices::iterator i = ver->begin(); i!= ver->end(); ++i)
	{
		glBegin(GL_QUADS);
		glColor3b(c*2, c*3, c*4);
		glVertex2f( -1+2*(*i)->x/w -0.01,  -1+2*(*i)->y/w - 0.00);
		glVertex2f( -1+2*(*i)->x/w +0.00,  -1+2*(*i)->y/w - 0.01);
		glVertex2f( -1+2*(*i)->x/w +0.01,  -1+2*(*i)->y/w + 0.00);
		glVertex2f( -1+2*(*i)->x/w -0.00,  -1+2*(*i)->y/w + 0.01);
		glEnd();
		c += 2;
	}
	
	for(vor::Edges::iterator i = edg->begin(); i!= edg->end(); ++i)
	{
		glBegin(GL_LINES);
		glColor3b(120, 140, 160); // цвет начала линии
		glVertex2f(-1+2*(*i)->start->x/w, -1+2*(*i)->start->y/w);
		glColor3b(20, 40, 60); // цвет конца линии
		glVertex2f( -1+2*(*i)->end->x/w, -1+2*(*i)->end->y/w);
		glEnd();
	}
}

void display (void) 
{
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -5.0f); 
	glFlush();
}


void onEF(int n)
{
	glutTimerFunc(50, onEF, 0);
	glClear(GL_COLOR_BUFFER_BIT); //Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // цвет заливки фона
	
	drawVoronoi();
	glutSwapBuffers();
}

void reshape (int width, int height) 
{
	glViewport(0, 0, (GLsizei)width, (GLsizei)height); // Set our viewport to the size of our window
	glMatrixMode(GL_PROJECTION); // Switch to the projection matrix so that we can manipulate how our scene is viewed
	glLoadIdentity(); // Reset the projection matrix to the identity matrix so that we don't get any artifacts (cleaning up)
	gluPerspective(22.5, (GLfloat)width / (GLfloat)height, 1.0, 100.0); // Set the Field of view angle (in degrees), the aspect ratio of our window, and the new and far planes
	glMatrixMode(GL_MODELVIEW); // Switch back to the model view matrix, so that we can start drawing shapes correctly
}