#ifndef FLAGS_H
#define FLAGS_H

// another singleton class

namespace gravwars {

class Flags {
	public:
		static void init( int argv, char* argv[] );
		static void destroy();

		static bool gravity_enabled;
		static bool particles_enabled;
		static bool low_quality;
		static bool stars_enabled;
		static bool qwerty_enabled;
};


}; // namespace

#endif

