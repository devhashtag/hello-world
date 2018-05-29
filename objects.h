#ifndef OBJECTS
#define OBJECTS

int clam(int x, int upper, int lower = 0);

class Vector {
	public:
	double x, y, z;
	
	Vector(double x = 0, double y = 0, double z = 0);
	
	Vector operator+(Vector v);
	
	Vector operator+(double d);
	
	Vector operator-(Vector v);
	
	Vector operator-(double d);

	Vector operator*(Vector v);
	
	Vector operator*(double d);
	
	Vector operator/(Vector v);
	
	Vector operator/(double d);
	
	// returns unit vector
	Vector operator!();
	
	// returns dot product
	double operator^(Vector v);
	
	// returns cross product
	Vector operator&(Vector v);
	
	// calculates the length. If squared is true, the square value of the length will be returned (reduces amount of computations)
	double length(bool squared = false);
	
	// returns a vector object with the same properties
	Vector clone();
};

class Ray {
	public:
	Vector origin, direction;

	Ray(Vector origin, Vector direction);

	double length(bool squared = false);

	double lengthFromCenter();
};

class Color {
public:
	int r, g, b;
	Color(int r = 0, int g = 0, int b = 0);

	Color operator*(double d);
	Color operator+(Color c);
	Color operator*(Color c);
	
};

class Object {
public:
	int shinyness;
	Vector normal;
	Color color;

	virtual double intersects(Ray ray) = 0;
	virtual Vector getNormal(Vector p) {};
};

class Triangle : public Object {
public:
	double intersects(Ray ray);
	Vector getNormal(Vector p);
	Vector p0, p1, p2;

	Triangle(Vector p0 = Vector(), Vector p1 = Vector(), Vector p2 = Vector(), int shinyness = 32, Color color = Color(200, 50, 50), Vector camDir = Vector(1, 0, 0));
};

class Sphere : public Object {
public:
	double intersects(Ray ray);
	Vector getNormal(Vector p);
	Vector c;
	double r;

	Sphere(Vector c = Vector(), double r = 1, int shinyness = 32, Color color = Color(50, 200, 50));
};

class Lightsource {
public:
	double range;
	Vector position;
	Color color;

	Lightsource(Vector position, double range, Color color = Color(255, 255, 255));
};
#endif
