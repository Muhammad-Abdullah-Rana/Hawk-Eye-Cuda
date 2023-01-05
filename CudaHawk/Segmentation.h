#pragma once
#include <iostream>
#include <cstdint>

#include"Base.h"
#include"Converter.h"
#include"Comparetor.h"
#include"Image.h"



/*  Segemetatio types defined */
#define GREY_SEGMENTATION 1
#define THRESHOULD_SEGMENTATION 2
#define HYBRID_SEGMENTATION 3

/*  Fix Frame size for a Thresould */
#define FRAME_COUNT 10

class Segmentation
{
private:
	Converter convert;
	Comparetor compare;
	Base base;
	
	bool isThresould = false;
	//int countDown = 10;

	Mat _curImg, _pImg;
	Mat _first, _avg, img;
	vector<Mat> _imgs;

	int _method;
	int _blvl;

	void _fit_grey(Mat&);
	void _fit_threshould(Mat&);
	void _transform_threshould();
	uchar _getImgesChannel(int,int,int);

	/* Slow type implementation with Image dataType */


	Image temp;
	Image first, avg, curImg;
	vector<Image> imgs;

	/// <summary>
	/// making thresould of frames
	/// </summary>
	/// <param name="img"> frame / image</param>
	void makeThresould(Image t) {

		if (FRAME_COUNT > imgs.size()) {
			if (imgs.size() == 0) {
				first = t;
			}
			imgs.push_back(t);
			isThresould = false;
		}
		else {
			avg = Image(imgs[0].getHeight(), imgs[0].getWidth(), imgs[0].getChannel());
			for (int r = 0; r < imgs[0].getHeight(); r++) {
				for (int c = 0; c < imgs[0].getWidth(); c++) {
					for (int cc = 0; cc < imgs[0].getChannel(); cc++) {
						*avg.at(r, c, cc) = getImgesChannel(r, c, cc);
					}
				}
			}
			isThresould = true;
		}
	}

	int getImgesChannel(int r, int c, int chan) {
		int* color = new int[FRAME_COUNT];
		for (int t = 0; t < FRAME_COUNT; t++) {
			color[t] = (imgs.at(t)).get(r, c, chan);
		}
		int n = base.mode(color, FRAME_COUNT);
		delete[] color;
		return n;
	}

public:
	Segmentation(int method = GREY_SEGMENTATION, int lvl = 90) :_method(method), _blvl(lvl) {}

	/// <summary>
	/// this function take a VideoCapture refrence and from it read one frame 
	/// </summary>
	/// <param name="VideoCapture"> refrencnce of video</param>
	/// <return>curr image under segmentation also can be found _getCurFrame()</return>
	Mat _fit(VideoCapture& vid);

	/// <summary>
	/// transform the image/frame by seprating froground and background and return an img with only foreground 
	/// by segmention method provided
	/// </summary>
	/// <return>curr frame after segmentation also can be found _getForeGroundFrame()</return>
	Mat _transform();

	/// <summary>
	/// current colored frame
	/// </summary>
	/// <returns>current colored frame </returns>
	Mat _getCurFrame();
	
	/// <summary>
	/// current processed frame if process else null
	/// </summary>
	/// <returns>current processed frame if process else null </returns>
	Mat _getForeGroundFrame();

	/// <summary>
	/// true = video provided is ended
	/// </summary>
	/// <return>true = video provided is ended</return>
	bool _shouldEnd();

	///////////////////////////////////////////////////////////////////////////////////////////////

	/*   Slow Implementation   not compeleted */

	//////////////////////////////////////////////////////////////////////////////////////////////

	Image getCurFrame() { return curImg; }
	Image fit(VideoCapture& vid) {
		convert.get_Color(vid,temp);
		while (!isThresould) {
			if (temp.empty()) {
				break;
			}
			makeThresould(temp);
			convert.get_Color(vid,temp);
		}
		curImg = temp;
		return temp;
	}
	Image transform() {
		if (isThresould) {
			curImg.replaceData(compare.compareImages(
				curImg.getData(), avg.getData(), first.getData(),
				curImg.getHeight(), curImg.getWidth(),
				curImg.getChannel()));


		}
		return curImg;
	}
	Image fit_transform(Image img) {}

	bool shouldEnd() {
		return curImg.empty();
	}

	


	// Some function utility
	/*
	bool _isValid(int x, int y) {
		// in the boundry of img or not
		if (x < 0 || x >= _curImg.rows || y < 0 || y >= _curImg.cols) {
			return false;
		}

		return ((int)_curImg.at<Vec3b>(x, y)[0] > 0);
	}
	int _9Count(int x,int y) {
		int n = 0;
	
		n+=_isValid(x - 1, y + 1);
		n+=_isValid(x - 1, y);
		n+=_isValid(x - 1, y - 1);

		n+=_isValid(x, y + 1);
		n+=_isValid(x, y);
		n+=_isValid(x, y - 1);

		n+=_isValid(x + 1, y + 1);
		n+=_isValid(x + 1, y);
		n+=_isValid(x + 1, y - 1);
		
		return n;
	}

	*/
	






	
	
	


















};


