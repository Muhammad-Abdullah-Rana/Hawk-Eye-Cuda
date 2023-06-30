#pragma once
#include "PersonDetectModule.cuh"

class CamHandler {
public:
	CamHandler(std::string path, int id) :_id(id){
		vid = cv::VideoCapture(path);
		std::cout << "Stream Loading.";
	}
	bool load() {
		return vid.isOpened();
	}
	void setup() {
		vid.read(img);
		segment = PersonDetect(_id);
	//	segment.init(img);
		segment.showDetectedPerson(false);
	}
	bool loop() {
		if (segment.shouldEnd())
			return true;
		vid.read(img);

		if (segment.fit(img)) {
			if (segment.transform()) {
				string str = "segment(";
				str += _id;
				str += ")";
				imshow(str, segment.getOrignalFrame());
				waitKey(5);
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
	VideoCapture vid;
	Mat img;
	PersonDetect segment;
};

