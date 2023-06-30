#pragma once
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "opencv2/opencv.hpp"

#include "Defines.cuh"
#include "Structs.cuh"
#include "Base.h"

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <unordered_map>
#include <vector>
#include <chrono>


using namespace cv;
using namespace std;
using namespace chrono;


class PersonDetect{
public:
	PersonDetect();
	PersonDetect(int camId);
	bool shouldEnd();
	bool fit(Mat&);
	void showDetectedPerson(bool _s);
	bool transform();
	Mat getOrignalFrame();
	void finilize();

protected:
	Person getPerson(Mat& frame, cv::Rect boundingRect);
	float calculateCentroidDistance(const cv::Point2f& centroid1, const cv::Point2f& centroid2);
	void save(const cv::Mat& personImage, int id);
	void log(Person p);
private:	
	int camId;
	bool _show=false;
	Basics basics;
	Mat frame;					// orignal frame
	cv::Mat bgModel;
	cv::Mat fgMask;

	cudaError_t cudaStatus = cudaSuccess;
	long _count= 0;

	int personId = 0;
	std::unordered_map<int, Person> persons;
};
