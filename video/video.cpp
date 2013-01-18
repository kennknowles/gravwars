#include <GL/gl.h>
#include <GL/glut.h>
#include <vector>
#include <png.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <cstring>

#include <strstream>

#include "../include/video.h"
#include "../include/update.h"
#include "../include/flags.h"

using namespace gravwars; 

//uses a lot of C instead of C++ to make libpng stuff easier
PngTexture::PngTexture( char* filename ) {
	int i;
	int bpc, color_type, zsize;
	FILE *infile;

	png_uint_32 x, y;
	png_structp png_ptr;
	png_infop info_ptr;
	png_byte **row_pointers;

	cerr << "reading png file: " << filename << endl;
	infile = fopen( filename, "rb" );

	cerr << "successfully opened file: " << filename << endl;

	if( !infile ) {
		cerr << "Could not open file: " << filename << endl;
		exit(1);
	}

	cerr << "creating png_read_struct\n";
	png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, 0, 0, 0 );
	if( !png_ptr ) {
		cerr << "Could not create png_read_struct" << endl;
		fclose(infile);
		exit(1);
	}
	cerr << "successfully created png_read_struct\n";

	cerr << "creating png_info_struct\n";
	info_ptr = png_create_info_struct( png_ptr );
	if( !info_ptr ) {
		png_destroy_read_struct( &png_ptr, (png_infopp)NULL, (png_infopp)NULL );
		cerr << "Could not create png_info_struct" << endl;
		fclose(infile);
		exit(1);
	}
	cerr << "succussfully created png_info_struct\n";

	cerr << "getting info...";
	png_read_info( png_ptr, info_ptr );
	cerr << "success\n";

	cerr << "getting stats..";
	png_get_IHDR( png_ptr, info_ptr, &x, &y, &bpc, &color_type, 0, 0, 0 );
	cerr << "success\n";

	if(	( color_type != PNG_COLOR_TYPE_RGB ) &&
		( color_type != PNG_COLOR_TYPE_RGB_ALPHA) ) {
		png_destroy_read_struct(&png_ptr, &info_ptr, 0);
		cerr << "wrong png_color_type" << endl;
		fclose(infile);
		exit(1);
	}

	if(bpc != 8) {
	    png_destroy_read_struct(&png_ptr, &info_ptr, 0);
		cerr << "wrong bitdepth:" << bpc << endl;
		fclose(infile);
		exit(1);
	}
					  
	switch( color_type ) {
	case PNG_COLOR_TYPE_RGB:
		zsize = 3;
		break;
	case PNG_COLOR_TYPE_RGB_ALPHA:
		zsize = 4;
		break;
	}
	
	width = x;
	height = y;
	data = (byte*) malloc( width * height * zsize );
	channels = zsize;

	row_pointers = (png_byte**) malloc( height * sizeof(png_byte*) );
	for( int row = 0; row < height; row++ )
		row_pointers[row] = data + (height - row - 1) * zsize * width;

	png_read_image( png_ptr, row_pointers );
	png_destroy_read_struct( &png_ptr, &info_ptr, 0);

	cerr << "finished loading file: " << filename << endl;
	free(row_pointers);
	fclose(infile);
}


PngTexture::~PngTexture() {
	free(data);
}

ostream& gravwars::operator << ( ostream& os, const ObjFace& f ) {
	os << "f ";
	for(	ObjFace::const_iterator iter = f.begin();
			iter != f.end();
			iter++ ) {

			os << (*iter) << ' ';
	}
	return( os );
}

ObjModel::ObjModel( char* filename ) {
	const int MAX_LINE = 256;
	char line[MAX_LINE];
	ifstream infile( filename );
	string s;
	double x, y, z;

	max = 0;

	if( !infile ) {
		cerr << "Cannot open obj file: " << filename << endl;
		exit(1);
	}

	// the 0th vertex is always the origin - this is unused by .obj files
	vertices.push_back( Vertex3( 0, 0, 0 ) );

	while( infile.getline(line, MAX_LINE) ) {
		istrstream linestream(line, strlen(line)); 
	//	cerr << "original line: " << line << endl;
//		linestream.clear();
		s = "";
//		linestream = istrstream(line, strlen(line));
	
		linestream >> s;

		if( s == "v" ) {
			linestream >> x >> y >> z;
	//		cerr << "Vertex: " << x << ' ' << y << ' ' << z << endl;
			if( abs(x) > max )
				max = abs(x);
			if( abs(y) > max )
				max = abs(y);
			if( abs(z) > max )
				max = abs(z);

			vertices.push_back( Vertex3( x, y, z ) );
		}
		else if( s == "vn" ) {
			//cerr << "Normal\n";
		}
		else if( s == "f" ) {
			ObjFace f;
			while( linestream >> x ) {
				f.push_back( x );
			}
			faces.push_back( f );
		}
	}

}

ostream& gravwars::operator << ( ostream& os, const ObjModel& o ) {
	for( int i=0; i < o.vertices.size(); i++ )
		os << o.vertices[i] << endl;

	for(	ObjFaceList::const_iterator iter = o.faces.begin();
			iter != o.faces.end();
			iter++ ) {
			
		os << (*iter) << endl;
	}
	return( os );
}

int VideoCore::glIndexCounter;
vector<PngTexture> VideoCore::textures;

void VideoCore::init() {
	glIndexCounter = 1;
}

void VideoCore::clearLists() {
	glIndexCounter = 1;
}

int VideoCore::planetList( const Planet& p ) {
	int index = glIndexCounter++;

	int slices;
	if( Flags::low_quality )
		slices = 10;
	else
		slices = 20;

	GLfloat specref[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	
	///cerr << "creating list for planet at " << p.pos << "..." << glIndexCounter << endl;

	glNewList( index, GL_COMPILE );
	glPushMatrix();
	
	glColor3f( p.r, p.g, p.b );
	glTranslatef( p.pos(0), p.pos(1), p.pos(2) );
	glutSolidSphere( p.radius, slices, slices );
	glPopMatrix();
	glEndList();

	return( index );
}

int VideoCore::asteroidList( const Asteroid& a ) {
	int index = glIndexCounter++;
	//cerr << "creating list for asteroid..." << glIndexCounter << endl;

	glNewList( index, GL_COMPILE );
	glPushMatrix();
	glColor3f( a.r, a.g, a.b );
//	glScalef( a.radius, a.radius, a.radius );
	if( Flags::low_quality )
		glutSolidSphere( a.radius, 5, 5 );
	else {
		double factor = 0.1 * a.radius;
		
		for( int offset=-1; offset<2; offset++ ) {
			glPushMatrix();
			glTranslatef( offset*factor, 0, 0 );
			glutSolidSphere( a.radius, int(randRange(4,7)), int(randRange(4,7)) );
			glPopMatrix();
		}
	}

//	glutSolidCube( a.radius * 1.7 );
	glPopMatrix();
	glEndList();

	return( index );
}

int VideoCore::shotList( const Shot& s ) {
	int index = glIndexCounter++;
	//cerr << "creating list for shot..." << glIndexCounter << endl;

	glNewList( index, GL_COMPILE );
	glPushMatrix();
	glColor3f( 1.0, 1.0, 1.0 );
//	glutWireSphere( s.radius, 10, 10 );
	if( Flags::low_quality ) {
		glScalef( s.radius * 1.5, s.radius * 1.5, s.radius * 1.5 );
		glutSolidOctahedron();
	}
	else {
		glScalef( s.radius, s.radius, s.radius );
		glutSolidDodecahedron();
	}
	glPopMatrix();
	glEndList();

	return( index );
}

int VideoCore::shipList( const Ship& s ) {
	return( objList( "data/650.obj", s.radius ) );
}

int VideoCore::objList( const ObjModel& o, double size ) {
	int index = glIndexCounter++;

	//cerr << "creating list for object..." << glIndexCounter << endl;

	glNewList( index, GL_COMPILE );
	glPushMatrix();

	//glutSolidCube( size*2 );

	glScalef( size / o.max, size / o.max, size / o.max );

	//cerr << "Scaling object from " << o.max << " to " << size << endl;

	for(	ObjFaceList::const_iterator faceIter = o.faces.begin();
			faceIter != o.faces.end();
			faceIter++ ) {
	
//		glBegin( GL_POLYGON );
//		cerr << "new face: " << endl;
		glBegin( GL_LINE_LOOP );
		
		for(	ObjFace::const_iterator vertIter = faceIter->begin();
				vertIter != faceIter->end();
				vertIter++ ) {

//			cerr << "v: "	<< o.vertices[*vertIter](0) << ' ' 
//							<< o.vertices[*vertIter](1) << ' '
//							<< o.vertices[*vertIter](2) << endl;
			glVertex3f(	o.vertices[*vertIter](0),
						o.vertices[*vertIter](1),
						o.vertices[*vertIter](2) );
		}

		glEnd();
	}

	glPopMatrix();
	glEndList();

	//cerr << "done\n";

	return( index );
}
