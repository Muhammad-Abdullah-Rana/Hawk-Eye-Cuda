#pragma once
#include <crow.h>  // Include Crow library
#include<map>
#include <unordered_set>
#include "PersonDetectionModule.cuh"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "opencv2/opencv.hpp"
#include<cmath>
#include<crow.h>
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <chrono>
class CamHandler {
public:
	std::map<int, std::map<uchar, int>> greyColorVals;

	CamHandler(std::string path, int id) : _id(id) {
        vid = cv::VideoCapture(path);
        std::cout << "Loading Stream " << id << std::endl;
        imgCount = 0;
    }
	bool load() {
		return vid.isOpened();
	}
	void setup() {
		vid.read(img);
        segment = PersonDetection(_id);
		segment.showDetectedPerson(false);
		segment.prepare(img);
	}
	void preloop() {
		for (int i = 0; i < 100; i++) {
				vid.read(img);
				segment.learning(img);
		}
	}
    bool loop(std::unordered_set<crow::websocket::connection*>& clients) {

        if (segment.shouldEnd()) {
            return true;
        }
        vid.read(img);
		if (segment.learning(img)) {
            if (imgCount++ >= 1000) {
                segment.extraction(greyColorVals, true);
                imgCount = 0;
            }
            else
                segment.extraction(greyColorVals, false);
            
            std::string str = "mask";
            str += _id;
            std::string str1 = "segment";
            str1 += _id;
            cv::waitKey(6);
            cv::Mat frame = segment.getOrignalFrame();
            std::vector<uchar> buffer;
            cv::imencode(".jpg", frame, buffer);
            std::string imageData(buffer.begin(), buffer.end());
            for (auto client : clients) {
                client->send_binary(imageData);
            }
        }
        return false;
    }
	void destroy() {
		segment.finilize();
		vid.release();
	}
private:
    int _id;
    cv::VideoCapture vid;
    cv::Mat img;
    PersonDetection segment;
    short int imgCount;
};

