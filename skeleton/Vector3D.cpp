#include "Vector3D.h"
#include <cmath>

Vector3D::Vector3D(const Vector3D& other)
{
	_x = other._x;
	_y = other._y;
	_z = other._z;
}

Vector3D Vector3D::operator+(const Vector3D& other) const
{
	return Vector3D(_x + other._x, _y + other._y, _z + other._z);
}

void Vector3D::operator+=(const Vector3D& other)
{
	_x += other._x;
	_y += other._y;
	_z += other._z;
}

Vector3D Vector3D::operator-(const Vector3D& other) const
{
	return Vector3D(_x - other._x, _y - other._y, _z - other._z);
}

void Vector3D::operator-=(const Vector3D& other)
{
	_x -= other._x;
	_y -= other._y;
	_z -= other._z;
}

Vector3D& Vector3D::operator=(const Vector3D& other)
{
	if (this != &other) {
		_x = other._x;
		_y = other._y;
		_z = other._z;
	}
	return *this;
}

float Vector3D::operator*(const Vector3D& other) const
{
	return _x * other._x + _y * other._y + _z * other._z;
}

Vector3D Vector3D::operator*(float e) const
{
	return Vector3D(e * _x, e * _y, e * _z);
}

float Vector3D::normalize()
{
	float mod = std::sqrt(_x * _x + _y * _y + _z * _z); // modulo
	if (mod > 0.0f) {  // evitar division por cero
		_x /= mod;
		_y /= mod;
		_z /= mod;
	}
	return mod;
}

Vector3D Vector3D::cross(const Vector3D& other) const
{
	float cx = _y * other._z - _z * other._y;
	float cy = _z * other._x - _x * other._z;
	float cz = _x * other._y - _y * other._x;
	return Vector3D(cx, cy, cz);
}

std::ostream& operator<<(std::ostream& os, const Vector3D& v)
{
	os << "(" << v._x << ", " << v._y << ", " << v._z << ")";
	return os;
}
