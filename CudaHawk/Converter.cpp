#include "Converter.h"
int**  Converter::get_Grey(VideoCapture& vid, int& row, int& col){
    Mat frame;
    vid.read(frame);
    row = frame.rows;
    col = frame.cols;
    return convert_Grey(frame);
}
void Converter::get_Grey(VideoCapture& vid, Mat& frame) {
    vid.read(frame);
    convert_Grey_Mat(frame);
}

int*** Converter::get_Color(VideoCapture& vid, int& row, int& col,int& channel){
    Mat frame;
    vid.read(frame);
    row = frame.rows;
    col = frame.cols;
    channel = frame.channels();
    return convert_Color(frame);
}
void Converter::get_Color(VideoCapture& vid, Mat& frame) {
    vid.read(frame);
}
Image Converter::get_Color(VideoCapture& vid) {
    Mat frame;
    vid.read(frame);
    return Image(frame);
}
void Converter::get_Color(VideoCapture& vid,Image& img) {
    Mat frame;
    vid.read(frame);
    img.replaceData(frame);
}

int*** Converter::convert_Color(Mat frame) {
    int*** arr = new int** [frame.rows];
    for (int i = 0; i < frame.rows; i++) {
        arr[i] = new int* [frame.cols];
        for (int j = 0; j < frame.cols; j++) {
            arr[i][j] = new int[frame.channels()];
            uchar* pixel = frame.at < Vec3b >(i, j).val;
            for (int k = 0; k < frame.channels(); k++) {
                arr[i][j][k] = (int)pixel[k];
            }
        }
    }
    return arr;
}
int**  Converter::convert_Grey(Mat frame) {
    int** arr = new int* [frame.rows];
    for (int i = 0; i < frame.rows; i++) {
        arr[i] = new int[frame.cols];
        for (int j = 0; j < frame.cols; j++) {
            uchar* pixel = frame.at<Vec3b>(i, j).val;
            arr[i][j] = (uchar)pixel[0] * 0.3 + (uchar)pixel[1] * 5.9 + (uchar)pixel[2] * 0.31;
        }
    }
    return arr;
}
int**  Converter::convert_Grey(int*** arr, int x, int y) {
    int** greyFrame = new int* [x];
    for (int i = 0; i < x; i++) {
        greyFrame[i] = new int[y];
        for (int j = 0; j < y; j++) {
            int* p = arr[i][j];
            int grey = ((int)p[0] * 0.3) + ((int)p[1] * 0.59) + ((int)p[2] * 0.11);
            greyFrame[i][j] = grey;
        }
    }
    return greyFrame;
}
void   Converter::convert_Grey_Mat(Mat& frame) {
    for (int i = 0; i < frame.rows; i++) {
        for (int j = 0; j < frame.cols; j++) {
            uchar* p = frame.at<Vec3b>(i, j).val;
            int gray = ((int)p[0] * 0.3) + ((int)p[1] * 0.59) + ((int)p[2] * 0.11);
            for (int k = 0; k < frame.channels(); k++) {
                p[k] = gray;
            }
        }
    }
}

Mat Converter::convert_Mat(int** arr, int x, int y) {
    Mat frame = Mat(x, y, 16);
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            uchar* pixel = frame.at<Vec3b>(i, j).val;
            pixel[2] = (uchar)arr[i][j];
            pixel[1] = (uchar)arr[i][j];
            pixel[0] = (uchar)arr[i][j];
        }
    }
    return frame;
}
Mat Converter::convert_Mat(int*** arr, int x, int y) {
    Mat frame = Mat(x, y, 16);
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            uchar* pixel = frame.at<Vec3b>(i, j).val;
            pixel[2] = (uchar)arr[i][j][0];
            pixel[1] = (uchar)arr[i][j][1];
            pixel[0] = (uchar)arr[i][j][2];
        }
    }
    return frame;
}


