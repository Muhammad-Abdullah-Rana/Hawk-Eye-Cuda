#include "HImage.h"

HawkFrame::HawkFrame() {}
HawkFrame::HawkFrame(Mat& frame) {set(frame);}
HawkFrame::HawkFrame(const HawkFrame& frame) {
	data = frame.data;
	channel = frame.channel;
	col = frame.col;
	row = frame.row;
}
HawkFrame::~HawkFrame() {
	if (data != nullptr) {
		for (int i = 0; i < row; i++) {
			if (data[i] != nullptr) {
				for (int j = 0; j < col; j++) {
					if (data[i][j] != nullptr)
						delete data[i][j];
				}
				delete data[i];
			}
		}
		delete data;
	}
}



void HawkFrame::set(Mat& frame){
	if (frame.empty())
		return;
	row = frame.rows;
	col = frame.cols;
	channel = frame.channels();

	data = new int** [row];
	for (int i = 0; i < row; i++) {
		data[i] = new int* [col];
		for (int j = 0; j < col; j++) {
			data[i][j] = new int[channel];
			uchar* px = frame.at<Vec3b>(i, j).val;
			for (int k = 0; k < channel; k++) {
				data[i][j][k] = (int)px[k];
			}
		}
	}
}
void HawkFrame::set(int r, int c, int ch) {
	row = r;
	col = c;
	channel = ch;
	data = new int** [row];
	for (int i = 0; i < row; i++) {
		data[i] = new int* [col];
		for (int j = 0; j < col; j++) {
			data[i][j] = new int[channel] {};
		}
	}
}
void HawkFrame::set(int r, int c, int ch, int*** px) {
	row = r;
	col = c;
	channel = ch;
	data = px;
}


int HawkFrame::getRows() { return row;}
int HawkFrame::getCols() { return col; }
int HawkFrame::getChannels() { return channel; }


void HawkFrame::setPixel(uchar* pxs, int r, int c) {
	for (int i = 0; i < channel; i++) {
		data[r][c][i] = (int)pxs[i];
	}
}
void HawkFrame::setColor(int val, int r, int c,int ch) {
	data[r][c][ch] = val;
}
int* HawkFrame::getPixel(int r, int c){
	return data[r][c];
}
int HawkFrame::getColor(int r, int c,int ch) {
	return data[r][c][ch];
}
bool HawkFrame::read(const char* path) { return false; }   // not yet to implement
bool HawkFrame::write(const char* path) { return false; }  // not yet to implement

int HawkFrame::grey_BGR(int b, int g, int r) {
	return (int)(b * 0.072) + (g * 0.7152) + (r * 0.2126);
}
int COLOR_BOUND(int v1, int v2) { return (v1 > v2) ? ((v1 - v2) > RANGE ? v2 : v1) : ((v2 - v1) > RANGE ? v2 : v1); }


void HawkFrame::showGrey() {
	cv::Mat frame = cv::Mat(row, col, 16);
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			uchar* pix = frame.at<cv::Vec3b>(i, j).val;
			if (channel == 3) {
				int g = grey_BGR(data [i][j][0], data[i][j][1], data[i][j][2]);
				pix[0] = (uchar)g;
				pix[1] = (uchar)g;
				pix[2] = (uchar)g;
			}
			else
				for (int k = 0; k < channel; k++) {
					
					pix[k] = (uchar)data[i][j][k];
				}

		}
	}
	imshow("frame", frame);
	waitKey(5);

}
void HawkFrame::show() {
	cv::Mat frame = cv::Mat(row, col,16);
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			uchar* pix = frame.at<cv::Vec3b>(i, j).val;
			for (int k = 0; k < channel; k++) {
				pix[k] = (uchar)getColor(i, j, k);
			}
			
		}
	}
	imshow("frame", frame);
	waitKey(5);
}


bool HawkFrame::compare_update(HawkFrame& frame) {
	if (channel == frame.channel && row == frame.row && col == frame.col) {
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				for (int k = 0; k < channel; k++)
					data[i][j][k] = COLOR_BOUND(getColor(i, j, k), frame.getColor(i,j,k));
			}
		}
		return true;
	}
	return false;
}
bool HawkFrame::isNull() {
	return (row<=0);
}