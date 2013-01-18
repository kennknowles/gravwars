
#include <iostream>
#include <string>

#include "../include/control.h"
#include "../include/input.h"
#include "../include/flags.h"

using namespace gravwars;

InputCore::InputCore() :
	invertY( true ) 
{
	if( Flags::qwerty_enabled )
		keymap = default_qwertymap();
	else
		keymap = default_dvorakmap();
}

void InputCore::rebindKeys() {
	if( Flags::qwerty_enabled )
		keymap = default_qwertymap();
	else
		keymap = default_dvorakmap();

}

void InputCore::enqueueKey( InputType key ) {
	Control c = getControl( key );
	if( c != NONE )
		inputQ.push( c );
}

Control InputCore::dequeueControl() {
	
	if( inputQ.empty() )
		return( NONE );
	else {
		Control c = inputQ.front();	
		inputQ.pop();
		return( c );
	}
}

Control InputCore::getControl( InputType key ) {
	InputKeyMap::const_iterator iter = keymap.find( key );
	if( iter == keymap.end() )
		return( NONE );
	else
		return( iter->second );
}

void InputCore::bindControl( InputType key, Control c ) {
	keymap[key] = c;
}

InputKeyMap InputCore::default_dvorakmap() {
	InputKeyMap dmap;

	dmap[KEY_NONE] = NONE;
	dmap[KEY_a] = FIRE;
	dmap[KEY_o] = TURN_LEFT;
	dmap[KEY_u] = TURN_RIGHT;
	dmap[KEY_PERIOD] = PITCH_DOWN;
	dmap[KEY_e] = PITCH_UP;
	dmap[KEY_COMMA] = ROLL_LEFT;
	dmap[KEY_p] = ROLL_RIGHT;
	dmap[KEY_SPACE] = THRUST_FORWARDS;
	dmap[KEY_UP] = THRUST_UP;
	dmap[KEY_DOWN] = THRUST_DOWN;
	dmap[KEY_LEFT] = THRUST_LEFT;
	dmap[KEY_RIGHT] = THRUST_RIGHT;
	dmap[KEY_g] = TOGGLE_GRAVITY;
	dmap[KEY_x] = TOGGLE_PARTICLES;
	dmap[KEY_i] = TOGGLE_QWERTY;
	dmap[KEY_b] = TOGGLE_STARS;
	dmap[KEY_v] = TOGGLE_QUALITY;
	dmap[KEY_q] = QUIT;

	return( dmap );
}

InputKeyMap InputCore::default_qwertymap() {
	InputKeyMap qmap;

	qmap[KEY_NONE] = NONE;
	qmap[KEY_a] = FIRE;
	qmap[KEY_s] = TURN_LEFT;
	qmap[KEY_f] = TURN_RIGHT;
	qmap[KEY_e] = PITCH_DOWN;
	qmap[KEY_d] = PITCH_UP;
	qmap[KEY_w] = ROLL_LEFT;
	qmap[KEY_r] = ROLL_RIGHT;
	qmap[KEY_SPACE] = THRUST_FORWARDS;
	qmap[KEY_UP] = THRUST_UP;
	qmap[KEY_DOWN] = THRUST_DOWN;
	qmap[KEY_LEFT] = THRUST_LEFT;
	qmap[KEY_RIGHT] = THRUST_RIGHT;
	qmap[KEY_g] = TOGGLE_GRAVITY;
	qmap[KEY_x] = TOGGLE_PARTICLES;
	qmap[KEY_i] = TOGGLE_QWERTY;
	qmap[KEY_b] = TOGGLE_STARS;
	qmap[KEY_v] = TOGGLE_QUALITY;
	qmap[KEY_q] = QUIT;

	return( qmap );
}



ostream& gravwars::operator << ( ostream& os, const InputCore& inputcore ) {

	for( InputKeyMap::const_iterator iter = inputcore.keymap.begin();
			iter != inputcore.keymap.end();
			iter++ ) {
		os << inputTypeToString(iter->first) << " -> " << controlToString(iter->second) << endl;
	}

	return( os );

}

istream& gravwars::operator >> ( istream& is, InputCore& intputcore ) {
	return( is );
}

