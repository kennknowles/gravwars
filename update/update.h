#ifndef UPDATE_H
#define UPDATE_H

#include <iostream>
#include <vector>
#include <GL/gl.h>

#include "../include/matrix.h"

namespace gravwars {

const double PI = 3.14159;

double randRange( double min, double max );

typedef Vector3 Vertex3;

class Entity {
	public:
		Entity( double x=0, double y=0, double z=0 );
		
		Vertex3 pos;
		int glIndex;
};

class DirectedEntity : public Entity {
	public:
		Vector3 forwards;
		Vector3 up;
		Vector3 left; //positive x
		
		// rotates the direction vectors and returns the 
		// matrix (for extensions)
		Matrix33 rotate( double theta, const Vector3& axis );
};

class Particle : public Entity {
	public:
		Particle(	double x=0, double y=0, double z=0,
					double dx=0, double dy=0, double dz=0,
					double intensity=1, double maxIntense=1);

		Vector3 vel;
		double age;
		double curIntensity, maxIntensity;
};

class Planet : public Entity {
	public:
		Planet(	double x=0, double y=0, double z=0,
				double rad=0, double m=0,
				double red=0, double green=0, double blue=0);
		double radius;
		double mass;
		float r, g, b;
};

enum AsteroidSize {
	ROID_LARGE,
	ROID_MEDIUM,
	ROID_SMALL
};

class Asteroid : public DirectedEntity {
	public:
		Asteroid( AsteroidSize s, double x = 0, double y=0, double z=0 ); 
	
		void moveTime( double dtime );
		void spawnSmaller( vector<Asteroid>& v );
		void die();

		AsteroidSize size;
		double radius;
		double mass;
		float r, g, b;
		Vector3 vel;

		double angle;
		double rotateRate;
		Vector3 axis;
};

class Shot : public DirectedEntity {
	public:
		Shot( const DirectedEntity& source );
	
		double mass;
		double age;
		Vector3 vel;
		double radius;
		void moveTime( double dtime );
};

class Ship : public DirectedEntity {
	private:

		Vector3 rotSensitivity;
		Vector3 accel, vel, gravaccel;
		Vector3 rotateRate; // corresponds to pitch, yaw, and roll in that order

		bool	thrustingLeft, thrustingRight, thrustingForwards, thrustingBack,
				turningLeft, turningRight, rollingLeft, rollingRight;

		double thrustPower;
		double coolDown;		

	public:
		Ship();

		bool dead;
		double deadTime;
		double radius;
		double mass;

		void die();
		void respawn();

		void fire();
		void thrustForwards();
		void thrustForwardsEnd();
		void thrustRight();
		void thrustRightEnd();
		void thrustLeft();
		void thrustLeftEnd();
		void thrustUp();
		void thrustUpEnd();
		void thrustDown();
		void thrustDownEnd();
		void pitchUp();
		void pitchUpEnd();
		void pitchDown();
		void pitchDownEnd();
		void turnLeft();
		void turnLeftEnd();
		void turnRight();
		void turnRightEnd();
		void rollLeft();
		void rollLeftEnd();
		void rollRight();
		void rollRightEnd();

		void moveTime( double dtime );
};

// singleton class
class Space {
	public:
		static vector<Ship> ships;
		static vector<Planet> planets;
		static vector<Asteroid> asteroids;
		static vector<Shot> shots;
		static vector<Particle> particles;
		static vector<Particle> stars;

		static int numParticles;
		static int maxParticles;

		static int numShots;

		static void recreateGraphics();

		static Vector3 max; //bounding planes of space
		static Vector3 min;

		static void addPlanet(	const Planet& p );
		static void addAsteroid( const Asteroid& a );
		static void addShot(	const Shot& s );

		static void addParticle( const Particle& p );
		
		static void removeDeadShots();

		static void checkShipPlanetCollisions();
		static void checkShipAsteroidCollisions();
		static void checkShotPlanetCollisions();
		static void checkShotAsteroidCollisions();
		static void checkAsteroidPlanetCollisions();

		static void prepForParticles( int num );

		static void particleDist(	int num,
									double maxIntensity, double minIntensity,
									Vector3 maxPos, Vector3 minPos,
									Vector3 maxVel, Vector3 minVel
									);

		static void particleSphere(	int num,
									double maxIntensity, double minIntensity,
									Vector3 pos, double radius,
									double  maxVel, double minVel
									);

		static void updateParticles( double dtime );

		static void initStars();

		static void init();
		static void destroy();
};


} // namespace

#endif
