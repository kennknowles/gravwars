#include <iostream>

#include "../include/input.h"
#include "../include/control.h"

using namespace gravwars;

int main( int argc, char* argv[] ) {
	InputCore input;
	unsigned char key;
	Control c;
	bool done = false;

	while( !done ) {
		cin >> key;
		c = input.getControl( InputType(key) );
		cout << controlToString( c );

		if( c == QUIT )
			done = true;
	}

	return( 0 );
}
