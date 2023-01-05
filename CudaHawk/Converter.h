#pragma once
# include "Base.h"
# include "Image.h"

#include <cstdio>
#include <cstdint>
//#include <jpeglib.h>

class Converter
{
    /// <summary>
    /// pass image path and refrence of size to get its size for later use
    /// </summary>
    /// <param name="filename"> name and path </param>
    /// <param name="height"> rows</param>
    /// <param name="width">cols</param>
    /// <param name="channel">channels</param>
    /// <returns>image in 3d</returns>
    /*
    int*** jpegTo3DArray(const std::string& fileName,int& height,int& width,int& channel) {
        // Open the JPEG file
        FILE* fp = fopen(fileName.c_str(), "rb");
        if (!fp) {
            throw std::runtime_error("Failed to open file: " + fileName);
        }

        // Create a JPEG decompression object
        jpeg_decompress_struct cinfo;
        jpeg_error_mgr jerr;
        cinfo.err = jpeg_std_error(&jerr);
        jpeg_create_decompress(&cinfo);

        // Specify the input file
        jpeg_stdio_src(&cinfo, fp);

        // Read the JPEG header and start the decompression process
        jpeg_read_header(&cinfo, TRUE);
        jpeg_start_decompress(&cinfo);

        // Get the dimensions of the image
        height = cinfo.output_height;
        width = cinfo.output_width;
        channel = cinfo.output_components;

        // Create a 3D array to store the pixels
        int*** pixels = new int** [height];
        for (int i = 0; i < height; i++) {
            pixels[i] = new int* [width];
            for (int j = 0; j < width; j++) {
                pixels[i][j] = new int[channel];
            }
        }

        // Allocate a buffer to hold the decompressed image data
        JSAMPLE* buffer = new JSAMPLE[width * channel];

        // Read the image data one scanline at a time
        while (cinfo.output_scanline < height) {
            jpeg_read_scanlines(&cinfo, &buffer, 1);

            // Copy the scanline data into the 3D array
            for (int j = 0; j < width; j++) {
                for (int c = 0; c < channel; c++) {
                    pixels[cinfo.output_scanline - 1][j][c] = buffer[j * channel + c];
                }
            }
        }

        // Clean up
        delete[] buffer;
        jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);
        fclose(fp);

        return pixels;
    }

   */
public:
    /// <summary>
    /// get image in grey scale as a 2d array
    /// </summary>
    /// <param name="vid"> Video capture instance</param>
    /// <param name="row"> rows</param>
    /// <param name="col">cols</param>
    /// <returns>greyScale image</returns>
    int** get_Grey(VideoCapture& vid,int&row,int &col);

    /// <summary>
    /// get image in greyscal Frame
    /// </summary>
    /// <param name="vid"> Video capture instance</param>
    /// <param name="frame"> frame of image</param>
    void  get_Grey(VideoCapture& vid,Mat& frame);

    /// <summary>
    /// get image in color as a 3d array
    /// </summary>
    /// <param name="vid"> Video capture instance</param>
    /// <param name="row"> rows</param>
    /// <param name="col">cols</param>
    /// <param name="channel">channels of image</param>
    /// <returns>colored image</returns>
    int*** get_Color(VideoCapture& vid, int& row, int& col,int&channel);

    /// <summary>
    /// get image in color as a Frame
    /// </summary>
    /// <param name="vid"> Video capture instance</param>
    /// <param name="frame"> frame of image</param>
    void   get_Color(VideoCapture& vid, Mat& frame);

    /// <summary>
    /// get image in color as a Frame
    /// </summary>
    /// <param name="vid"> Video capture instance</param>
    /// <param name="frame"> frame of image</param>
    /// <returns>color image</returns>
    Image get_Color(VideoCapture& vid);
    void get_Color(VideoCapture& vid,Image&img);
    /// <summary>
    /// get image in 3d array
    /// </summary>
    /// <param name="frame"> frmae of image</param>
    /// <returns>colr 3d image</returns>
    int*** convert_Color(Mat frame);
    
    /// <summary>
    /// get image in 2d array
    /// </summary>
    /// <param name="frame"> frmae of image</param>
    /// <returns>greyscal 2d image</returns>
    int**  convert_Grey(Mat frame);

    /// <summary>
    /// get image in 2d array
    /// </summary>
    /// <param name="arr"> image in 3d colr</param>
    /// <param name="x"> rows</param>
    /// <param name="y"> cols</param>
    /// <returns>greyscal image in 2d array</returns>
    int**  convert_Grey(int*** arr, int x, int y);

    /// <summary>
    /// get image in Mat frame
    /// </summary>
    /// <param name="arr"> image in 2d grey</param>
    /// <param name="x"> rows</param>
    /// <param name="y"> cols</param>
    /// <returns>Mat iamge </returns>
    Mat convert_Mat(int** arr, int x, int y);

    Mat convert_Mat(Image img) {
        return convert_Mat(img.getData(), img.getHeight(), img.getWidth());
    }



    /// <summary>
    /// get image in Mat frame
    /// </summary>
    /// <param name="arr"> image in 3d color</param>
    /// <param name="x"> rows</param>
    /// <param name="y"> cols</param>
    /// <returns>Mat iamge </returns>
    Mat convert_Mat(int*** arr, int x, int y);

    /// <summary>
    /// get image in Mat frame
    /// </summary>
    /// <param name="frame"> this image will be update in grey</param>
    void convert_Grey_Mat(Mat& frame);
};

/***

int*** convertToRGB(Mat frame) {
    int*** arr = new int** [frame.rows];
    for (int i = 0; i < frame.rows; i++) {
        arr[i] = new int* [frame.cols];
        for (int j = 0; j < frame.cols; j++) {
            arr[i][j] = new int[frame.channels()];
            uchar* pixel = frame.at<Vec3b>(i, j).val;
            arr[i][j][0] = pixel[2];
            arr[i][j][1] = pixel[1];
            arr[i][j][2] = pixel[0];
        }
    }
    return arr;
}
int*** convertToBGR(Mat frame) {
    int*** arr = new int** [frame.rows];
    for (int i = 0; i < frame.rows; i++) {
        arr[i] = new int* [frame.cols];
        for (int j = 0; j < frame.cols; j++) {
            arr[i][j] = new int[frame.channels()];
            uchar* pixel = frame.at<Vec3b>(i, j).val;
            for (int k = 0; k < frame.channels(); k++) {
                arr[i][j][k] = (int)pixel[k];
            }
        }
    }
    return arr;
}
int** convertMatToGrey(Mat frame) {
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
int** convertArrToGrey(int*** arr, int x, int y) {
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
int** convertToBlack(int*** arr1, int*** arr2, int x, int y, int z) {
    int** blackFrame = new int* [x];
    for (int i = 0; i < x; i++) {
        blackFrame[i] = new int[y];
        for (int j = 0; j < y; j++) {
            int* p1 = arr1[i][j];
            int* p2 = arr2[i][j];
            if (p1[0] == p2[0] && p1[1] == p2[1] && p1[2] == p2[2]) {
                blackFrame[i][j] = 0;
            }
            else
            {
                int white = p1[0] + p1[1] + p1[2];
                blackFrame[i][j] = white;
            }
        }
    }
    return blackFrame;
}
Mat convertToGrey(Mat frame) {
    for (int i = 0; i < frame.rows; i++) {
        for (int j = 0; j < frame.cols; j++) {
            uchar* p = frame.at<Vec3b>(i, j).val;
            int gray = ((int)p[0] * 0.3) + ((int)p[1] * 0.59) + ((int)p[2] * 0.11);
            for (int k = 0; k < frame.channels(); k++) {
                p[k] = gray;
            }
        }
    }
    return frame;
}
Mat convertToMat(int** arr, int x, int y) {
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
Mat convertToMat(int*** arr, int x, int y) {
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

*/