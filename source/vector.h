#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

extern const float PI;


float degToRad( float ug  );

class Vector
{
public:
	float x,y;
public:
	inline Vector(void)
	{}

	inline Vector(float Ix,float Iy)
	: x(Ix)
	, y(Iy)
	{}

	inline void Set(int x1, int y1) {x=x1; y=y1;};
	inline Vector &operator /=(const float Scalar)	{ x /= Scalar; y /= Scalar;		return *this; }

	inline Vector &operator *=(const float Scalar)	{ x *= Scalar; y *= Scalar;		return *this; }
	
	inline Vector &operator +=(const Vector &Other) { x += Other.x;	y += Other.y;	return *this; }

	inline Vector &operator -=(const Vector &Other) { x -= Other.x;	y -= Other.y;	return *this;	}

	inline float operator ^ (const Vector &V)	const	{	return (x * V.y) - (y * V.x); } // cross product

	inline float operator * (const Vector &V)	const	{	return (x*V.x) + (y*V.y); } // dot product

	inline Vector operator * (float  s)			const	{	return Vector(x*s, y*s); }
	
	inline Vector operator / (float  s)			const	{	return Vector(x/s, y/s); }
	
	inline Vector operator + (const Vector &V)	const	{	return Vector(x+V.x, y+V.y); }
		
	inline Vector operator - (const Vector &V)	const	{	return Vector(x-V.x, y-V.y); }

	friend Vector operator * (float k, const Vector& V) {	return Vector(V.x*k, V.y*k); }

	
	inline Vector operator -(void) const { return Vector(-x, -y); }
	
		inline float Length(void) const { return (float) sqrt(x*x + y*y); }

	float Normalise(void) 
	{	
		float fLength = Length();	
		
		if (fLength == 0.0f) 
			return 0.0f; 
			
		(*this) /= fLength;
	
		return fLength;	
	}

	Vector Direction(void) const
	{
		Vector Temp(*this);

		Temp.Normalise();

		return Temp;
	}
	

	Vector perp() const
	{
       Vector a= *this;
		return Vector(-a.y, a.x);
	}

	Vector& rotate(float angle)
	{
		float tx = x;
		x =  x * cos(degToRad(angle)) - y * sin(degToRad(angle));
		y = tx * sin(degToRad(angle)) + y * cos(degToRad(angle));
		return *this;
	}

	Vector& transform(const Vector& trans, float rot)
	{
		Vector D = *this;
		D.rotate(rot);
		*this = trans + D;
		return *this;
	}


};

#endif

