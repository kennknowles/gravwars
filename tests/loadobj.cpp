#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "../include/video.h"
#include "../include/input.h"

using namespace gravwars;

VideoCore *video;
ObjModel *obj;
int objIndex;

void display() {
	cerr << "drawing model test" << endl;
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	gluLookAt(	0, 0, 10,
				0, 0, 0,
				0, 1, 0 );

	glColor3f( 1.0, 0.0, 0.0 );
	
	cerr << "Cube" << endl;
	glutSolidCube( 0.5 );

	cerr << "List" << endl;
	glCallList( objIndex );

	glFlush();
	glutSwapBuffers();
}

void reshape( int width, int height ) {
	cerr << "reshaping" << endl;
	glViewport( 0, 0, width, height );
	
	glutPostRedisplay();
}

void mouse( int button, int state, int x, int y ) {
	glutPostRedisplay();
}

void keyboard( unsigned char key, int x, int y ) {	
	InputType i;
	
	if( (key == 'q') || (key == 'Q') )
		exit(0);
}

int main( int argc, char* argv[] ) {	
	video = new VideoCore();

	obj = new ObjModel( argv[1] );
	cerr << (*obj) << endl;

	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
	glutCreateWindow("Model test");
	glutReshapeWindow(600, 450);


	glutDisplayFunc(display);

	glutMouseFunc( mouse );

	glutKeyboardFunc( keyboard );

	glClearColor( 0.0, 0.0, 0.0, 1.0 );
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPerspective( 45,
					glutGet(GLenum(GLUT_WINDOW_WIDTH)) /
					glutGet(GLenum(GLUT_WINDOW_HEIGHT)),
					0.5,
					4000 );

	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	objIndex = video->objList( *obj, 1.0 );

	cerr << "Index is: " << objIndex << endl;

	glutMainLoop();

	return( 0 );
}
