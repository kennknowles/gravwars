#include "../include/flags.h"

using namespace gravwars;

bool Flags::gravity_enabled;
bool Flags::particles_enabled;
bool Flags::low_quality;
bool Flags::qwerty_enabled;
bool Flags::stars_enabled;

void Flags::init( int argc, char* argv[]) {
	// first load defaults
	gravity_enabled = false;
	particles_enabled = true;
	low_quality = true;
	qwerty_enabled = false;
	stars_enabled = true;

	// check command lines options
	for( int i = 1; i < argc; i++ ) {
		switch( argv[i][0] ) {
		case '+':
			switch( argv[i][1] ) {
			case 'g':
				gravity_enabled = true;
				break;
			case 'p':
				particles_enabled = true;
				break;
			case 'l':
				low_quality = true;
				break;
			case 'q':
				qwerty_enabled = true;
				break;
			case 's':
				stars_enabled = true;
				break;
			}
			break;

		case '-':
			switch( argv[i][1] ) {
			case 'g':
				gravity_enabled = false; 
				break;
			case 'p':
				particles_enabled = false;
				break;
			case 'l':
				low_quality = false;
				break;
			case 'q':
				qwerty_enabled = false;
				break;
			case 's':
				stars_enabled = false;
				break;
			}
			break;
		}
	}
	
}

void Flags::destroy() {

}
