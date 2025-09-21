#pragma once
#include <iostream>
class Vector3D
{
public:
	Vector3D(float x = 0.0, float y = 0.0, float z = 0.0) :_x(x), _y(y), _z(z) {};
	Vector3D(const Vector3D& other);

	//sobrecargar operadores

	Vector3D operator+(const Vector3D& other) const;
	void operator+=(const Vector3D& other);

	Vector3D operator-(const Vector3D& other)const;
	void operator-=(const Vector3D& other);

	Vector3D& operator=(const Vector3D& other);

	float operator*(const Vector3D& other) const; //producto escalar con otro vector
	Vector3D operator*(float e) const; //producto con un escalar

	float normalize();

	Vector3D cross(const Vector3D& other) const;
	
	friend std::ostream& operator<<(std::ostream& os, const Vector3D& v);

private:
	float _x, _y, _z;
};

