#pragma once
#include <corecrt_math_defines.h>
#include <iostream>
#include <cmath>

class Coords {
public:
	Coords(){}
	Coords(double x, double y) :x_(x), y_(y) {}
	void setCoords(double x, double y) { x_ = x; y_=y; }
	void setCoords(const Coords c) { x_ = c.x_; y_=c.y_; }
	double getXcord() { return x_; }
	double getYcord() { return y_; }
    bool isEmpty() { return x_ == -1; }
    Coords& Empty() { 
        x_ = -1;
        return *this; }
private:
	double x_, y_;
};

class Location {
public:
	Location(){}
	int setCoords(double x, double y) {
		coords.setCoords(x, y);
	}
    void setCoords(const Coords c) {
        coords.setCoords(c);
    }
	Coords getCoords() {
		return coords;
	}	
    // cam = 31.451747,74.292998
    // fov = 68.0
    // height = 35 feet or 10668 mm

    Coords getGeoLocation(Coords head,Coords foot,Coords camCoords, double fov,double height,double orientation) { 
        // Calculate the width and height of the person in the image
        double xdf = abs(foot.getXcord() - head.getXcord());
        double ydf = abs(head.getYcord() - foot.getYcord());

        // Calculate the x and y positions of the person in the image as a fraction of the image size
        double xPos = abs(coords.getXcord() - head.getXcord()) / xdf;
        double yPos = abs(coords.getYcord() - head.getYcord()) / ydf;



        // Calculate the angle from the center of the image to the person in the x and y directions
        double xAngle = (xPos - 0.5) * fov;
        double yAngle = (yPos - 0.5) * fov;

        // Convert the camera orientation and angles to radians
        double orientationRadians = orientation * M_PI / 180.0;

        double xAngleRadians = xAngle * M_PI / 180.0;
        double yAngleRadians = yAngle * M_PI / 180.0;

        // Calculate the distance from the camera to the person
        double distance = height / (2 * std::tan(fov * M_PI / 360.0));

        // Calculate the geolocation of the person
        double xGeo = camCoords.getXcord() + distance * std::sin(orientationRadians + xAngleRadians);
        double yGeo = camCoords.getXcord() + distance * std::cos(orientationRadians + yAngleRadians);

        return Coords(xGeo, yGeo);
    }

private:
	Coords coords;
};