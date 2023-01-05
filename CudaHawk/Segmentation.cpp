#include "Segmentation.h"


Mat  Segmentation::_getCurFrame() { return _curImg; }
Mat  Segmentation::_getForeGroundFrame() { return _pImg; }
bool Segmentation::_shouldEnd() {
	return _curImg.empty();
}

Mat  Segmentation::_fit(VideoCapture& vid) {
	vid.read(img);
	_curImg = img.clone();
	if (img.empty()) {
		return img;
	}

	switch (_method) {
	default:
	case GREY_SEGMENTATION: {
		_fit_grey(img);
		break;
	}
	case THRESHOULD_SEGMENTATION: {
		while (!isThresould) {
			_fit_threshould(img);

			vid.read(img);
			_curImg = img.clone();
			if (img.empty()) {
				return _curImg;
			}
		}
		break;
	}
	case HYBRID_SEGMENTATION: {
		while (!isThresould) {
			_fit_grey(img);

			_fit_threshould(img);

			vid.read(img);
			_curImg = img.clone();
			if (img.empty()) {
				return _curImg;
			}
		}
		break;
	}
	}
	return _curImg;
}

void Segmentation::_fit_grey(Mat& ii) {
	//kernel_segmentation_fit_grey(ii,_blvl);
	

	for (int i = 0; i < ii.rows; i++) {
		for (int j = 0; j < ii.cols; j++) {
			uchar* p = ii.at<Vec3b>(i, j).val;
			int gray = ((int)p[0] * 0.3) + ((int)p[1] * 0.59) + ((int)p[2] * 0.11);
			for (int k = 0; k < ii.channels(); k++) {
				if (gray > _blvl) p[k] = (uchar)255;
				else p[k] = (uchar)0;
			}
		}
	}
	_pImg = ii.clone();
}
void Segmentation::_fit_threshould(Mat& ii) {
	if (FRAME_COUNT > _imgs.size()) {
		if (_imgs.size() == 0) {
			_first = ii.clone();

		}
		_imgs.push_back(ii.clone());
		isThresould = false;
	}
	else {
		_avg = _first.clone();
		for (int r = 0; r < _avg.rows; r++) {
			for (int c = 0; c < _avg.cols; c++) {
				for (int cc = 0; cc < _avg.channels(); cc++) {
					_avg.at<Vec3b>(r, c)[cc] = _getImgesChannel(r, c, cc);
				}
			}
		}
		isThresould = true;
	}

	//if (isThresould) {
	//	countDown--;
	//	if (countDown <= 1) {
	//		_first = _imgs.at(1);
	//		_imgs.erase(_imgs.begin());
	//		isThresould = false;
	//		countDown = 10;
	//	}
	//}

}


Mat  Segmentation::_transform() {
	if (_method == THRESHOULD_SEGMENTATION || _method == HYBRID_SEGMENTATION)
		_transform_threshould();
	return _pImg;
}

void Segmentation::_transform_threshould() {
	if (isThresould) {
		_pImg = img.clone();
		int col, row;
		for (row = 0; row < img.rows; row++) {
			for (col = 0; col < img.cols; col++) {
				bool diff = compare.allChange(img.at<Vec3b>(row, col), _avg.at<Vec3b>(row, col));

				// Compare pixel values of image1 and image2
				if (diff) {
					diff = compare.allChange(_avg.at<Vec3b>(row, col), _first.at<Vec3b>(row, col));

					// Compare pixel values of image2 and image3
					if (diff) {
						// Set pixel value to image1's value if image2 and image3 differ
						//result.at<Vec3b>(row, col) = image1.at<Vec3b>(row, col);
						_pImg.at<Vec3b>(row, col) = (Vec3b)(255, 255,255);
					}
					else {
						// Set pixel value to 0 if image2 and image3 are the same
						_pImg.at<Vec3b>(row, col) = (Vec3b)((uchar)0, (uchar)0,(uchar) 0);
					}
				}
				else {
					// Set pixel value to 0 if image1 and image2 are the same
					_pImg.at<Vec3b>(row, col) = (Vec3b)((uchar)0, (uchar)0, (uchar)0);
				}

			}
		}
	}
}

uchar Segmentation::_getImgesChannel(int r, int c, int chan) {
	int* color = new int[FRAME_COUNT];
	for (int t = 0; t < FRAME_COUNT; t++) {
		color[t] = (int)(_imgs.at(t)).at<Vec3b>(r, c)[chan];
	}
	int n = base.mode(color, FRAME_COUNT);
	delete[] color;
	return (uchar)n;
}






/*
void Segmentation::makeThresould(Mat img) {
	if (threshArraySize == 0) {
		firstThresh = img;
	//	threshArray = new Mat[THRESH_IMG];
	}
	if (THRESH_IMG > threshArraySize) {
		threshArray[threshArraySize] = img;
		threshArraySize++;
		isThresould = false;
	}
	else {
		thresh = Mat(img.rows, img.cols, 16);
		for (int r = 0; r < img.rows; r++) {
			for (int c = 0; c < img.cols; c++) {
				for (int cc = 0; cc < img.channels(); cc++) {
					int*color=new int[threshArraySize];
					for (int t = 0; t < threshArraySize; t++) {
						uchar px = (threshArray[t]).at<Vec3b>(r, c)[cc] ;
						color[t]= (int)px;
					}
					int n = base.mode(color, threshArraySize);
					thresh.at<Vec3b>(r,c)[cc] = (uchar)n;
					delete color;
				}
			}
		}
		isThresould = true;
	}

}

*/
/*
void Segmentation::transform() {
	if (isThresould){

		//curImg= compare.objectify(curImg, thresh, firstThresh);

		// will add code about thresould time limit here
		/*


		*/


//Mat Segmentation::fit_transform(Mat img) {
//	if (isThresould) {
//		for (int r = 0; r < img.rows; r++) {
//			for (int c = 0; c < img.cols; c++) {
//				if ((curImg.at<Vec3b>(r, c)[0]) == (uchar)WHITE ) {
//					curImg.at<Vec3b>(r, c) = img.at<Vec3b>(r, c);
//				}
//			}
//		}
//		return curImg;
//	}
//	return Mat();
//}