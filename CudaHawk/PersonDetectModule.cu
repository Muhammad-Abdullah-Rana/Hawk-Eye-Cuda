#include "PersonDetectModule.cuh"

PersonDetect::PersonDetect(){
	camId = -1;
	cudaStatus = cudaSetDevice(0);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
	}
}
PersonDetect::PersonDetect(int cid) {
	camId = cid;
	cudaStatus = cudaSetDevice(0);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
	}
}
bool PersonDetect::shouldEnd() { return cudaStatus != cudaSuccess; }
bool PersonDetect::fit(Mat& curr) {
	_count++;
	frame = curr.clone();
	Mat grayFrame;
	basics.convertGrey(curr, grayFrame);
	basics.checkDiffrence(grayFrame, bgModel, fgMask);
	bgModel = 0.9 * bgModel + 0.1 * grayFrame;
	basics.removeNoise(fgMask, 40);
	return true;
}

void print(Location l) {
	fprintf(stderr, "CamId: %d, PId: %d, time: %d, loc.x: %f,  loc.y %f \n",l.camId, l.pid, l.time, l.locX, l.locY);
}

void PersonDetect::save(const Mat& personImage, int id) {
	// Placeholder function to save person image and ID to a database
	// Implement the logic to save the image and ID to your specific database
	if (_show) {
		string str = "ID: ";
		str += (id);
		cv::imshow(str, personImage);
	}
}
void PersonDetect::log(Person p) {
	Location loc;
	loc.pid = p.id;
	loc.camId = camId;
	loc.locX = p.centroid.x;
	loc.locY = p.centroid.y;
	auto currentTime = system_clock::now().time_since_epoch();
	loc.time = duration_cast<seconds>(currentTime).count();
	print(loc);
}
void PersonDetect::showDetectedPerson(bool _s) {
	_show = _s;
}
bool PersonDetect::transform(){
	//imshow("mask", fgMask);
	// waiting for background learning
	if (_count < FRAME_THRESH_COUNT) {
		return false;
	}
	vector<std::vector<cv::Point>> contours;
	basics.findContours(fgMask, contours);
	
	// Process each contour to detect and track persons
	for (const auto& contour : contours) {
		double area = cv::contourArea(contour);
		cv::Rect boundingRect = cv::boundingRect(contour);
		//if (boundingRect.height / boundingRect.width >= 1.25f) 
		if (area > 500 ) {
			Person person = getPerson(frame, boundingRect);
			Mat personImage = frame(person.boundingRect).clone();
			save(personImage, person.id);
			log(person);

			// Draw bounding box with person ID on the frame
			cv::rectangle(frame, person.boundingRect, cv::Scalar(0, 255, 0), 2);
			cv::putText(frame, std::to_string(person.id), cv::Point(person.boundingRect.x, person.boundingRect.y - 10),
				cv::FONT_HERSHEY_SIMPLEX, 0.9, cv::Scalar(0, 255, 0), 2);;
		}
	}
	return !shouldEnd();
}
Mat PersonDetect::getOrignalFrame() { return frame; }

void PersonDetect::finilize(){
	if (shouldEnd())
		cout << cudaGetErrorString(cudaStatus) << endl;

	cudaStatus = cudaDeviceReset();

}

float PersonDetect::calculateCentroidDistance(const cv::Point2f& centroid1, const cv::Point2f& centroid2) {
	return cv::norm(centroid1 - centroid2);
}
Person PersonDetect::getPerson(Mat& frame, cv::Rect boundingRect) {
	cv::Point2f centroid = (boundingRect.tl() + boundingRect.br()) * 0.5f;
	

	for (auto& pair : persons) {
		Person& person = pair.second;

		// Calculate distance between centroids
		float distance = calculateCentroidDistance(person.centroid, centroid);

		// Check if the distance is smaller than a threshold
		if (distance < 50) {
			person.boundingRect = boundingRect;
			person.centroid = centroid;
			persons[personId] = person;
			return person;
		}
	}

	// If person not found, assign a new ID and add to the list
	Person newPerson;
	newPerson.id = personId;
	newPerson.boundingRect = boundingRect;
	newPerson.centroid = centroid;
	persons[personId] = newPerson;

	personId++;
	return newPerson;
}
