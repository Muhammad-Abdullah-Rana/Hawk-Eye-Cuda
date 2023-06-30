#pragma once

#include "opencv2/opencv.hpp"
struct Person {
	int id;
	cv::Rect boundingRect;
	cv::Point2f centroid;
};


struct Location {
	float locX, locY;			// location
	int pid;		       // person id
	int camId;				// camera Id
	long time;			// sec time
};
