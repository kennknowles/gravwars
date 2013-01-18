#include <iostream>
#include <vector>
#include <cmath>

#include "../include/matrix.h"

/*==================================*/
/*     Vector3                     */
/*==================================*/
Vector3::Vector3(	const double x=0.0,
					const double y=0.0,
					const double z=0.0 ) 
{
		myVector[0] = x;
		myVector[1] = y;
		myVector[2] = z;
}

Vector3::Vector3( const Vector3& v ) {
	for( int i=0; i<3; i++ ) {
		myVector[i] = v.myVector[i];
	}
}

double Vector3::length() const {
	double result = 0.0;

	for( int i=0; i<3; i++ ) {
		result += myVector[i] * myVector[i];
	}
	result = sqrt( result );
	
	return( result );
}
	
// in radians, of course :)
double Vector3::angleFrom( const Vector3& v ) const {
	Vector3 n = v % (*this);
	double arccos;

	arccos = acos( ((*this) * v)
					/ length() 
					/ v.length() 
				 );

	if( n(2) > 0 ) {
		return( arccos );
	}
	else {
		return( -arccos );
	}
}

void Vector3::normalize() {
	double len = length();

	for( int i = 0; i < 3; i++ ) {
		myVector[i] = myVector[i] / len;
	}
}

double& Vector3::operator () ( const unsigned int index ) {
	return( myVector[index] );
}

double Vector3::operator () ( const unsigned int index ) const {
	return( myVector[index] );
}

Vector3 Vector3::operator = ( const Vector3& v3 ) {
	for( int i = 0; i < 3; i++ )
		myVector[i] = v3(i);
	
	return( *this );
}

Vector3 Vector3::operator += ( const Vector3& v3 ) {
	for( int i = 0; i < 3; i++ )
		myVector[i] += v3(i);
	
	return( *this );
}

Vector3 Vector3::operator -= ( const Vector3& v3 ) {
	for( int i = 0; i < 3; i++ )
		myVector[i] -= v3(i);
	
	return( *this );
}

double operator * ( const Vector3& v1, const Vector3& v2 ) {
	double result = 0.0;

	for( int i=0; i < 3; i++ ) {
		result += v1(i) * v2(i);
	}
	
	return( result );
}

Vector3 operator * ( const double d, const Vector3& v ) {
	Vector3 result;

	for( int i=0; i < 3; i++ ) {
		result(i) = d * v(i);
	}

	return( result );
}

Vector3 operator + ( const Vector3& v1, const Vector3& v2 ) {
	Vector3 result;
	
	for( int i=0; i<3; i++ )
		result(i) = v1(i) + v2(i);

	return( result );
}

Vector3 operator - ( const Vector3& v1, const Vector3& v2 ) {
	Vector3 result;
	
	for( int i=0; i<3; i++ )
		result(i) = v1(i) - v2(i);

	return( result );
}

// cross product
Vector3 operator % ( const Vector3& v1, const Vector3& v2 ) {
	Vector3 result;

	result(0) = v1(1)*v2(2) - v1(2)*v2(1);
	result(1) = v1(2)*v2(0) - v1(0)*v2(2);
	result(2) = v1(0)*v2(1) - v1(1)*v2(0);

	return( result );
}

ostream& operator << ( ostream& os, const Vector3& v ) {
	os << '(' << v(0) << ' ' << v(1) << ' ' << v(2) << ')'; 
	return( os );
}

// expects space delimited values
istream& operator >> ( istream& os, Vector3& v ) {
	os >> v(0);
	os >> v(1);
	os >> v(2);
	return( os );
}


/*==================================*/
/*     Matrix33                     */
/*==================================*/
Matrix33::Matrix33() {
	for( int r=0; r < 3; r++ ) {
		for( int c=0; c < 3; c++ ) {
			myMatrix[r][c] = 0;
		}
	}
}

Matrix33::Matrix33( const Matrix33& m ) {
	for( int r=0; r < 3; r++ ) {
		for( int c=0; c < 3; c++ ) {
			myMatrix[r][c] = m(r, c);
		}
	}
}

Matrix33::~Matrix33() {
}

Vector3 Matrix33::getRow( const unsigned int r ) const {
	Vector3 result;

	for( int c=0; c<3; c++ ) {
		result(c) = myMatrix[r][c];
	}

	return( result );
}

Vector3 Matrix33::getCol( const unsigned int c ) const {
	Vector3 result;

	for( int r=0; r<3; r++ )
		result(r) = myMatrix[r][c];
	
	return( result );
}


Matrix33 Matrix33::operator = ( const Matrix33& m ) {
	for( int r=0; r < 3; r++ ) {
		for( int c=0; c < 3; c++ ) {
			myMatrix[r][c] = m(r, c);
		}
	}
	return( (*this) );
}

double& Matrix33::operator () ( const unsigned int row, const unsigned int col ) {
	return( myMatrix[row][col] );
}

double Matrix33::operator () ( const unsigned int row, const unsigned int col ) const {
	return( myMatrix[row][col] );
}

const Matrix33 Matrix33::identity() {
	Matrix33 m;

	for( int r=0; r<3; r++ ) {
		for( int c=0; c<3; c++ ) {
			m(r, c) = (r==c) ? 1 : 0;
		}
	}
	return( m );
}

const Matrix33 Matrix33::zero() {
	Matrix33 m;

	return( m );
}

Matrix33 Matrix33::rotate( double theta, const Vector3& axis ) {
	//uses formulas derived from quaternion rotation
	
	Vector3 u, v;
	double s, a, b, c;
	Matrix33 m = identity();

	u = axis;
	u.normalize();

	s = cos( theta / 2 );
	v = sin( theta / 2 ) * axis;

	a = v(0);
	b = v(1);
	c = v(2);

    m(0, 0) = 1 - 2*b*b - 2*c*c;
    m(0, 1) = 2*a*b - 2*s*c;
    m(0, 2) = 2*a*c + 2*s*b;

    m(1, 0) = 2*a*b + 2*s*c;
    m(1, 1) = 1 - 2*a*a - 2*c*c;
    m(1, 2) = 2*b*c - 2*s*a;

    m(2, 0) = 2*a*c - 2*s*b;
    m(2, 1) = 2*b*c + 2*s*a;
    m(2, 2) = 1 - 2*a*a - 2*b*b;

	return( m );
}

Matrix33 operator * ( const Matrix33& m1, const Matrix33& m2 ) {
	Matrix33 result;

	for( int r = 0; r < 3; r++ ) {
		for( int c = 0; c < 3; c++ ) {
			result( r, c ) = m1.getRow( r ) * m2.getCol( c );
		}
	}

	return( result );
}

Matrix33 operator * ( const double d, const Matrix33& m ) {
	Matrix33 result;

	for( int r = 0; r < 3; r++ ) {
		for( int c = 0; c < 3; c++ ) {
			result( r, c ) = d * m( r, c );
		}
	}

	return( result );
}

Vector3 operator * ( const Matrix33& m, const Vector3& v ) {
	Vector3 result;

	for( int r = 0; r < 3; r++ ) {
		result(r) = m.getRow(r) * v;
	}

	return( result );
}

Matrix33 operator + ( const Matrix33& m1, const Matrix33& m2 ) {
	Matrix33 result;

	for( int r=0; r<3; r++ ) {
		for( int c=0; c<3; c++ ) {
			result(r, c) = m1(r, c) + m2(r, c);
		}
	}

	return( result );
}

ostream& operator << ( ostream& os, const Matrix33& m ) {
	for( int r=0; r < 3; r++ ) {
		for( int c=0; c < 3; c++ ) {
			os << m( r, c ) << ' ';
		}
		os << endl;
	}
	return( os );
}
