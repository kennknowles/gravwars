#ifndef CONTROL_H
#define CONTROL_H

#include <string>

namespace gravwars {

enum InputType {
	KEY_PERIOD = '.',
	KEY_COMMA = ',',
	KEY_TAB = '\t',
	KEY_RETURN = '\n',
	KEY_SPACE = ' ',
	KEY_0 = '0',
	KEY_1 = '1',
	KEY_2 = '2',
	KEY_3 = '3',
	KEY_4 = '4',
	KEY_5 = '5',
	KEY_6 = '6',
	KEY_7 = '7',
	KEY_8 = '8',
	KEY_9 = '9',
	KEY_A = 'A',
	KEY_B = 'B',
	KEY_C = 'C',
	KEY_D = 'D',
	KEY_E = 'E',
	KEY_F = 'F',
	KEY_G = 'G',
	KEY_H = 'H',
	KEY_I = 'I',
	KEY_J = 'J',
	KEY_K = 'K',
	KEY_L = 'L',
	KEY_M = 'M',
	KEY_N = 'N',
	KEY_O = 'O',
	KEY_P = 'P',
	KEY_Q = 'Q',
	KEY_R = 'R',
	KEY_S = 'S',
	KEY_T = 'T',
	KEY_U = 'U',
	KEY_V = 'V',
	KEY_W = 'W',
	KEY_X = 'X',
	KEY_Y = 'Y',
	KEY_Z = 'Z',
	KEY_a = 'a',
	KEY_b = 'b',
	KEY_c = 'c',
	KEY_d = 'd',
	KEY_e = 'e',
	KEY_f = 'f',
	KEY_g = 'g',
	KEY_h = 'h',
	KEY_i = 'i',
	KEY_j = 'j',
	KEY_k = 'k',
	KEY_l = 'l',
	KEY_m = 'm',
	KEY_n = 'n',
	KEY_o = 'o',
	KEY_p = 'p',
	KEY_q = 'q',
	KEY_r = 'r',
	KEY_s = 's',
	KEY_t = 't',
	KEY_u = 'u',
	KEY_v = 'v',
	KEY_w = 'w',
	KEY_x = 'x',
	KEY_y = 'y',
	KEY_z = 'z',
	
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_NONE,
	
	MOUSE_1,
	MOUSE_2,
	MOUSE_3,
	MOUSE_4,
	MOUSE_5
};

enum Control {
	NONE,
	FIRE,
	TURN_LEFT,
	TURN_RIGHT,
	PITCH_UP,
	PITCH_DOWN,
	ROLL_LEFT,
	ROLL_RIGHT,
	THRUST_FORWARDS,
	THRUST_RIGHT,
	THRUST_LEFT,
	THRUST_UP,
	THRUST_DOWN,
	TOGGLE_GRAVITY,
	TOGGLE_PARTICLES,
	TOGGLE_QWERTY,
	TOGGLE_STARS,
	TOGGLE_QUALITY,
	QUIT
};

enum ControlState {
	ON, OFF, PRESS
};

inline InputType charToInputType( unsigned char c ) {
	if( 	( ('A' <= c) && ('Z' >= c ) )
		|	( ('a' <= c) && ('z' >= c ) )
		|	( ('0' <= c) && ('9' >= c ) )
		|	( c == '.' ) 
		|	( c == ' ' ) ) {

		return( InputType( c ) );
	}
}

inline string inputTypeToString( InputType i ) {
	string s = "";

	if( i <= KEY_z ) {
		s.resize( 1 );
		s[0] = char( i );
		return( s );
	}
	else {
		switch( i ) {
		case KEY_UP:
			return( "KEY_UP" );
		case KEY_DOWN:
			return( "KEY_DOWN" );
		case KEY_LEFT:
			return( "KEY_LEFT" );
		case KEY_RIGHT:
			return( "KEY_RIGHT" );
		default:
			return( string("") );
		}
	}
}

inline string controlToString( Control c ) {
	switch( c ) {
		case NONE:
			return("NONE");
		case FIRE:
			return("FIRE");
		case TURN_LEFT:
			return("TURN_LEFT");
		case TURN_RIGHT:
			return("TURN_RIGHT");
		case PITCH_UP:
			return("PITCH_UP");
		case PITCH_DOWN:
			return("PITCH_DOWN");
		case ROLL_LEFT:
			return("ROLL_LEFT");
		case ROLL_RIGHT:
			return("ROLL_RIGHT");
		case THRUST_FORWARDS:
			return("THRUST_FORWARDS");
		case THRUST_LEFT:
			return("THRUST_LEFT");
		case THRUST_RIGHT:
			return("THRUST_RIGHT");
		case THRUST_UP:
			return("THRUST_UP");
		case THRUST_DOWN:
			return("THRUST_DOWN");
		case TOGGLE_GRAVITY:
			return("TOGGLE_GRAVITY");
		case TOGGLE_PARTICLES:
			return("TOGGLE_PARTICLES");
		case QUIT:
			return("QUIT");
		default:
			return("UNKNOWN");
	}
}

} //namespace
#endif
