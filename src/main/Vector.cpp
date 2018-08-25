#include <cmath>
#include "../utils/Vector.h"
#include "../utils/Color.h"

//Constructors
Vector::Vector(): x(0.0f), y(1.0f), z(0.0f) {

}

Vector::Vector(const Vector& v): x(v.x), y(v.y), z(v.z) {

}

Vector::Vector(float x, float y, float z): x(x), y(y), z(z) {

}

Vector::Vector(float f): x(f), y(f), z(f) {

}

//Destructor
Vector::~Vector() {

}

//Function implementation
//Length2 means length squared
inline float Vector::length2() {
	return sqr(x) + sqr(y) + sqr(z);
}

//the actual length is the square root of the length2
inline float Vector::length() {
	return std::sqrt(length2());
}
   //Transform the current vector to unit vector, effectively the same as the normalize function, but this one is local
inline void Vector::transformToUnit(){
	float k = 1.0 / this->length();

	this->x *= k;
	this->y *= k;
	this->z *= k;
}

//Normalize a vector's x,y,z (map their values between 0 and 1)
float Vector::normalize() {
	float len = length();
	//division between a vector and it's length
	*this /= len;

	return len;
}

void Vector::setColor(float r, float b, float g) {
	this->color = Color(r, g, b);
}

void Vector::setColor(Color color) {
	this->color = color;
}

Vector Vector::normalized() {
	Vector v(*this);
	v.normalize();
	return v;
}

float dot(Vector v1, Vector v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector cross(Vector v1, Vector v2) {
	return Vector(v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x);
}

//Operator overloading so we can add/multiply/divide vectors easily
Vector& Vector::operator =(const Vector& v) {
	x = v.x;
	y = v.y;
	z = v.z;

	color = v.color;

	return *this;
}

Vector& Vector::operator +=(const Vector& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector& Vector::operator -=(const Vector& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector& Vector::operator *=(float f) {
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

Vector& Vector::operator /=(float f) {
	x /= f;
	y /= f;
	z /= f;
	return *this;
}

Vector Vector::operator -() const {
	return Vector(-x, -y, -z);
}

//Vector2 Constructors
Vector2::Vector2(): u(0.0f), v(0.0f) {

}

Vector2::Vector2(const Vector2 &v): u(v.u), v(v.v) {

}

Vector2::Vector2(float u, float v): u(u), v(v) {

}

Vector2::Vector2(float f): u(f), v(f) {
}

//Vector2 Deconstructors
Vector2::~Vector2() {
    
}

Vector2& Vector2::operator =(const Vector2& v)
{
	u = v.u;
	this->v = v.v;
	return *this;
}