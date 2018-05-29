#include <cmath>
#include <stdio.h>
#include <iostream>
#include "objects.h"

using namespace std;

	int clam(int x, int upper, int lower) {
		return x > upper ? upper : x < lower ? lower : x;
	}

	Vector::Vector(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	
	Vector Vector::operator+(Vector v) {
		return Vector(x + v.x, y + v.y, z + v.z);
	}
	
	Vector Vector::operator+(double d) {
		return Vector(x + d, y + d, z + d);
	}
	
	Vector Vector::operator-(Vector v) {
		return Vector(x - v.x, y - v.y, z - v.z);
	}
	
	Vector Vector::operator-(double d) {
		return Vector(x - d, y - d, z - d);
	}

	Vector Vector::operator*(Vector v) {
		return Vector(x * v.x, y * v.y, z * v.z);
	}
	
	Vector Vector::operator*(double d) {
		return Vector(x * d, y * d, z * d);
	}
	
	Vector Vector::operator/(Vector v) {
		return Vector(x / v.x, y / v.y, z / v.z);
	}
	
	Vector Vector::operator/(double d) {
		return d == 0 ? Vector(0, 0, 0) : Vector(x / d, y / d, z / d);
	}
	
	// returns unit vector
	Vector Vector::operator!() {
		return Vector::clone() / Vector::length();
	}
	
	// returns dot product
	double Vector::operator^(Vector v) {
		return x * v.x + y * v.y + z * v.z;
	}
	
	// returns cross product
	Vector Vector::operator&(Vector v) {
		return Vector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
	
	double Vector::length(bool squared) {
		return squared ? x * x + y * y + z * z : sqrt(x * x + y * y + z * z);
	}
	
	Vector Vector::clone() {
		return Vector(x, y, z);
	}

	Ray::Ray(Vector origin, Vector direction) {
		this->origin = origin;
		this->direction = direction;
	}

	double Ray::length(bool squared) {
		return direction.length(squared);
	}

	double Ray::lengthFromCenter() {
		return (origin + direction).length();
	}

	Triangle::Triangle(Vector p0, Vector p1, Vector p2, int shinyness, Color color, Vector camDir) {
		this->p0 = p0;
		this->p1 = p1;
		this->p2 = p2;
		this->shinyness = shinyness;
		this->color = color;
		// calculate normal of the triangle
		this->normal = !((this->p1 - this->p0) & (this->p2 - this->p0));
		if ((this->normal ^ camDir) > 0) {
			this->p0 = this-> p0 + this->p2.clone();
			this->p2 = this->p0.clone() - this->p2;
			this->p0 = this->p2.clone() - this->p0;
			this->normal = !((this->p1 - this->p0) & (this->p2 - this->p0));
		}
	}

	double Triangle::intersects(Ray ray) {
		// if the triangle plane and the ray are parralel, there is no intersection
		double nd = this->normal ^ !ray.direction;

		if (nd == 0)
			return -1;

		// compute d (from plane equation)
		double d = this->normal^this->p0;

		// calculating t and getting the point from t
		double t = (d - (this->normal ^ ray.origin)) / nd;
		Vector Q = ray.origin + ray.direction * t;
		//cout << t << "   " << Q.x <<"\n";
		// checks if the intersection point lies on the triangle
		if (((this->p1 - this->p0) & (Q - this->p0) ^ this->normal) >= 0 && ((this->p2 - this->p1) & (Q - this->p1) ^ this->normal) >= 0 && ((this->p0 - this->p2) & (Q - this->p2) ^ this->normal) >= 0)
			return t;

		return -1;
	}

	Vector Triangle::getNormal(Vector p) {
		return this->normal;
	}

	Sphere::Sphere(Vector c, double r, int shinyness, Color color) {
		this->c = c;
		this->r = r;
		this->shinyness = shinyness;
		this->color = color;
	}

	double Sphere::intersects(Ray ray) {
		Vector l = this->c - ray.origin;

		double tc = ray.direction ^ l;
		if (tc <= 0) 
			return -1;

		Vector tcPoint = ray.origin + ray.direction * tc;
		Vector d = this->c - tcPoint;

		if (d.length(true) > this->r * this->r)
			return  -1;

		double t1c = sqrt(this->r * this->r - d.length(true));

		if (tc - t1c < 0)
			return -1;
		return tc - t1c;
	}

	Vector Sphere::getNormal(Vector p) {
		return !(p - this->c);
	}

	Color::Color(int r, int g, int b) {
		this->r = r;
		this->g = g;
		this->b = b;
	}

	Color Color::operator*(double d) {
		return Color(this->r * d, this->g * d, this->b * d);
	}

	Color Color::operator*(Color c) {
		return Color(this->r * (c.r / 255), this->g * (c.g / 255), this->b * (c.b / 255));
	}

	Color Color::operator+(Color c) {
		return Color(clam(this->r + c.r, 255), clam(this->g + c.g, 255), clam(this->b + c.b, 255));
	}

	Lightsource::Lightsource(Vector position, double range, Color color) {
		this->position = position;
		this->range = range;
		this->color = color;
	}