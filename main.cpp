// A Simple Ray Tracer

#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <vector>
#include "objects.h"

using namespace std;

#pragma region
int width = 750;
int height = 500;
double fov = 0.8;
Ray camera = Ray(Vector(), Vector(1, 0, 0));


Color background = Color();
double ambient = 0;

#pragma endregion settings

// declaring arrays
vector<Object*> objects;
vector<Lightsource> lightsources;


double clam(double x, double upper, double lower = 0) {
	return x > upper ? upper : x < lower ? lower : x;
}

void intersection(Ray ray, int &n, double &t) {
	double tB = -1;
	n = 0;
	for (int i = 0; i < objects.size(); i++) {
		double temp = objects[i]->intersects(ray);
		if ((tB == -1 && temp > 0) || (temp < tB && temp > 0)) {
			tB = temp;
			n = i;
		}
	}
	t = tB;
}

void getColor(Ray ray, Color &c) {

	// find the closest intersection
	double t;
	int n;
	intersection(ray, n, t);

	// if no intersection was found, return
	if (t == -1)
		return;

	// declare intensity of color
	double intensity;;

	// calculate intersection point Q
	Vector Q = ray.origin + ray.direction * t;

	// variable for keeping track of the received light
	Color totalD = Color(0, 0, 0);
	Color totalS = Color(0, 0, 0);


	// for every lightsource, do:
	for (int i = 0; i < lightsources.size(); i++) {
		// make a ray from intersection point to light source
		Ray light  = Ray(Q, lightsources[i].position - Q);
		double lightDistSq = light.direction.length(true);
		light.direction = !light.direction;

		// move the intersection point a tiny bit, otherwise it intersects with itself
		light.origin = light.origin + (light.direction * 0.01);

		// look for objects between light and intersection point
		int tempN;
		double tempT;
		intersection(light, tempN, tempT);

		// if there is an object inbetween, go to the next lightsource
		if (tempT != -1 && tempT * tempT < lightDistSq)
			continue;		


		// SECOND COMPONENT: diffuse
		double diffuse = objects[n]->getNormal(Q) ^ !light.direction;
		Color diffuseC = objects[n]->color * diffuse;

		// if the diffuse is less than zero, it means the objects receives no light at all
		if (diffuse <= 0)
			continue;

		// THIRD COMPONENT: specular
		Vector view = !(camera.origin - Q);
		Vector reflection = (light.direction * -1) - objects[n]->getNormal(Q) * 2 * ((light.direction * - 1 ) ^ objects[n]->getNormal(Q));
		double specular = pow(max(view ^ reflection, 0.0), objects[n]->shinyness);
		Color specularC = lightsources[i].color * specular;

		totalD = totalD + diffuseC;
		totalS = totalS + specularC;
	}
	// multiply the color of the object with the light intensity;
	c = (objects[n]->color * ambient) + totalD + totalS;
	return;
}

Vector getVector(string str) {
	double a, b, c;
	for (int i = 0; i < 3; i++) {
		string tmp = str.substr(0, str.find(","));
		str = str.substr(str.find(",") > str.length() ? 0 : str.find(","), str.length() - str.find(","));
		str.erase(0, 1);

		switch (i) {
			case 0: 
				stringstream(tmp) >> a;
				break;
			case 1: 
				stringstream(tmp) >> b;
				break;
			case 2: 
				stringstream(tmp) >> c;
				break;
		}

		
	}
	return Vector(a, b, c);
}

void initialize() {

	string inp = "";

	int i = 0;
	do {

		getline(cin, inp);

		switch (i) {
			case 0: 
				stringstream(inp.substr(0, inp.find(" "))) >> width;
				break;
			case 1: 
				stringstream(inp.substr(0, inp.find(" "))) >> height;
				break;
			case 2: 	
				stringstream(inp.substr(0, inp.find(" "))) >> fov;
				break;
			case 3: 
				stringstream(inp.substr(0, inp.find(" "))) >> ambient;
				break;

			case 4: {
				Vector pos;
				Vector dir;
				for (int x = 0; x < 2; x++)
				{
					string tmp = inp.substr(0, inp.find(" "));
					inp = inp.substr(inp.find(" ") > inp.length() ? 0 : inp.find(" "), inp.length() - inp.find(" "));
					inp.erase(0, 1);

					switch (x) {
						case 0:
							pos = getVector(tmp);
							break;
						case 1: 
							dir = getVector(tmp);
							break;
					}

					
				}

				camera = Ray(pos, dir);
				}
				break;

			case 5: {
				string tmp = inp.substr(0, inp.find(" "));
				Vector c = getVector(tmp);
				background = Color(c.x, c.y, c.z);
				}
				break;

			case 6: {
				int n;
				stringstream(inp.substr(0, inp.find(" "))) >> n;
				for (int x = n; x > 0; x--) {
					getline(cin, inp);
					Vector a, b, c;
					Color clr;
					int s;
					for (int y = 0; y < 5; y++) {
						string tmp = inp.substr(0, inp.find(" "));
						inp = inp.substr(inp.find(" ") > inp.length() ? 0 : inp.find(" "), inp.length() - inp.find(" "));
						inp.erase(0, 1);
						switch (y) {
							case 0:
								a = getVector(tmp);
								break;

							case 1:
								b = getVector(tmp);
								break;

							case 2:
								c = getVector(tmp);
								break;

							case 3: {
								Vector clrT = getVector(tmp);
								clr = Color(clrT.x, clrT.y, clrT.z);
								}
								break;

							case 4:
								stringstream(tmp) >> s;
								break;
						}
					}
					objects.push_back(new Triangle(a, b, c, s, clr, camera.direction));
				}

				}
				break;
			case 7: {
				int n;
				stringstream(inp.substr(0, inp.find(" "))) >> n;
				for (int x = n; x > 0; x--) {
					getline(cin, inp);
					Vector c;
					double r;
					Color clr;
					int s;
					for (int y = 0; y < 4; y++) {
						string tmp = inp.substr(0, inp.find(" "));
						inp = inp.substr(inp.find(" ") > inp.length() ? 0 : inp.find(" "), inp.length() - inp.find(" "));
						inp.erase(0, 1);
						switch (y) {
							case 0:
								c = getVector(tmp);
								break;

							case 1:
								stringstream(tmp) >> r;
								break;

							case 2: {
								Vector clrT = getVector(tmp);
								clr = Color(clrT.x, clrT.y, clrT.z);
								}
								break;

							case 3:
								stringstream(tmp) >> s;
								break;
						}
					}
					objects.push_back(new Sphere(c, r, s, clr));
				}
				}
				break;
			case 8: {
				int n;
				stringstream(inp.substr(0, inp.find(" "))) >> n;
				for (int x = n; x > 0; x--) {
					getline(cin, inp);
					Vector p;
					string tmp = inp.substr(0, inp.find(" "));
					inp = inp.substr(inp.find(" ") > inp.length() ? 0 : inp.find(" "), inp.length() - inp.find(" "));
					inp.erase(0, 1);
					p = getVector(tmp);
					lightsources.push_back(Lightsource(p, 0));
					}
				}
				break;
		}

		i++;
	} while (inp.substr(0, inp.find(" ")) != "END");
}

int main() {
	

	// initalizing settings and objects
	initialize();

	// Making a loop that casts a ray through every pixel of an imaginary screen,
	// placing that screen one unit in front of the camera, and giving the screen a fixed horizontal field of view (must be less than pi, since a horizontal field of view would be infinite)

	// Calculating the width of the screen in units
	double scrWidth = 2 * tan(fov);

	// Calculating the height of the screen, respecting the ratio
	double scrHeight = scrWidth * height / width;

	// Pre-calculating the pixelsize, to enhance performance (only need to do width or height (not both), since the pixels are square and have the same width and height)
	double pixelSize = scrWidth / width;

	// writing header of image file to stdout
	cout << "P3 " << width << " " << height << " 255\n";

	// The loop that goes through every pixel
	for (int z = height / 2; z > -height / 2; --z) {
		for (int y = -width / 2; y < width / 2; ++y) {
			// For every pixel, generate a ray that passes through it
			//
			// The origin of the ray is the same as the position of the camera, because that is where the ray is sent from
			// The direction of the ray pointing to a pixel in the screen can be computed by adding the vector from the middle of the screen to the current pixel (the exact middle of the pixel), and the normalized direction vector of the camera
			// (the normalized direction vector of the camera points to the middle of the screen, per definition)
			Vector direction = !(camera.direction + Vector(0, y * pixelSize + 0.5 * pixelSize, z* pixelSize + 0.5 * pixelSize));
			Ray ray = Ray(camera.origin, direction);

			// getting the color of each pixel using the function
			Color color = background;
			getColor(ray, color);
			// writing the color to stdout
			cout << color.r << " " << color.g << " " << color.b << " ";
		}
		// necessary for file format
		cout << "\n";
	}

	return 0;
}
