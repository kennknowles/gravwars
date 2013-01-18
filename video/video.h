#ifndef VIDEO_H
#define VIDEO_H

#include <list>
#include <vector>
#include <GL/gl.h>

#include "../include/update.h"

namespace gravwars {

typedef unsigned char byte;

class PngTexture {
	public:
		PngTexture( char* filename );
		~PngTexture();
		
		int width, height;
		int channels;
		byte *data;
};

class ObjFace : public vector<unsigned int> {
	public:
		unsigned int material;
};

ostream& operator << ( ostream& os, const ObjFace& f );

typedef list<ObjFace> ObjFaceList;

class ObjModel {
	public:
		ObjModel( char* filename );
		vector<Vertex3> vertices;
		ObjFaceList faces;
		double max;
};

ostream& operator << ( ostream& os, const ObjModel& o );


// singleton renderer
class VideoCore {
	private:
		static int glIndexCounter;
		static vector<PngTexture> textures;

	public:
		static void init();
		static void clearLists();

		static int shotList( const Shot& s );
		static int planetList( const Planet& p ); //returns gl display list index
		static int asteroidList( const Asteroid& a );
		static int shipList( const Ship& s );
		static int objList( const ObjModel& o, double size );
};

} // namespace

#endif
