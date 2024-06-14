#include <gl/glut.h>
#include <math.h>
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_POLYGON);
	glColor3f(1.0, 1.0, 1.0);
	glVertex2f(-200, -200);
	glVertex2f(-200, 200);
	glVertex2f(200, 200);
	glVertex2f(200, -200);
	/*glVertex2f(120.0, 50.0);
	glVertex2f(120.0, 90.0);*/
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3f(1.0,0.0,0.0);
	glVertex2f(0.0, 0.0);
	glVertex2f(150.0, -50.0);
	glVertex2f(150.0, 0.0);
	/*glVertex2f(120.0, 50.0);
	glVertex2f(120.0, 90.0);*/
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2f(0.0, 0.0);
	//glVertex2f(150.0, -50.0);
	//glVertex2f(150.0, 0.0);
	glVertex2f(120.0, 50.0);
	glVertex2f(120.0, 90.0);
	glEnd();
	glFlush();
}
void circle()
{
	glClear(GL_COLOR_BUFFER_BIT);
	int n = 3600;
	float r = 1.0;
	float PI = 3.1415926;
	glBegin(GL_LINES);
	glColor3f(1.0,0,0);
	for (int i = 1; i<=n; i++)
	{
		glVertex2f(r*cos(2 * PI * (i-1) / n),r*sin(2*PI*(i-1)/n));
		glVertex2f(r * cos(2 * PI * i / n), r * sin(2 * PI * i / n));
	}
	glEnd();
	glFlush();
}
void star()
{
	glClear(GL_COLOR_BUFFER_BIT);
	float r = 1.0;
	float PI = 3.1415926;
	glBegin(GL_LINES);
	glColor3f(1.0, 0, 0);
	glVertex2f(r * cos(PI/10), r * sin(PI/10));
	glVertex2f(r * cos(9*PI / 10), r * sin(9*PI / 10));
	glVertex2f(r * cos(9 * PI / 10), r * sin(9 * PI / 10));
	glVertex2f(r * cos(17*PI / 10), r * sin(17*PI / 10));
	glVertex2f(r * cos(17*PI / 10), r * sin(17*PI / 10));
	glVertex2f(r * cos(PI / 2), r * sin(PI / 2));
	glVertex2f(r * cos(PI / 2), r * sin(PI / 2));
	glVertex2f(r * cos(13 * PI / 10), r * sin(13 * PI / 10));
	glVertex2f(r * cos(13 * PI / 10), r * sin(13 * PI / 10));
	glVertex2f(r * cos(PI / 10), r * sin(PI / 10));
	glEnd();
	glFlush();
}
void DDALine(int x1,int y1,int x2,int y2) 
{
	int x;
	float dx, dy, y, k;
	dx = x2 - x1;
	dy = y2 - y1;
	k = dy / dx;
	for (x=x1,y=y1;x<=x2;x++,y+=k)
	{
		glBegin(GL_POINTS);
		glVertex2f(x,int(y+0.5));
		glEnd();
	}
	glFlush();
	
}
void Bresenham(int x1, int y1, int x2, int y2)
{
	int x = x1, y = y1;
	int dx = x2 - x1;
	int dy = y2 - y1;
	int d0 = dx - 2 * dy;
	int d1 = 2 * dx - 2 * dy;
	int d2 = - 2 * dy;
	int d = d0;
	for (;x<x2;x++)
	{
		glBegin(GL_POINTS);
		glVertex2f(x, y);
		glEnd();
		if (d < 0)
		{
			y++;
			d += d1;
		}
		else
		{
			d += d2;
		}
	}	
	glFlush();
}
void Bresenham2(int x1, int y1, int x2, int y2)
{
	int x = x1, y = y1;
	int dx = x2 - x1;
	int dy = y2 - y1;
	int d0 = 2*dx - dy;
	int d1 = 2 * dx - 2 * dy;
	int d2 = 2 * dx;
	int d = d0;
	for (; y< y2; y++)
	{
		glBegin(GL_POINTS);
		glVertex2f(x, y);
		glEnd();
		if (d > 0)
		{
			x++;
			d += d1;
		}
		else
		{
			d += d2;
		}
	}
	glFlush();
}
void Test()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0, 0);
	Bresenham2(100, 200, 200, 600);
	
}
void reshape(GLsizei w,GLsizei h)
{

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,200,0,600);

}
void myreshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
	float rate = (float)w / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(w>h)
	gluOrtho2D(-1*rate, 1*rate, -1, 1);
	else
		gluOrtho2D(-1, 1, -1/rate, 1/rate);
}
void init()
{
	glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel(GL_FLAT);
}
void main(int argc,char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	//glutInitWindowSize(500,500);
	//glutInitWindowPosition(0, 0);
	glutCreateWindow("mywindow");
	glutDisplayFunc(Test);
	glutReshapeFunc(reshape);
	init();
	glutMainLoop();
}