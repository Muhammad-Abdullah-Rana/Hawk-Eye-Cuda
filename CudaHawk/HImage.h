#pragma once

# include "Base.h"

class HawkFrame
{
	int*** data=nullptr;
	int channel=0;
	int row=0;
	int col=0;
public:
	HawkFrame();
	HawkFrame(Mat& frame);
	HawkFrame(const HawkFrame& frame);
	~HawkFrame();

	void set(Mat& frame);
	void set(int r, int c, int channels);
	void set(int r, int c, int channels, int*** data);


	bool isNull();

	int getRows();
	int getCols();
	int getChannels();

	bool compare_update(HawkFrame& frame);
	void setPixel(uchar* pxs, int r, int c);
	void setColor(int val, int r, int c,int ch);
	int* getPixel(int r, int c);
	int getColor(int r, int c,int ch);


	bool read(const char* path);
	bool write(const char* path);


	int grey_BGR(int b,int g,int r);

	void showGrey();
	void show();

};

