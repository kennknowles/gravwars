#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>

/*===================================================*/
class Vector3 {
    private:
		double myVector[3];

	public:
		Vector3(	const double x=0.0,
					const double y=0.0,
					const double z=0.0 );
		Vector3( const Vector3& v );

		double length() const;
		double angleFrom( const Vector3& v ) const;
		
		void normalize();

		double& operator () ( const unsigned int index );
		double operator () ( const unsigned int index ) const;

		Vector3 operator = ( const Vector3& v3 );
		Vector3 operator += ( const Vector3& v3 );
		Vector3 operator -= ( const Vector3& v3 );
};

double operator * ( const Vector3& v1, const Vector3& v2);
Vector3 operator * ( const double d, const Vector3& v );
Vector3 operator + ( const Vector3& v1, const Vector3& v2 );
Vector3 operator - ( const Vector3& v1, const Vector3& v2 );


// just use % as cross product
Vector3 operator % ( const Vector3& v1, const Vector3& v2);

ostream& operator << ( ostream& os, const Vector3& v ); 
istream& operator >> ( istream& is, Vector3& v );

class Matrix33 {
	private:
		double myMatrix[3][3];

	public:
		Matrix33();
		Matrix33( const Matrix33& m );
		~Matrix33();

		Vector3 getRow( const unsigned int r ) const;
		Vector3 getCol( const unsigned int c ) const;

		Matrix33 operator = ( const Matrix33& m );

		// one for assignment
		double& operator () ( const unsigned int row, const unsigned int col );

		// one for access
		double operator () ( const unsigned int row, const unsigned int col ) const;

		static const Matrix33 identity();
		static const Matrix33 zero();

		static Matrix33 rotate( double theta, const Vector3& axis );
};

Matrix33 operator * ( const Matrix33& m1, const Matrix33& m2 );
Matrix33 operator * ( const double d, const Matrix33& m );
Vector3 operator * ( const Matrix33& m, const Vector3& v );

Matrix33 operator + ( const Matrix33& m1, const Matrix33& m2 );

ostream& operator << ( ostream& os, const Matrix33& m );
#endif
