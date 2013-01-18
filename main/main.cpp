#include <iostream>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

#include "../include/control.h"
#include "../include/update.h"
#include "../include/input.h"
#include "../include/video.h"
#include "../include/flags.h"
#include "../include/proportions.h"

using namespace gravwars;

int lastIdleTime=0;
InputCore* input;

GLfloat lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat spec_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };

void displayStars() {
//	glDisable( GL_DEPTH_TEST );
	glDisable( GL_LIGHTING );
//	glPushMatrix();
//	glScalef( PARTICLE_SIZE, PARTICLE_SIZE, PARTICLE_SIZE );
	glBegin( GL_POINTS );
	for(	vector<Particle>::const_iterator iter = Space::stars.begin();
			iter != Space::stars.end();
			iter++ ) {
//		glPushMatrix();
		glColor3f(	iter->curIntensity,
					iter->curIntensity,
					iter->curIntensity );
		glVertex3f( iter->pos(0), iter->pos(1), iter->pos(2) );

//		glTranslatef( iter->pos(0), iter->pos(1), iter->pos(2) );
//		glutSolidTetrahedron();

//		glPopMatrix();
/*		glPushMatrix();
		glTranslatef( iter->pos(0), iter->pos(1), iter->pos(2) );
		glutSolidCube( 10.0 );
		glPopMatrix();*/
//		cerr << "drawing particle at point " << iter->pos << endl;
	}
//	glPopMatrix();
	glEnd();
	glEnable( GL_LIGHTING );
//	glEnable( GL_DEPTH_TEST );

}

void displayParticles() {
	glDisable( GL_LIGHTING );
//	glPushMatrix();
//	glScalef( PARTICLE_SIZE, PARTICLE_SIZE, PARTICLE_SIZE );
	glBegin( GL_POINTS );
	for(	vector<Particle>::const_iterator iter = Space::particles.begin();
			iter != Space::particles.end();
			iter++ ) {
//		glPushMatrix();
		glColor3f(	iter->curIntensity,
					iter->curIntensity,
					iter->curIntensity );
		glVertex3f( iter->pos(0), iter->pos(1), iter->pos(2) );

//		glTranslatef( iter->pos(0), iter->pos(1), iter->pos(2) );
//		glutSolidTetrahedron();

//		glPopMatrix();
/*		glPushMatrix();
		glTranslatef( iter->pos(0), iter->pos(1), iter->pos(2) );
		glutSolidCube( 10.0 );
		glPopMatrix();*/
//		cerr << "drawing particle at point " << iter->pos << endl;
	}
//	glPopMatrix();
	glEnd();
	glEnable( GL_LIGHTING );
}

void displayPlanets() {
	for(	vector<Planet>::const_iterator iter = Space::planets.begin();
			iter != Space::planets.end();
			iter++ ) {
		glCallList( iter->glIndex );
	}
}

void displayAsteroids() {
	for(	vector<Asteroid>::const_iterator iter = Space::asteroids.begin();
			iter != Space::asteroids.end();
			iter++ ) {
		glPushMatrix();
		glTranslatef( iter->pos(0), iter->pos(1), iter->pos(2) );
		glRotatef( iter->angle*180/PI, iter->axis(0), iter->axis(1), iter->axis(2) );
		glCallList( iter->glIndex );
		glPopMatrix();
	}
}

void displayShots() {
	for(	vector<Shot>::const_iterator iter = Space::shots.begin();
			iter != Space::shots.end();
			iter++ ) {
		glPushMatrix();
		glTranslatef( iter->pos(0), iter->pos(1), iter->pos(2) );
		glCallList( iter->glIndex );
		glPopMatrix();
	}

}

void display() {
	Ship s = Space::ships[0];

	double	cameradist = 5.0,
			cameraheight = 1.0;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	glMatrixMode( GL_MODELVIEW );

//******************************************************

	glLoadIdentity();
	Vector3 camera, view;
	for( int i=0; i<3; i++ )
		camera(i) =	s.pos(i)	- cameradist * s.radius * s.forwards(i)
								+ cameraheight * s.radius * s.up(i);

	gluLookAt(	camera(0), camera(1), camera(2),
				s.pos(0), s.pos(1), s.pos(2),
				s.up(0), s.up(1), s.up(2) );


	// let me see the border of the level
	/*Vector3 t1 = Space::max, t2 = Space::min;
	glColor3f( 0.7, 0.7, 0.7 );
	glutWireCube( Space::max(0) - Space::min(0) );
*/
	glMateriali( GL_FRONT, GL_SHININESS, 0 );

	Vector3 offset(	Space::max(0) - Space::min(0),
					Space::max(1) - Space::min(1),
					Space::max(2) - Space::min(2) );
	if( Flags::stars_enabled ) {
		for( int xquad = -1; xquad < 2; xquad++ )
			for( int yquad = -1; yquad < 2; yquad++ )
				for( int zquad = -1; zquad < 2; zquad++ ) {
					glPushMatrix();
					glTranslatef( xquad*offset(0), yquad*offset(1), zquad*offset(2) );
					displayStars();
					glPopMatrix();
		}
		glClear( GL_DEPTH_BUFFER_BIT );
	}	

	for( int xquad = -1; xquad < 2; xquad++ )
		for( int yquad = -1; yquad < 2; yquad++ )
			for( int zquad = -1; zquad < 2; zquad++ ) {
				glPushMatrix();
				glTranslatef( xquad*offset(0), yquad*offset(1), zquad*offset(2) );
				glLightfv( GL_LIGHT0, GL_POSITION, lightPos );
				displayPlanets();
				displayAsteroids();
				displayShots();
				if( Flags::particles_enabled )
					displayParticles();
				glPopMatrix();
	} // all quadrants
	
	//****************************************************
	if( !s.dead ) {
		glMateriali( GL_FRONT, GL_SHININESS, 50 );
	
		glLoadIdentity();
		gluLookAt(	0, cameraheight * s.radius, -cameradist * s.radius,
					0, 0, 0,
					0, 1, 0 );

		// draw ship
	//	glDisable( GL_DEPTH_TEST );
	//	glEnable( GL_BLEND );
	//	glColor4f( 0.7, 0.7, 0.7, 0.6 );
		glColor3f( 0.7, 0.7, 0.7 );
		glPushMatrix();
		glRotatef( 180, 0, 1, 0);
		glCallList( s.glIndex );
		glPopMatrix();

		glPushMatrix();
		int rings, sides;
		if( Flags::low_quality ) {
			rings = CROSSHAIR_RINGS_LOW;
			sides = CROSSHAIR_SIDES_LOW;
		}
		else {
			rings = CROSSHAIR_RINGS;
			sides = CROSSHAIR_SIDES;
		}
		glTranslatef( 0, 0, CROSSHAIR_DIST );
//		glRotatef( 90, 1, 1, 1 );
		glColor3f( 1.0, 0.0, 0.0 );
		glutWireTorus(	CROSSHAIR_INNER_RAD,
						CROSSHAIR_OUTER_RAD,
						sides, rings );
		glBegin( GL_LINES );

		glVertex3f( 0, CROSSHAIR_OUTER_RAD, 0 );
		glVertex3f( 0, -CROSSHAIR_OUTER_RAD, 0 );

		
		glVertex3f( CROSSHAIR_OUTER_RAD, 0, 0 );
		glVertex3f( -CROSSHAIR_OUTER_RAD, 0, 0 );

		glEnd();

		glTranslatef( 0, 0, CROSSHAIR_DIST );
//		glRotatef( 90, 1, 1, 1 );
		glColor3f( 1.0, 0.0, 0.0 );
		glutWireTorus(	CROSSHAIR_INNER_RAD,
						CROSSHAIR_OUTER_RAD,
						sides, rings );
		glBegin( GL_LINES );

		glVertex3f( 0, CROSSHAIR_OUTER_RAD, 0 );
		glVertex3f( 0, -CROSSHAIR_OUTER_RAD, 0 );

		
		glVertex3f( CROSSHAIR_OUTER_RAD, 0, 0 );
		glVertex3f( -CROSSHAIR_OUTER_RAD, 0, 0 );
		
		glEnd();
		glPopMatrix();

//	glDisable( GL_BLEND );
	//	glEnable( GL_DEPTH_TEST );
	
		// to indicate direction of positive x and y and z
		// y
	/*	glPushMatrix();
		glTranslatef( 0.0, 10.0, 0.0 );
		glColor3f( 1.0, 0.0, 0.0 );
		glutSolidCube( 1.0 );
		glPopMatrix();
		
		// x
		glPushMatrix();
		glTranslatef( 10.0, 0.0, 0.0 );
		glColor3f( 0.0, 1.0, 0.0 );
		glutSolidCube( 1.0 );
		glPopMatrix();
	
		// z
		glPushMatrix();
		glTranslatef( 0.0, 0.0, 10.0 );
		glColor3f( 0.0, 0.0, 1.0 );
		glutSolidCube( 1.0 );
		glPopMatrix();*/
	}
	glFlush();
	glutSwapBuffers();
	
}

void reshape( int width, int height ) {
	glViewport( 0, 0, width, height );
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective(	45,
					width /
					height,
					0.5,
					(Space::max(0) - Space::min(0) ) );
	
	glMatrixMode( GL_MODELVIEW );
	
	glutPostRedisplay();
}

void mouse( int button, int state, int x, int y ) {
}

void motion( int x, int y ) {
}

void dispatchControl( Control c ) {

	switch( c ) {
	case FIRE:
//		cerr << "firing\n";
		Space::ships[0].fire();
		break;
	case THRUST_FORWARDS:
//		cerr << "thrusting forwards\n";
		Space::ships[0].thrustForwards();
		break;
	case THRUST_DOWN:
//		cerr << "thrusting down\n";
		Space::ships[0].thrustDown();
		break;
	case THRUST_UP:
//		cerr << "thrusting up\n";
		Space::ships[0].thrustUp();
		break;
	case THRUST_RIGHT:
//		cerr << "thrusting right\n";
		Space::ships[0].thrustRight();
		break;
	case THRUST_LEFT:
//		cerr << "thrusting left\n";
		Space::ships[0].thrustLeft();
		break;
	case QUIT:
//		cerr << "quitting\n";
		exit(0);
		break;
	case PITCH_UP:
//		cerr << "Pitch up\n";
		Space::ships[0].pitchUp();
		break;
	case PITCH_DOWN:
//		cerr << "Pitch down\n";
		Space::ships[0].pitchDown();
		break;
	case TURN_LEFT:
//		cerr << "Turning left\n";
		Space::ships[0].turnLeft();
		break;
	case TURN_RIGHT:
//		cerr << "Turning right\n";
		Space::ships[0].turnRight();
		break;
	case ROLL_RIGHT:
//		cerr << "Rolling right\n";
		Space::ships[0].rollRight();
		break;
	case ROLL_LEFT:
//		cerr << "Rolling left\n";
		Space::ships[0].rollLeft();
		break;
	case TOGGLE_PARTICLES:
		Flags::particles_enabled = !Flags::particles_enabled;
		switch( Flags::particles_enabled ) {
		case true:
			cerr << "particles enabled\n";
			break;
		case false:
			cerr << "particles disabled\n";
			break;
		}
		break;
	case TOGGLE_GRAVITY:
		
		switch( Flags::gravity_enabled ) {
		case true:
			cerr << "disabling gravity\n";
			break;
		case false:
			cerr << "enabling gravity\n";
			break;
		}
		Flags::gravity_enabled = !Flags::gravity_enabled;
		break;
	case TOGGLE_QUALITY:
		Flags::low_quality = !Flags::low_quality;
		Space::recreateGraphics();
		switch( Flags::low_quality ) {
		case true:
			cerr << "low quality\n";
			break;
		case false:
			cerr << "high quality\n";
			break;
		}
		break;
	case TOGGLE_STARS:
		switch( Flags::stars_enabled ) {
		case true:
			cerr << "disabling stars\n";
			break;
		case false:
			cerr << "enabling stars\n";
			break;
		}
		Flags::stars_enabled = !Flags::stars_enabled;
		break;
	case TOGGLE_QWERTY:
		Flags::qwerty_enabled = !Flags::qwerty_enabled;
		input->rebindKeys();
		switch( Flags::qwerty_enabled ) {
		case true:
			cerr << "qwerty enabled\n";
			break;
		case false:
			cerr << "qwerty disabled\n";
			break;
		}
		break;
		
	}
}

void dispatchControlEnd( Control c ) {
	
	switch( c ) {
	case THRUST_FORWARDS:
//		cerr << "end thrusting forwards\n";
		Space::ships[0].thrustForwardsEnd();
		break;
	case THRUST_DOWN:
//		cerr << "end thrusting down\n";
		Space::ships[0].thrustDownEnd();
		break;
	case THRUST_UP:
//		cerr << "end thrusting up\n";
		Space::ships[0].thrustUpEnd();
		break;
	case THRUST_RIGHT:
//		cerr << "end thrusting right\n";
		Space::ships[0].thrustRightEnd();
		break;
	case THRUST_LEFT:
//		cerr << "end thrusting left\n";
		Space::ships[0].thrustLeftEnd();
		break;
	case QUIT:
//		cerr << "end quitting\n";
		exit(0);
		break;
	case PITCH_UP:
//		cerr << "end Pitch up\n";
		Space::ships[0].pitchUpEnd();
		break;
	case PITCH_DOWN:
//		cerr << "end Pitch down\n";
		Space::ships[0].pitchDownEnd();
		break;
	case TURN_LEFT:
//		cerr << "end Turning left\n";
		Space::ships[0].turnLeftEnd();
		break;
	case TURN_RIGHT:
//		cerr << "end Turning right\n";
		Space::ships[0].turnRightEnd();
		break;
	case ROLL_RIGHT:
//		cerr << "end Rolling right\n";
		Space::ships[0].rollRightEnd();
		break;
	case ROLL_LEFT:
//		cerr << "end Rolling left\n";
		Space::ships[0].rollLeftEnd();
		break;
	}

}

void keyboard( unsigned char key, int x, int y ) {	
	InputType i;
//	cout << "keydown: " << key << endl;
	if( key=='\n' )
		cerr << "A new line!\n";

	dispatchControl( input->getControl( InputType(key) ) );
}

InputType glutToInputType( int key ) {
	
	switch( key ) {
	case GLUT_KEY_LEFT:
		return( KEY_LEFT );
	case GLUT_KEY_DOWN:
		return( KEY_DOWN );
	case GLUT_KEY_UP:
		return( KEY_UP );
	case GLUT_KEY_RIGHT:
		return( KEY_RIGHT );
	default:
		return( KEY_NONE );
	}
	
}

void keyboardup( unsigned char key, int x, int y ) {
	InputType i;
//	cout << "keyup: " << key << endl;

	dispatchControlEnd( input->getControl( InputType(key) ) );
}

void specialkey( int key, int x, int y ) {
	InputType i;

	dispatchControl( input->getControl( glutToInputType(key) ) );
}

void specialkeyup( int key, int x, int y ) {
	InputType i;
	
	dispatchControlEnd( input->getControl( glutToInputType(key) ) );
}

void idle() {
	int currentTime;
	double dtime;

	currentTime = glutGet(GLenum(GLUT_ELAPSED_TIME));
	dtime = ((double)(currentTime - lastIdleTime)) / 1000;

	for(	vector<Ship>::iterator iter = Space::ships.begin();
			iter != Space:: ships.end();
			iter++ ) {

		iter->moveTime( dtime );
	//	cerr << iter->pos;
	}

	for(	vector<Asteroid>::iterator iter = Space::asteroids.begin();
			iter != Space:: asteroids.end();
			iter++ ) {

		iter->moveTime( dtime );
	//	cerr << iter->pos;
	}

	for(	vector<Shot>::iterator iter = Space::shots.begin();
			iter != Space:: shots.end();
			iter++ ) {

		iter->moveTime( dtime );
	//	cerr << iter->pos;
	}

	Space::checkShipPlanetCollisions();
	Space::checkShipAsteroidCollisions();
	Space::checkShotAsteroidCollisions();
	Space::checkAsteroidPlanetCollisions();
	Space::removeDeadShots();

	if( Flags::particles_enabled )
		Space::updateParticles( dtime );

	lastIdleTime = currentTime;
	glutPostRedisplay();
}

void init( int argc, char* argv[] ) {
	Flags::init( argc, argv );
	input = new InputCore();
	VideoCore::init();
	Space::init();
	srand( glutGet(GLenum(GLUT_ELAPSED_TIME)) );
	Space::initStars();
}

void loadDefaultUniverse() {
	Space::addPlanet(	Planet(	0, 0, 0.6*SPACE_RAD*SHIP_RAD,
								10.0 * SHIP_RAD, 100.0 * SHIP_MASS,
								0.5, 0.7, 0.6 )
					);

	Space::addPlanet(	Planet(	0.39*SPACE_RAD, 0, 0,
								4 * SHIP_RAD, 300 * SHIP_MASS,
								0.8, 0.6, 0.6 )
					);
		
	Space::addPlanet( 	Planet(	-0.4*SPACE_RAD, 
								-0.1*SPACE_RAD, 
								-0.1*SPACE_RAD,
								8.0*SHIP_RAD, 500 * SHIP_MASS,
								0.5, 0.5, 0.7 )
					);

	Space::addPlanet( 	Planet(	-0.1*SPACE_RAD, 
								-0.85*SPACE_RAD, 
								-0.45*SPACE_RAD,
								0.1*SPACE_RAD, 500 * SHIP_MASS,
								0.5, 0.3, 0.7 )
					);

	Asteroid a( ROID_LARGE, 0, 0, 0.3*SPACE_RAD );
	a.vel = Vector3( 0, 0, ROID_SPEED_LARGE );
	Space::addAsteroid(	a );


	Space::addAsteroid(	Asteroid( ROID_LARGE, 0, 0.7*SPACE_RAD, 0) );

	Space::addAsteroid(	Asteroid( ROID_LARGE, 0, 0.9*SPACE_RAD, 0.9*SPACE_RAD) );
	
	Space::addAsteroid(	Asteroid( ROID_LARGE, 0.7*SPACE_RAD, 0, -0.3*SPACE_RAD) );

	Space::addAsteroid( Asteroid( ROID_LARGE, -0.3*SPACE_RAD, 0.8*SPACE_RAD, 0.44*SPACE_RAD ) );
	
	Space::addAsteroid( Asteroid( ROID_LARGE, 0.3*SPACE_RAD, -0.8*SPACE_RAD, 0.1*SPACE_RAD ) );
}

int main( int argc, char* argv[] ) {

	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
	glutCreateWindow("Gravity Wars");
	glutReshapeWindow(600, 450);
	
	// if i figure out how to prevent the mouse from escaping
	//glutSetCursor( GLUT_CURSOR_NONE );

	glutDisplayFunc( display );
	glutReshapeFunc( reshape );
	
	glutKeyboardFunc( keyboard );
	glutKeyboardUpFunc( keyboardup );

	glutMouseFunc( mouse );
	glutMotionFunc( motion );
	glutPassiveMotionFunc( motion );
	
	glutSpecialUpFunc( specialkeyup );
	glutSpecialFunc( specialkey );
	glutIdleFunc( idle );

	init( argc, argv );

	glEnable(GL_DEPTH_TEST);
	glDepthFunc( GL_LEQUAL );

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
//	glEnable( GL_BLEND );

	glLightfv( GL_LIGHT0, GL_AMBIENT, ambient );
	glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
	glMaterialfv( GL_FRONT, GL_SPECULAR, spec_color );
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
	
	glEnable( GL_LIGHT0 );

	glEnable( GL_COLOR_MATERIAL );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective(	45,
					glutGet(GLenum(GLUT_WINDOW_WIDTH)) /
					glutGet(GLenum(GLUT_WINDOW_HEIGHT)),
					0.5,
					(Space::max(0) - Space::min(0)) );
	
	glClearColor( 0.0, 0.0, 0.0, 1.0 );

	loadDefaultUniverse();


	//cerr << "Entering main loop." << endl;

// aotnsehunsaoteuhsaonhteusnaoteuhsaonhteu
/*
	int index = 500;
	glNewList( index, GL_COMPILE );
	glPushMatrix();
	glutSolidCube( SHIP_RAD * 2 );
	glPopMatrix();
	glEndList();
	Space::ships[0].glIndex = 500;
*/
// aotnsehutnsoaheutnsohaeu

	glutMainLoop();
	return(0);
}
