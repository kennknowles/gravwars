#include <cmath>
#include <math.h>
#include <vector>
#include <iostream>
#include <GL/gl.h>

#include "../include/update.h"
#include "../include/matrix.h"
#include "../include/flags.h"
#include "../include/proportions.h"
#include "../include/video.h"

using namespace gravwars;

double gravwars::randRange( double min, double max ) {
	return( min +
			double(rand()) / RAND_MAX *
			( max - min )
		);
}

Entity::Entity( double x=0, double y=0, double z=0 ) :
	pos( x, y, z ) 
{
}

Matrix33 DirectedEntity::rotate( double theta, const Vector3& axis ) {
	Matrix33 m;

	m = Matrix33::rotate( theta, axis );

	forwards = m * forwards;
	up = m * up;
	left = m * left;

	return( m );
}
		
Particle::Particle(	double x=0, double y=0, double z=0,
					double dx=0, double dy=0, double dz=0,
					double intensity= 1, double maxIntense=1) :
																Entity( x, y, z ),
																vel( dx, dy, dz ),
																curIntensity( intensity ),
																maxIntensity( maxIntense )
{
}

Planet::Planet( double x=0, double y=0, double z=0,
				double rad=0, double m=0,
				double red=0, double green=0, double blue=0 ) :
																Entity( x, y, z ),
																radius( rad ),
																mass( m ),
																r( red ),
																g( green ),
																b( blue )
{
	glIndex = VideoCore::planetList( *this );
}
	
	

Asteroid::Asteroid( AsteroidSize s, double x=0, double y=0, double z=0 ) :
																			size( s )
{
	pos(0) = x;
	pos(1) = y;
	pos(2) = z;
	
	forwards(0) = rand() - RAND_MAX/2;
	forwards(1) = rand() - RAND_MAX/2;
	forwards(2) = rand() - RAND_MAX/2;
	forwards.normalize();
	//cerr << forwards << endl;
	switch( size ) {
	case ROID_LARGE:
		radius = ROID_RAD_LARGE;
		mass = ROID_MASS_LARGE;
		vel = ROID_SPEED_LARGE * forwards;
		break;
	case ROID_MEDIUM:
		radius = ROID_RAD_MEDIUM; 
		mass = ROID_MASS_MEDIUM;
		vel = ROID_SPEED_MEDIUM * forwards;
		break;
	case ROID_SMALL:
		radius = ROID_RAD_SMALL;
		mass = ROID_MASS_SMALL;
		vel = ROID_SPEED_SMALL * forwards;
		break;
	}

	rotateRate = randRange( ROID_ROTATERATE_MIN, ROID_ROTATERATE_MAX );
	angle = 0;
	axis(0) = rand() - RAND_MAX/2;
	axis(1) = rand() - RAND_MAX/2;
	axis(2) = rand() - RAND_MAX/2;
	axis.normalize();
	r = g = b = randRange( 0.2, 0.4 );
	glIndex = VideoCore::asteroidList( *this );
}

void Asteroid::moveTime( double dtime ) {
	if( Flags::gravity_enabled ) {
		Vector3 gravaccel = 0;
		Vector3 gravV;
		double gravD;

		for(	vector<Planet>::const_iterator iter = Space::planets.begin();
				iter != Space::planets.end();
				iter++ ) {
	
			gravV = iter->pos - pos; // vector from here to planet
			if( gravV*gravV > TOLERANCE ) {
				gravD = myG * mass * iter->mass / ( gravV * gravV ); // newtons law
	
				gravV.normalize();
				gravaccel += gravD * gravV;
				//cerr << gravaccel << endl;
			}
		}
	}
	
	if( (abs(vel(0) - 0) > TOLERANCE)
		|	(abs(vel(1) - 0) > TOLERANCE)
		|	(abs(vel(2) - 0) > TOLERANCE)	)

	{

		pos += dtime * vel;

		for( int i = 0; i < 3; i++ ) {
			
			if( pos(i) > Space::max(i) )
				pos(i) = Space::min(i) + (pos(i) - Space::max(i));
			else if( pos(i) < Space::min(i) )
				pos(i) = Space::max(i) - (Space::min(i) - pos(i));
		}
	}

	if( abs(rotateRate - 0) > 0 )
		angle += dtime * rotateRate;

//	cerr << angle << endl;

}

void Asteroid::die() {
	if( Flags::particles_enabled ) {
		int part_count;
		switch( size ) {
		case ROID_LARGE:
			if( Flags::low_quality )
				part_count = ROID_LARGE_PARTICLES;
			else
				part_count = ROID_LARGE_PARTICLES_LOW;
			break;
		case ROID_MEDIUM:
			if( Flags::low_quality )
				part_count = ROID_MEDIUM_PARTICLES;
			else
				part_count = ROID_MEDIUM_PARTICLES_LOW;
			break;
		case ROID_SMALL:
			if( Flags::low_quality )
				part_count = ROID_SMALL_PARTICLES;
			else
				part_count = ROID_SMALL_PARTICLES_LOW;
			break;
		}		
	
		Space::particleSphere(	part_count,
								1.0, 1.0,
								pos, 0,
								5, 1.0);
	}
}

Shot::Shot( const DirectedEntity& source ) {

	mass = SHOT_MASS;
	pos = source.pos;
	forwards = source.forwards;
	vel = SHOT_SPEED * forwards;
	radius = SHOT_RAD;
	age = 0;
	glIndex = VideoCore::shotList( *this );
}

void Shot::moveTime( double dtime ) {
			
	if( Flags::gravity_enabled ) {
		Vector3 gravaccel = 0;
		Vector3 gravV;
		double gravD;

		for(	vector<Planet>::const_iterator iter = Space::planets.begin();
				iter != Space::planets.end();
				iter++ ) {
	
			gravV = iter->pos - pos; // vector from here to planet
			if( gravV*gravV > TOLERANCE ) {
				gravD = myG * mass * iter->mass / ( gravV * gravV ); // newtons law
	
				gravV.normalize();
				gravaccel += gravD * gravV;
				//cerr << gravaccel << endl;
			}
		}
	}

	if( (abs(vel(0) - 0) > TOLERANCE)
		|	(abs(vel(1) - 0) > TOLERANCE)
		|	(abs(vel(2) - 0) > TOLERANCE)	)

	{
		Vector3 maxPos( pos );
		Vector3 minPos( pos );
		for( int i = 0; i < 3; i++ ) {
			maxPos(i) += radius / 1.4;
			minPos(i) -= radius / 1.4;
		}
		
		if( Flags::particles_enabled ) {
			double part_rate;
			if( Flags::low_quality )
				part_rate = SHOT_PART_RATE_LOW;
			else
				part_rate = SHOT_PART_RATE;

			Vector3 independentVel = vel - (SHOT_SPEED * forwards);

		//	cerr << independentVel << " aoeu " << vel << endl;

			Space::particleDist(	dtime * part_rate,
									1.0, 1.0,
									maxPos, minPos, independentVel, independentVel
								);
		}
		pos += dtime * vel;
		age += dtime;

		for( int i = 0; i < 3; i++ ) {
			
			if( pos(i) > Space::max(i) )
				pos(i) = Space::min(i) + (pos(i) - Space::max(i));
			else if( pos(i) < Space::min(i) )
				pos(i) = Space::max(i) - (Space::min(i) - pos(i));

		}
	}

}

Ship::Ship() {
	thrustPower = SHIP_THRUST; 

	mass = SHIP_MASS;
	radius = SHIP_RAD;
	dead = false;

	forwards(0) = 0;
	forwards(1) = 0;
	forwards(2) = 1;
	up(0) = 0;
	up(1) = 1;
	up(2) = 0;
	left(0) = 1;
	left(1) = 0;
	left(2) = 0;
	rotateRate(0) = 0;
	rotateRate(1) = 0;
	rotateRate(2) = 0;
	rotSensitivity(0) = SHIP_ROTATE_SENSITIVITY;  
	rotSensitivity(1) = SHIP_ROTATE_SENSITIVITY;
	rotSensitivity(2) = SHIP_ROTATE_SENSITIVITY;
	accel(0) = accel(1) = accel(2) = 0;
	gravaccel = accel;
	vel(0) = vel(1) = vel(2) = 0;
	
	glIndex = VideoCore::shipList( *this );//VideoCore::objList( "650.obj", radius );
	cerr << "done initializing ship\n";

};

void Ship::die() {
	if( Flags::particles_enabled )
		Space::particleSphere(	SHIP_PARTICLES,
								1.0, 1.0,
								pos, 0,
								5, 1.0);
	deadTime = 0;
	dead = true;
//	cerr << "death pos: " << pos << endl;
}

void Ship::respawn() {
	vel = accel = pos = Vector3( 0, 0, 0 );
	forwards(0) = 0;
	forwards(1) = 0;
	forwards(2) = 1;
	up(0) = 0;
	up(1) = 1;
	up(2) = 0;
	left(0) = 1;
	left(1) = 0;
	left(2) = 0;
	dead = false;
}

void Ship::fire() {
	if( (coolDown <= 0) &&
		(Space::numShots < MAX_SHOTS) ) {

		Shot s(*this);
		s.vel += vel;
		Space::addShot( s );
		coolDown = COOLDOWN_TIME;
	}
}

void Ship::thrustForwards() {
	accel += thrustPower * forwards; //vel = 100;
}

void Ship::thrustForwardsEnd() {
	accel -= thrustPower * forwards; //vel = 0;
}

void Ship::thrustLeft() {
	accel += thrustPower * left; //vel = 100;
}

void Ship::thrustLeftEnd() {
	accel -= thrustPower * left; //vel = 0;
}

void Ship::thrustRight() {
	accel -= thrustPower * left; //vel = 100;
}

void Ship::thrustRightEnd() {
	accel += thrustPower * left; //vel = 0;
}

void Ship::thrustDown() {
	accel -= thrustPower * up; //vel = 100;
}

void Ship::thrustDownEnd() {
	accel += thrustPower * up; //vel = 0;
}

void Ship::thrustUp() {
	accel += thrustPower * up;
}

void Ship::thrustUpEnd() {
	accel -= thrustPower * up;
}


void Ship::pitchUp() {
	rotateRate(0) -= rotSensitivity(0);
}

void Ship::pitchUpEnd() {
	rotateRate(0) += rotSensitivity(0);
}


void Ship::pitchDown() {
	rotateRate(0) += rotSensitivity(0);
}

void Ship::pitchDownEnd() {
	rotateRate(0) -= rotSensitivity(0);
}


void Ship::turnLeft() {
	rotateRate(1) += rotSensitivity(1);
}

void Ship::turnLeftEnd() {
	rotateRate(1) -= rotSensitivity(1);
}


void Ship::turnRight() {
	rotateRate(1) -= rotSensitivity(1);
}

void Ship::turnRightEnd() {
	rotateRate(1) += rotSensitivity(1);
}

void Ship::rollLeft() {
	rotateRate(2) -= rotSensitivity(2);
}

void Ship::rollLeftEnd() {
	rotateRate(2) += rotSensitivity(2);
}


void Ship::rollRight() {
	rotateRate(2) += rotSensitivity(2);
}

void Ship::rollRightEnd() {
	rotateRate(2) -= rotSensitivity(2);
}


void Ship::moveTime( double dtime ) {
	bool rotateFlag=false;

	if( dead ) {
		deadTime += dtime;
		if( deadTime > DEATH_TIME ) {
			respawn();
		}
		return;
	}

	//aoe uhtnsao eutnaoheuntsaoheutnshoaeu
	gravaccel = 0;

	Vector3 gravV;
	double gravD;

	if( Flags::gravity_enabled ) {
		for(	vector<Planet>::const_iterator iter = Space::planets.begin();
				iter != Space::planets.end();
				iter++ ) {
	
			gravV = iter->pos - pos; // vector from here to planet
			if( gravV*gravV > TOLERANCE ) {
				gravD = myG * mass * iter->mass / ( gravV * gravV ); // newtons law
	
				gravV.normalize();
				gravaccel += gravD * gravV;
				//cerr << gravaccel << endl;
			}
		}
	}

	//=tnsaoheunsaohteutnsaoheutnshaoeu
	if( (abs(accel(0) - 0) > TOLERANCE)
		|	(abs(accel(1) - 0) > TOLERANCE)
		|	(abs(accel(2) - 0) > TOLERANCE)	)
		
	{
//		cerr << ':';
		
		vel += dtime * accel;
	}

	if( (abs(gravaccel(0) - 0) > TOLERANCE)
		|	(abs(gravaccel(1) - 0) > TOLERANCE)
		|	(abs(gravaccel(2) - 0) > TOLERANCE)	)
		
	{
//		cerr << ':';
		
		vel += dtime * gravaccel;
	}

	if( (abs(vel(0) - 0) > TOLERANCE)
		|	(abs(vel(1) - 0) > TOLERANCE)
		|	(abs(vel(2) - 0) > TOLERANCE)	)

	{
//		cerr << '.';

		pos += dtime * vel;

		for( int i = 0; i < 3; i++ ) {
			
			if( pos(i) > Space::max(i) )
				pos(i) = Space::min(i) + (pos(i) - Space::max(i));
			else if( pos(i) < Space::min(i) )
				pos(i) = Space::max(i) - (Space::min(i) - pos(i));
		}
	}

	for( int i = 0; i<3; i++ )
		if( abs(rotateRate(i) - 0) > TOLERANCE ) {
//			cerr << ',';
			double dangle;
			Matrix33 m;

			dangle = dtime * rotateRate(i);
			
			switch( i ) {
			case 0:
				m = rotate( dangle, left );
				accel = m * accel;
				break;
			case 1:
				m = rotate( dangle, up );
				accel = m * accel;
				break;
			case 2:
				m = rotate( dangle, forwards );
			//	cerr << m << endl;
				accel = m * accel;
				break;
			}
	
	}

	if( coolDown > 0 )
		coolDown -= dtime;

	// check against planets for collisions
/*	for(	vector<Planet>::const_iterator iter = Space::planets.begin();
			iter != Space::planets.end();
			iter++ ) {

		double dist = iter->radius + radius;
		gravV = iter->pos - pos;
		if( gravV*gravV < dist*dist ) {
			cerr << "Crashed:\n";
			cerr << "Pos: " << pos << endl;
			cerr << "Planet: " << iter->pos << endl;
			cerr << "Radius: " << iter->radius << endl;
			
			accel = vel = pos = Vector3();
		}
	}*/

//	cerr << accel << endl;
//	cerr << "pos: " << pos << endl;
//	cerr << "up: " << up;// << endl;
//	cerr << "forwards:" << forwards;
//	cerr << "left:" << left;
//	cerr << "rotateRate: " << rotateRate <<endl;
//	cerr << "vel: " << vel << endl;
}

vector<Ship> Space::ships;
vector<Planet> Space::planets;
vector<Asteroid> Space::asteroids;
vector<Shot> Space::shots;
vector<Particle> Space::particles;
vector<Particle> Space::stars;
Vector3 Space::max;
Vector3 Space::min;
int Space::numParticles, Space::maxParticles, Space::numShots;

void Space::addPlanet(	const Planet& p ) {

	planets.push_back( p );
}

void Space::addAsteroid(	const Asteroid& a ) {


	asteroids.push_back( a );
}

void Space::addShot( const Shot& s ) {
	numShots++;
	shots.push_back( s );
}

void Space::removeDeadShots() {
	vector<Shot>::iterator iter;
	iter = shots.begin();
	
	while( iter != shots.end() ) {
		if( iter->age > SHOT_LIFESPAN ) {
			numShots--;
			iter = shots.erase( iter );
		}
		else
			iter++;
	}
}

// i BS the radius in order to BS tho hitscan w/ spherical hitscan
void Space::checkShipPlanetCollisions() {
	Ship &s = ships[0];
	Vector3 diff;
	double dist;

	for(	vector<Planet>::const_iterator iter = planets.begin();
			iter != planets.end();
			iter++ ) {

		diff = s.pos - iter->pos;
		dist = s.radius + iter->radius;
		if( ((diff * diff) < (dist * dist)) 
			&& (!s.dead) ) {
			s.die();
			cerr << "Ship collided with planet\n";
			
		}
	}
}

void Space::checkShipAsteroidCollisions() {
	Ship &s = Space::ships[0];

	Vector3 diff;
	double dist;

	for(	vector<Asteroid>::iterator iter = asteroids.begin();
			iter != asteroids.end();
			iter++ ) {

		diff = s.pos - iter->pos;
		dist = s.radius + iter->radius;
		if( ((diff * diff) < (dist * dist))
			&& (!s.dead) ) {
			s.die();
			cerr << "Ship collided with asteroid\n";
		}
	}
}

void Asteroid::spawnSmaller( vector<Asteroid>& v ) {
	switch( size ) {
	case ROID_LARGE:
		cerr << "Large asteroid destroyed\n";
		for( int i = 0; i<ROID_SPLIT_LARGE; i++) {
			v.push_back( Asteroid(	ROID_MEDIUM, 
									pos(0),
									pos(1),
									pos(2) ) );
		}
		break;
	case ROID_MEDIUM:
		cerr << "Medium asteroid destroyed\n";
		for( int i = 0; i<ROID_SPLIT_LARGE; i++) {
			v.push_back( Asteroid(	ROID_SMALL, 
									pos(0),
									pos(1),
									pos(2) ) );
		}
		break;
	}
}

void Space::checkShotAsteroidCollisions() {
	bool shotEliminated;
	Vector3 diff;
	double dist;
	AsteroidSize newSize;

	vector<Asteroid> newRoids;

	vector<Shot>::iterator shotiter = shots.begin();

	while( shotiter != shots.end() ) {
		shotEliminated = false;
	
		vector<Asteroid>::iterator roiditer = asteroids.begin();

		while( (roiditer != asteroids.end()) && !shotEliminated ) {
			diff = shotiter->pos - roiditer->pos;
			dist = shotiter->radius + roiditer->radius;
			if( (diff * diff) < (dist * dist) ) {
				cerr << "Shot hit asteroid\n";

				shotEliminated = true;
				roiditer->spawnSmaller( newRoids );	
				roiditer->die();
				
				roiditer = asteroids.erase( roiditer );
				shotiter = shots.erase( shotiter );
				numShots--;
			}
			else
				roiditer++;
		}

		if( !shotEliminated )
			shotiter++;
	}

	for(	vector<Asteroid>::const_iterator iter = newRoids.begin();
			iter != newRoids.end();
			iter++ ) {
		asteroids.push_back( *iter );
	}
			
}

void Space::checkShotPlanetCollisions() {

	vector<Shot>::iterator shotiter = shots.begin();
		
	while( shotiter != shots.end()) {
		bool shotEliminated = false;
		
		for(	vector<Planet>::const_iterator planetiter = planets.begin();
				(planetiter != planets.end()) && (!shotEliminated);
				planetiter++ ) {

			Vector3 diff = shotiter->pos - planetiter->pos;
			double dist = shotiter->radius + planetiter->radius;

			if( (diff * diff) < (dist * dist) ) {
				cerr << "Shot hit planet\n";
				shotEliminated = true;
				shotiter = shots.erase( shotiter );
				numShots--;
			}
		}
		if( !shotEliminated )
			shotiter++;

	}
}


void Space::checkAsteroidPlanetCollisions() {
	vector<Asteroid> newRoids;
	bool roidEliminated;
	Vector3 diff;
	double dist;

	vector<Asteroid>::iterator roiditer = asteroids.begin();
	
	while( roiditer != asteroids.end() ) {
		roidEliminated = false;

		vector<Planet>::iterator planetiter = planets.begin();
		
		while( (planetiter != planets.end()) && !roidEliminated ) {
			diff = planetiter->pos - roiditer->pos;
			dist = planetiter->radius + roiditer->radius;
			if( (diff * diff) < (dist * dist) ) {
				roidEliminated = true;
				cerr << "Asteroid hit planet" << endl;
				
				switch( roiditer->size ) {
				case ROID_LARGE:
					cerr << "Large asteroid destroyed\n";
					for( int i = 0; i<ROID_SPLIT_LARGE; i++) {
						Asteroid a(	ROID_MEDIUM, 
									roiditer->pos(0),
									roiditer->pos(1),
									roiditer->pos(2) );
						if( a.vel * roiditer->vel > 0 ) {
							a.vel = -1 * a.vel;
							a.forwards = -1 * a.forwards;
						}
						newRoids.push_back( a );
					}

					break;
				case ROID_MEDIUM:
					cerr << "Medium asteroid destroyed\n";
					for( int i = 0; i<ROID_SPLIT_MEDIUM; i++) {
						Asteroid a(	ROID_SMALL, 
									roiditer->pos(0),
									roiditer->pos(1),
									roiditer->pos(2) );
						if( a.vel * roiditer->vel > 0 ) {
							a.vel = -1 * a.vel;
							a.forwards = -1 * a.forwards;
						}
						newRoids.push_back( a );
					}
					break;
				} //switch
				roiditer->die();
				roiditer = asteroids.erase( roiditer );
			}
			planetiter++;
		} //while planet
		if( !roidEliminated )
			roiditer++;
	}
	
	// add new roids
	for(	vector<Asteroid>::const_iterator iter = newRoids.begin();
			iter != newRoids.end();
			iter++ ) {
	//	cerr << "new roid added\n";
		asteroids.push_back( *iter );
	}
}

void Space::recreateGraphics() {
	if( Flags::low_quality )
		maxParticles = MAX_PARTICLES_LOW;
	else
		maxParticles = MAX_PARTICLES;

	for(	vector<Planet>::iterator iter = planets.begin();
			iter != planets.end(); iter++ )
		iter->glIndex = VideoCore::planetList( *iter );
	
	for(	vector<Asteroid>::iterator iter = asteroids.begin();
			iter != asteroids.end(); iter++ )
		iter->glIndex = VideoCore::asteroidList( *iter );
		
	for(	vector<Shot>::iterator iter = shots.begin();
			iter != shots.end(); iter++ )
		iter->glIndex = VideoCore::shotList( *iter );

	for(	vector<Ship>::iterator iter = ships.begin();
			iter != ships.end(); iter++ )
		iter->glIndex = VideoCore::shipList( *iter );
}

void Space::prepForParticles( int num ) {
	if( num+numParticles > maxParticles ) {
		int diff = num+numParticles - maxParticles;
		vector<Particle>::iterator beginiter, enditer;
		beginiter = enditer = particles.begin();

		for( int i=1; i<diff; i++ )
			enditer++;

		particles.erase( beginiter, enditer );
		numParticles = maxParticles - num;
	}
}


void Space::particleDist(	int num,
							double maxIntensity, double minIntensity,
							Vector3 maxPos, Vector3 minPos,
							Vector3 maxVel, Vector3 minVel ) {

//	cerr	<< "creating " << num << " particles " << endl
//			<< "between " << maxPos << " and " << minPos << endl
//			<< "intensity between " << maxIntensity << " and " << minIntensity << endl
//			<< "vel between " << maxVel << " and " << minVel << endl;
//	cerr << "creating " << num << " particles\n";
	prepForParticles( num );

	for( int i = 0; i < num; i++ ) {
		double intensity = randRange( minIntensity, maxIntensity );

		Vector3 vel, pos;
		for( int i=0; i<3; i++ ) {
			vel(i) = randRange( minVel(i), maxVel(i) );
			pos(i) = randRange( minPos(i), maxPos(i) );
		}	
	//	cerr << "part: " << pos << ", v: " << vel << endl;
	
		Particle p(	pos(0), pos(1), pos(2),
					vel(0), vel(1), vel(2),
					intensity, maxIntensity );

		particles.push_back( p );
	}

}

void Space::particleSphere(	int num,
							double maxIntensity, double minIntensity,
							Vector3 pos, double radius,
							double maxVel, double minVel) {
	// clear room for the new particles
	prepForParticles( num );

							
//	cerr << "creating " << num << " particles in sphere\n";
	for( int i = 0; i < num; i++ ) {
		double intensity = randRange( minIntensity, maxIntensity );

		Vector3 dir, newpos, vel;
		for( int i = 0; i < 3; i++ )
			dir(i) = randRange( -1, 1 );
		dir.normalize();
		
		vel = SHIP_RAD * randRange( minVel, maxVel ) * dir;
		newpos = pos + randRange( 0, radius ) * dir;
//		cerr << "p: " << newpos << "v: " << vel << endl;
		Particle p(	newpos(0), newpos(1), newpos(2),
					vel(0), vel(1), vel(2),
					intensity, maxIntensity );

		particles.push_back( p );
		
	}

}

void Space::updateParticles( double dtime ) {
	vector<Particle>::iterator iter = particles.begin();
	
	// update intensity
	while( iter != particles.end() ) {
		iter->curIntensity -= dtime * PART_DECAY_RATE;
		if( iter->curIntensity > 0 ) {
//			cerr << "aoeu: " << dtime * iter->vel << endl;
//			cerr << "v: " << iter->vel
			iter->pos += dtime * iter->vel;
			iter++;
		}
		else {
			iter = particles.erase( iter );
		}
	}	
}

void Space::initStars() {
	int num_stars;
	if( Flags::low_quality )
		num_stars = NUM_STARS_LOW;
	else
		num_stars = NUM_STARS;

	stars.reserve( num_stars );

	for( int i = 0; i < num_stars; i++ ) {
		Particle p(		randRange( Space::min(0), Space::max(0) ),
						randRange( Space::min(1), Space::max(1) ),
						randRange( Space::min(2), Space::max(2) ),
						0, 0, 0,
						randRange( STAR_MIN_INTENSITY, STAR_MAX_INTENSITY ), 1.0 );

		stars.push_back( p );
		

	}

}

void Space::init() {
	Ship ship;

	max(0) = SPACE_RAD;
	max(1) = SPACE_RAD;
	max(2) = SPACE_RAD;

	min(0) = -SPACE_RAD;
	min(1) = -SPACE_RAD;
	min(2) = -SPACE_RAD;
	
	ship.pos(0) = 0;
	ship.pos(1) = 0;
	ship.pos(2) = 0;
	
	ships.push_back( ship );

	numParticles = 0;
	numShots = 0;
	
	if( Flags::low_quality )
		maxParticles = MAX_PARTICLES_LOW;
	else
		maxParticles = MAX_PARTICLES;

	particles.reserve(maxParticles);
	shots.reserve(MAX_SHOTS);

}

void Space::destroy() {
}

