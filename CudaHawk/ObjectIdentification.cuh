#pragma once

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>

#include "Base.h"
#include "Object.h"

/// <summary>
/// Travers row and check all 255/ white color bounds
/// </summary>
/// <param name="RGBImage"> black white RGBImage of 2d array in 1d row*col </param>
/// <param name="size">size of RGBImage</param>
/// <param name="rows">toatal rows in RGBImage</param>
/// <param name="cols">total cols in RGBImage</param>
/// <param name="ref">rows ref for active nodes to return or update</param>
/// <returns></returns>


#define block_width 32
#define cuda_check_errors(val) check( (val), #val, __FILE__, __LINE__)

using namespace cv;
using namespace std;


#define BLOCK_WIDTH 32
#define cuda_check_errors(val) check( (val), #val, __FILE__, __LINE__)
template<typename T>

void check(T err, const char* const func, const char* const file,
    const int line) {
    if (err != cudaSuccess) {
        cerr << "CUDA error at: " << file << ":" << line << endl;
        cerr << cudaGetErrorString(err) << " " << func << endl;
        exit(1);
    }
}

__global__ void lightSegmentation(uchar4* const d_rgba, size_t rows, size_t cols, const int blvl) {
    size_t j = blockIdx.y * blockDim.y + threadIdx.y;
    size_t i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i >= rows || j >= cols)
        return;

    uchar4 p = d_rgba[i * cols + j];
    unsigned char gray = (unsigned char)(0.299f * p.x + 0.587f * p.y + 0.114f * p.z);
    if (gray >= blvl)
        d_rgba[i * cols + j].x = d_rgba[i * cols + j].y = d_rgba[i * cols + j].z = 0;
}

void lightSegmentationLauncher(uchar4* const d_rgba, size_t rows, size_t cols, const int blvl) {
    const dim3 block_size(BLOCK_WIDTH, BLOCK_WIDTH, 1);
    unsigned int grid_x = (unsigned int)(rows / BLOCK_WIDTH + 1);
    unsigned int grid_y = (unsigned int)(cols / BLOCK_WIDTH + 1);
    const dim3 grid_size(grid_x, grid_y, 1);
    lightSegmentation<<<grid_size, block_size>>>(d_rgba, rows, cols, blvl);
    cudaDeviceSynchronize();
    cuda_check_errors(cudaGetLastError());
}

size_t numPixels(Mat image) { return image.cols * image.rows; }



void getPointers(uchar4** h_rgba, uchar4** d_rgba,  Mat RGBImage) {
    cuda_check_errors(cudaFree(0));

    *h_rgba = (uchar4*)RGBImage.ptr<unsigned char>(0);

    const size_t np = numPixels(RGBImage);
    cuda_check_errors(cudaMalloc(d_rgba, sizeof(uchar4) * np));
    cuda_check_errors(cudaMemcpy(*d_rgba, *h_rgba, sizeof(uchar4) * np, cudaMemcpyHostToDevice));
}

cudaError_t cudaFitTransform(Mat& BRGImage, const int blvl) {
    cudaError_t cudaStatus;
    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
        //goto ERROR;
    }

    uchar4* h_rgba, * d_rgba;
    cvtColor(BRGImage, BRGImage, COLOR_BGR2BGRA);

    if (!BRGImage.isContinuous()) {
        cerr << "RGB image isn't continuous!! Exiting." << endl;
        exit(1);
    }


    getPointers(&h_rgba, &d_rgba, BRGImage);

    lightSegmentationLauncher(d_rgba, BRGImage.rows, BRGImage.cols, blvl);
    cudaDeviceSynchronize();
    cuda_check_errors(cudaGetLastError());

    cuda_check_errors(cudaMemcpy(h_rgba, d_rgba, sizeof(uchar4) * numPixels(BRGImage), cudaMemcpyDeviceToHost));

    /*uchar4* RGBImagePointer = 0;
    lightSegmentation(RGBImage, &RGBImagePointer, blvl);*/
    /*int gray = 0;
    ORow* rows = new ORow[RGBImage.rows];
    for (int r = 0; r < RGBImage.rows; r++) {
        rows[r] = ORow(r);
        for (int c = 0; c < RGBImage.cols; c++) {
            gray = (0.2989 * (int)RGBImage.at<Vec3b>(r, c)[0]) + (0.5870 * (int)RGBImage.at<Vec3b>(r, c)[1]) + (0.1140 * (int)RGBImage.at<Vec3b>(r, c)[2]);
            if (gray < blvl) rows[r].add(c);
            else {
                RGBImage.at<Vec3b>(r, c) = (Vec3b)((uchar)0, (uchar)0, (uchar)0);
            }
        }
    }*/
}
