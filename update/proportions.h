#ifndef PROPORTIONS_H
#define PROPORTIONS_H

#include "../include/update.h"

namespace gravwars {

const double TOLERANCE = 0.00001;

// RAD = RADIUS
const double SHIP_RAD = 10.0;

// all terms are ratios of the ships size
const double ROID_RAD_LARGE = 5.0 * SHIP_RAD;
const double ROID_RAD_MEDIUM = 3.0 * SHIP_RAD;
const double ROID_RAD_SMALL = 1.0 * SHIP_RAD;

const double SHOT_RAD = 0.05 * SHIP_RAD;

const double SPACE_RAD = 50.0 * SHIP_RAD;

const double CROSSHAIR_INNER_RAD = 0.001 * SHIP_RAD;
const double CROSSHAIR_OUTER_RAD = 0.12 * SHIP_RAD;


// MASS
const double SHIP_MASS = 10.0;

const double ROID_MASS_LARGE = 2.0 * SHIP_MASS;
const double ROID_MASS_MEDIUM = 1.0 * SHIP_MASS;
const double ROID_MASS_SMALL = 0.5 * SHIP_MASS;

const double SHOT_MASS = 0.1 * SHIP_MASS;

// SPEED, relative to ships size, per second
const double SHIP_THRUST = 15.0 * SHIP_RAD;

const double SHOT_SPEED = 20.0 * SHIP_RAD;

const double ROID_SPEED_LARGE = 2.0 * SHIP_RAD;
const double ROID_SPEED_MEDIUM = 5.0 * SHIP_RAD;
const double ROID_SPEED_SMALL = 10.0 * SHIP_RAD;

// MISC, relative to nothing
const double G = 6.67e-11;
const double myG = 10.0;
const double SHOT_LIFESPAN = 2.0;
const double COOLDOWN_TIME = 0.3;
const double DEATH_TIME = 0.7;

const int ROID_LARGE_PARTICLES = 300;
const int ROID_MEDIUM_PARTICLES = 200;
const int ROID_SMALL_PARTICLES = 400;

const int MAX_PARTICLES = 1500;
const int MAX_PARTICLES_LOW = 700;
const int MAX_SHOTS = 20;

const int ROID_LARGE_PARTICLES_LOW = 100;
const int ROID_MEDIUM_PARTICLES_LOW = 60;
const int ROID_SMALL_PARTICLES_LOW = 120;

const int ROID_SPLIT_LARGE = 2;
const int ROID_SPLIT_MEDIUM = 2;

const int CROSSHAIR_SIDES = 5;
const int CROSSHAIR_RINGS = 20;

const int CROSSHAIR_SIDES_LOW = 5;
const int CROSSHAIR_RINGS_LOW = 10;

const double CROSSHAIR_DIST = 1.0 * SHIP_RAD;

// the constant in front is the number of particles per ship length
const double SHOT_PART_RATE = 60 * SHOT_SPEED / SHIP_RAD / SHIP_RAD;
const double SHOT_PART_RATE_LOW = 20 * SHOT_SPEED / SHIP_RAD / SHIP_RAD;

const int SHIP_PARTICLES = 500;
const int SHIP_PARTICLES_LOW = 150;

const double PART_DECAY_RATE = 0.9;

const int NUM_STARS = 1000;
const int NUM_STARS_LOW = 200;
const double STAR_MIN_INTENSITY = 0.7;
const double STAR_MAX_INTENSITY = 1.0;

// ANGULAR CONSTANTS
const double ROID_ROTATERATE_MAX = 0.5 * PI;
const double ROID_ROTATERATE_MIN = PI;

const double SHIP_ROTATE_SENSITIVITY = PI/3;

}

#endif
