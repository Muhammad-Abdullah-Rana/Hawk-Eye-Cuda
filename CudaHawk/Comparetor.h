#pragma once
# include "Base.h"
//#include "HList.h"

#include <iostream>
#include <cmath>
/* 
<summary>
* functions with '_BW' always return or update data in Base.BLACK and Base.WHITE 
* </summary>
*/
class Comparetor{
public:
    Mat thresouldMethod_BW(Mat& bw) {
        Mat result = bw.clone();
        for (int i = 0; i < bw.rows; i++) {
            for (int j = 0; j < bw.cols; j++) {
                if (bw.at<Vec3b>(i, j)[0] > 90) {  
                    // 75 is clear and 90 is more clear for sen building video

                    result.at<Vec3b>(i, j)[0] = (uchar)0;
                    result.at<Vec3b>(i, j)[1] = (uchar)0;
                    result.at<Vec3b>(i, j)[2] = (uchar)0;
                }
                else {
                    result.at<Vec3b>(i, j)[0] = (uchar)255;
                    result.at<Vec3b>(i, j)[1] = (uchar)255;
                    result.at<Vec3b>(i, j)[2] = (uchar)255;
                }
            }
        }
        return result;
    }
    Mat thresouldMethod_Color(Mat& bw) {
        Mat result = bw.clone();
        for (int i = 0; i < bw.rows; i++) {
            for (int j = 0; j < bw.cols; j++) {
                Vec3b pixel = bw.at<Vec3b>(i, j);
                int gray = 0.2126 * pixel[2] + 0.7152 * pixel[1] + 0.0722 * pixel[0];
                if (gray <= 127) {
                    result.at<Vec3b>(i, j)[0] = (uchar)0;
                    result.at<Vec3b>(i, j)[1] = (uchar)0;
                    result.at<Vec3b>(i, j)[2] = (uchar)0;
                }
                else {
                    result.at<Vec3b>(i, j)[0] = (uchar)255;
                    result.at<Vec3b>(i, j)[1] = (uchar)255;
                    result.at<Vec3b>(i, j)[2] = (uchar)255;
                }
            }
        }
        return result;
    }

    // Assume that the images are represented as 3D arrays of integers.
   // The dimensions of the arrays are: height x width x channel
    int*** compareImages(int*** image1, int*** image2, int height, int width, int channel) {
        // Create a new 3D array to store the result image
        int*** result = new int** [height];

        for (int i = 0; i < height; i++) {
            result[i] = new int* [width];

            for (int j = 0; j < width; j++) {
                result[i][j] = new int[channel];
                for (int c = 0; c < channel; c++) {
                    if (image1[i][j][c] != image2[i][j][c]) {
                        // If the pixels are different, set the result pixel to the value from one of the input images
                        result[i][j][c] = image1[i][j][c];
                    }
                    else {
                        // If the pixels are the same, set the result pixel to 0
                        result[i][j][c] = 0;
                    }
                }
            }
        }

        return result;
    }


    // Assume that the images are represented as 3D arrays of integers.
    // The dimensions of the arrays are: height x width x channel
    int*** compareImages(int*** image1, int*** image2, int*** image3, int height, int width, int channel) {
        // Create a new 3D array to store the result image
        int*** result = new int** [height];
        for (int i = 0; i < height; i++) {
            result[i] = new int* [width];
            for (int j = 0; j < width; j++) {
                result[i][j] = new int[channel];
                for (int c = 0; c < channel; c++) {
                    // Check if the pixels in the first two images are different by more than 5
                    bool diff = std::abs(image1[i][j][c] - image2[i][j][c]) > 5;
                    if (diff) {
                        // If the pixels in the first two images are different, check if the pixel in the third image is within 5 of either of them
                        bool same = std::abs(image1[i][j][c] - image3[i][j][c]) <= 5 || std::abs(image2[i][j][c] - image3[i][j][c]) <= 5;
                        if (same) {
                            // If the pixel in the third image is within 5 of either of the first two, set the result pixel to 0
                            result[i][j][c] = 0;
                        }
                        else {
                            // If the pixel in the third image is not within 5 of either of the first two, set the result pixel to the value from one of the input images
                            result[i][j][c] = image1[i][j][c];
                        }
                    }
                    else {
                        // If the pixels in the first two images are not different, set the result pixel to 0
                        result[i][j][c] = 0;
                    }
                }

            }
        }
        return result;
    }
    Mat _compareImages(Mat& image1,Mat& image2,Mat& image3) {
        Mat result = image1.clone();  // create a copy of image1 to store the result

        int col, row;
        for (row = 0; row < image1.rows; row++) {
            for (col = 0; col < image1.cols; col++) {
                bool diff = allChange(image1.at<Vec3b>(row, col), image2.at<Vec3b>(row, col));

                // Compare pixel values of image1 and image2
                if (diff) {
                     diff = allChange(image2.at<Vec3b>(row, col), image3.at<Vec3b>(row, col));

                    // Compare pixel values of image2 and image3
                    if (diff)  {
                        // Set pixel value to image1's value if image2 and image3 differ
                        //result.at<Vec3b>(row, col) = image1.at<Vec3b>(row, col);
                        result.at<Vec3b>(row, col) = (Vec3b)((uchar)255, (uchar)255, (uchar)255);
                    }
                    else {
                        // Set pixel value to 0 if image2 and image3 are the same
                        result.at<Vec3b>(row, col) = (Vec3b)((uchar)0, (uchar)0, (uchar)0);
                    }
                }  
                else {
                    // Set pixel value to 0 if image1 and image2 are the same
                    result.at<Vec3b>(row, col) = (Vec3b)((uchar)0, (uchar)0, (uchar)0);
                }
              
            }
        }

        return result;
    }

    bool change(uchar p1,uchar p2,int range) {
        return (abs((int)p1-(int)p2) > range);

    }
    bool allChange(Vec3b& p1, Vec3b& p2) {
        return change(p1[0], p2[0],5) && change(p1[1], p2[1],10) && change(p1[2], p2[2],5);
    }
    bool allChange(Vec2b& p1, Vec2b& p2) {
        return change(p1[0], p2[0],5) && change(p1[1], p2[1],5);
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="cur">will be update</param>
    /// <param name="thresh">threshould frame</param>
    /// <param name="firstThresh">one frame in formation of threshould</param>
    /// <param name="row">rows</param>
    /// <param name="cols">cols</param>
    /// <param name="channel">channels</param>
    void objectify(int*** cur, int*** thresh, int*** firstThresh, int row, int col, int channel);
    Mat objectify(Mat cur, Mat thresh, Mat firstThresh);



    /// <summary>
    ///  check if the difference between values is less than error
    /// </summary>
    /// <param name="o"> Old value</param>
    /// <param name="n"> New Value</param>
    /// <param name="err"> Error: change in value to be ignore</param>
    /// <returns> true if value is not in range </returns>
    bool checkInRange(int o, int n, int err);

    /// <summary>
    /// 
    /// </summary>
    /// <param name="n"> New Value</param>
    /// <param name="o"> Old value </param>
    /// <param name="po"> Prev of Old value</param>
    /// <param name="error"> Error: change in value to be ignore</param>
    /// <returns> CHANGED if new and old are far not equal, 
    /// UNCHANGED if new and old and also prev of old is in range 
    /// FLIKER if old and neware in range but prev of old is diffferent
    /// </returns>
    int valueChanged(int n,int o, int po, int error);

    /// <summary>
    /// compare frames in 3D array and Mat  
    /// </summary>
    /// <param name="t"> 3D arr of equal size with sencd parameter frame</param>
    /// <param name="cur"> Mat Frame</param>
    /// <returns> 2D array having pixels containg 255 if found change and 0 if same </returns>
    int** compareFrame_BW(int*** t, Mat cur);

    /// <summary>
    ///  frames in 3D array and Mat same pixel will be Base.BLACK and rest will be same as new
    /// </summary>
    /// <param name="t"> 3D arr of equal size with sencd parameter frame</param>
    /// <param name="cur"> Mat Frame</param>
    void compareFrame_Color(Mat& cur, int*** t);

    /// <summary>
    /// 
    /// </summary>
    /// <param name="cur">will be update</param>
    /// <param name="t">threshould frame</param>
    /// <param name="st">one frame in formation of threshould</param>
    void compareFrame_Color(Mat& cur, int*** t, int*** st);


    /// <summary>
    ///  compare two Mat's and update first parameter 
    /// </summary>
    /// <param name="cur"> Current frame (will be update)</param>
    /// <param name="prev"> Previous frame unchanged</param>
    void compareFrame_BW(Mat& cur, Mat prev);
    
    /// <summary>
    ///  compare two Array's and update first parameter 'cur'
    /// </summary>
    /// <param name="cur"> Current frame (will be update)</param>
    /// <param name="prev"> Previous frame unchanged</param>
    /// <param name="x"> rows</param>
    /// <param name="y"> cols</param>
    void compareFrame_BW(int** cur, int** prev,int x,int y);

    /// <summary>
    ///  compare two Array's and update first parameter 'cur'
    /// </summary>
    /// <param name="cur"> Current frame (will be update)</param>
    /// <param name="prev"> Previous frame unchanged</param>
    /// <param name="x"> rows</param>
    /// <param name="y"> cols</param>
    /// <param name="z"> color</param>
    void compareFrame_BW(int*** cur, int*** prev,int x,int y,int z);


    /// <summary>
    ///  compare three Mat's , copmare cur and thresold and then prev and thresold
    /// </summary>
    /// <param name="cur"> Current frame (will be update)</param>
    /// <param name="prev"> Previous frame unchanged</param>
    /// <param name="thresold"> Thresold frame unchanged</param>
    void compareFrame_BW(Mat& cur, Mat prev,Mat thresold);


    Mat difference(const Mat& f1, const Mat& f2);
    void difference_Grey(const Mat& f1, Mat& f2);
    int** diffrenceBW(const Mat& f1,const Mat& f2); 
    int*** diffrenceColor(const Mat& f1,const Mat& f2); 

};

