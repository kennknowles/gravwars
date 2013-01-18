#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <map>
#include <functional>
#include <string>
#include <queue>

#include "control.h"

namespace gravwars {

// InputKeyMap : unsigned char -> Control
typedef map<	InputType,
				Control,
				less< InputType > > 
		InputKeyMap;

typedef map< Control, ControlState, less<Control> > ControlStateMap;

typedef queue< Control > ControlQueue;

class InputCore {
	private:
		InputKeyMap keymap;
		ControlStateMap state;
		bool invertY;
		ControlQueue inputQ;

	public:
		InputCore();

		void rebindKeys();

		void enqueueKey( InputType key );
		Control dequeueControl();
		Control getControl( InputType input );
		void bindControl( InputType key, Control c);
		InputKeyMap default_dvorakmap();
		InputKeyMap default_qwertymap();

		friend istream& operator >> ( istream& is, InputCore& inputcore );
		friend ostream& operator << ( ostream& os, const InputCore& inputcore );
};

istream& operator >> ( istream& is, InputCore& inputcore );
ostream& operator << ( ostream& os, const InputCore& inputcore );

} //namespace

#endif
