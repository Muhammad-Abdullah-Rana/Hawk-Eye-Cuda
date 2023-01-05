#pragma once
#include "Base.h"
#include <stdexcept>

class Image
{
	int rows=0, cols=0, channels=0;
	int*** pxs=nullptr;
	
public:
	Image() {}
	Image(int r, int c, int channel) :rows(r),cols(c),channels(channel) {
		pxs = new int** [rows];
		for (int r = 0; r < rows; r++) {
			pxs[r] = new int* [cols];
			for (int c = 0; c < cols; c++) {
				pxs[r][c] = new int[channels] {};
			}
		}
	}
	Image(cv::Mat img) :rows(img.rows),cols(img.cols),channels(img.channels()) {
		pxs = new int** [rows];
		
		for (int r = 0; r < rows; r++) {
			pxs[r] = new int* [cols];
			for (int c = 0; c < cols; c++) {
				pxs[r][c] = new int[channels];
				for (int ch = 0; ch < channels; ch++) {
					pxs[r][c][ch] =(int)img.at<cv::Vec3b>(r, c)[ch];
				}
			}
		}
	}
	Image(const Image& img) :rows(img.rows),cols(img.cols),channels(img.channels) {
		pxs = new int** [rows];

		for (int r = 0; r < rows; r++) {
			pxs[r] = new int* [cols];
			for (int c = 0; c < cols; c++) {
				pxs[r][c] = new int[channels];
				for (int ch = 0; ch < channels; ch++) {
					pxs[r][c][ch] =img.pxs[r][c][ch];
				}
			}
		}
	}
	Image& operator=(const Image& img){
		if (rows == img.rows && cols == img.cols && channels == img.channels) {
			replaceData(img.pxs);
		}
		else {
			setImage(img.pxs, img.rows, img.cols, img.channels);
		}

		return *this;
	}  

	
	int getHeight() { return rows; }
	int getWidth() { return cols; }
	int getChannel() { return channels; }
	int pixelCount() { return rows * cols * channels; }
	int*** getData() { return pxs; }

	bool empty() {
		return pxs == nullptr;
	}


	void setImage(cv::Mat img) {
		if (pxs != nullptr) {
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < cols; j++)
					delete pxs[i][j];
				delete pxs[i];
			}
			delete[]pxs;
		}

		rows = img.rows;
		cols = img.cols;
		channels = img.channels();

		pxs = new int** [rows];
		
		for (int r = 0; r < rows; r++) {
			pxs[r] = new int* [cols];
			for (int c = 0; c < cols; c++) {
				pxs[r][c] = new int[channels];
				for (int ch = 0; ch < channels; ch++) {
					pxs[r][c][ch] = (int)img.at<cv::Vec3b>(r, c)[ch];
				}
			}
		}

	}
	void setImage(int *** img,int row,int col,int chan) {
		if (pxs != nullptr) {
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < cols; j++)
					delete pxs[i][j];
				delete pxs[i];
			}
			delete[]pxs;
		}

		rows = row;
		cols = col;
		channels = chan;

		pxs = new int** [rows];
		for (int r = 0; r < rows; r++) {
			pxs[r] = new int* [cols];
			for (int c = 0; c < cols; c++) {
				pxs[r][c] = new int[channels] {};
				for (int ch = 0; ch < channels; ch++) {
					pxs[r][c][ch] = img[r][c][ch];
				}
			}
		}

	}

	/// <summary>
	/// only use when you are damn sure that data size is same as previous data size

	/// </summary>
	/// <param name="arr"></param>
	void replaceData(int*** arr) {
		if (pxs == nullptr && rows > 0 && cols > 0 && channels > 0)
		{
			pxs = new int** [rows];
			for (int r = 0; r < rows; r++) {
				pxs [r] = new int* [cols];
				for (int c = 0; c < cols; c++) {
					pxs[r][c] = new int [channels];
					for (int ch = 0; ch < channels; ch++) {
						pxs[r][c][ch] = arr[r][c][ch];
					}
				}
			}

		}
		else {
			for (int r = 0; r < rows; r++) {
				for (int c = 0; c < cols; c++) {
					for (int ch = 0; ch < channels; ch++) {
						pxs[r][c][ch] = arr[r][c][ch];
					}
				}
			}
		}
		
	}

	// replace data always but if empty make new
	void replaceData(Mat img) {
		if (pxs == nullptr && rows == 0 && cols == 0 && channels == 0)
		{
			rows = img.rows;
			cols = img.cols;
			channels = img.channels();

			pxs = new int** [rows];
			for (int r = 0; r < rows; r++) {
				pxs [r] = new int* [cols];
				for (int c = 0; c < cols; c++) {
					pxs[r][c] = new int [channels];
					for (int ch = 0; ch < channels; ch++) {
						pxs[r][c][ch] = img.at<Vec3b>(r,c)[ch];
					}
				}
			}

		}
		else if(pxs!=nullptr) {
			for (int r = 0; r < rows; r++) {
				for (int c = 0; c < cols; c++) {
					for (int ch = 0; ch < channels; ch++) {
						pxs[r][c][ch] = img.at<Vec3b>(r, c)[ch];
					}
				}
			}
		}
		
	}

	int get(int r, int c, int cc) {
		if (r < 0 || r >= rows || c<0 || c>cols || cc < 0 || cc >= channels)
			return-1;
		if (pxs == nullptr)
			return -1;
		return pxs[r][c][cc];
	}
	int* at(int r, int c, int cc) {
		if (r < 0 || r >= rows || c<0 || c>cols || cc < 0 || cc >= channels)
			return nullptr;
		if (pxs == nullptr)
			return nullptr;
		return &pxs[r][c][cc];
	}

	~Image() {
		if (pxs != nullptr) {
			for (int i = 0; i < rows; i++) {
				if (pxs[i] != nullptr) {
					for (int j = 0; j < cols; j++)
						if(pxs[i][j]!=nullptr)
							delete pxs[i][j];
					delete pxs[i];
				}
			}
			delete[]pxs;
		}
	}

};

