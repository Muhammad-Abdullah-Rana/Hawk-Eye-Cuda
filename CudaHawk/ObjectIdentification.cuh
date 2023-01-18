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

__global__ void lightSegmentation(uchar4* const d_rgba, unsigned char* d_grey, size_t rows, size_t cols, const int blvl) {
    size_t j = blockIdx.y * blockDim.y + threadIdx.y;
    size_t i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i >= rows || j >= cols)
        return;

    uchar4 p = d_rgba[i * cols + j];
    if ((unsigned char)(0.299f * p.x + 0.587f * p.y + 0.114f * p.z) < blvl) {
        d_grey[i * cols + j] = 255;
    }
    else {
        d_rgba[i * cols + j].x = d_rgba[i * cols + j].y = d_rgba[i * cols + j].z = 0;
        d_grey[i * cols + j] = 0;
    }
}

void lightSegmentationLauncher(uchar4* const d_rgba, unsigned char* d_grey,size_t rows, size_t cols, const int blvl) {
    const dim3 blockSize(BLOCK_WIDTH, BLOCK_WIDTH, 1);
    unsigned int gridX = (unsigned int)(rows / BLOCK_WIDTH + 1);
    unsigned int gridY = (unsigned int)(cols / BLOCK_WIDTH + 1);
    const dim3 gridSize(gridX, gridY, 1);
    lightSegmentation<<<gridSize, blockSize>>>(d_rgba, d_grey, rows, cols, blvl);
    cudaDeviceSynchronize();
    cuda_check_errors(cudaGetLastError());
}

void createArrayList(unsigned char**** array, const int grid) {
    *array = new unsigned char** [grid];
    for (int i = 0; i < grid; i++) {
        *array[i] = new unsigned char* [BLOCK_WIDTH];
    }
}
void initCudaArrayPointer(unsigned char ****arrLst, unsigned char ****arrPtr) {
    *arrPtr = *arrLst;
    cuda_check_errors(cudaMalloc(arrPtr, sizeof(unsigned int***)));
    //cuda_check_errors(cudaMemset(*d_grey, 0, sizeof(unsigned char) * np));
    //cuda_check_errors(cudaMemcpy(*d_rgba, *h_rgba, sizeof(uchar4) * np, cudaMemcpyHostToDevice));*/
}

__global__ void objectDetection(unsigned char* d_grey, size_t rows, size_t cols) {
    size_t i = (threadIdx.x + blockIdx.x * 32) * 4;
    if (i >= rows)
        return;
    int curr = 0;
    int* arr = NULL;
    int* temp = NULL;
    int arrSize = 0;
    for (int j = 0; j < 4; j++) {
        for (int k = 0; k < cols; k++) {
            if (curr != 0) {
                if (!d_grey[i + j * cols + k] == 0) {
                    temp = new int[(arrSize + 1) * 2];
                    for (int s = 0; s < arrSize; i++) {
                        for (int w = 0; w < 2; j++) {
                            *(temp + s * 2 + w) = *(arr + s * 2 + w);
                        }
                    }
                    delete(arr);
                    arr = temp;
                    *(arr + arrSize * 2) = curr;
                    *(arr + arrSize * 2+1) = d_grey[i + j * cols + k-1];
                    curr = 0;
                }
                else if (k + 1 == cols)
                    curr = i + j * cols + k;
                else;
            }
            else {
                if (!d_grey[i + j * cols + k] == 0);
                curr = i + j * cols + k;

            }
        }
    }
    /*uchar4 p = d_grey[i * cols + j];
    if ((unsigned char)(0.299f * p.x + 0.587f * p.y + 0.114f * p.z) < 0) {
        d_grey[i * cols + j] = 255;
    }
    else {
        d_grey[i * cols + j] = 0;
    }*/
}

void objectDetectionLauncher(unsigned char *d_grey, size_t rows, size_t cols) {
    int grid = (rows / 4) / BLOCK_WIDTH + 1;
    /*unsigned char*** arrList, arrPtr;
    createArrayList(&arrList, grid);
    initCudaArrayPointer(&arrList, &arrPtr);*/
    objectDetection<<<grid, BLOCK_WIDTH >>> (d_grey, rows, cols);
    cudaDeviceSynchronize();
    cuda_check_errors(cudaGetLastError());
}

size_t numPixels(Mat image) { return image.cols * image.rows; }
void getPointers(uchar4 **h_rgba, uchar4 **d_rgba, unsigned char** h_grey, unsigned char **d_grey, Mat BRGImage, Mat greyImage) {
    cuda_check_errors(cudaFree(0));

    *h_rgba = (uchar4*)BRGImage.ptr<unsigned char>(0);
    *h_grey = greyImage.ptr<unsigned char>(0);

    const size_t np = numPixels(BRGImage);
    cuda_check_errors(cudaMalloc(d_rgba, sizeof(uchar4) * np));
    cuda_check_errors(cudaMalloc(d_grey, sizeof(unsigned char) * np));
    cuda_check_errors(cudaMemset(*d_grey, 0, sizeof(unsigned char) * np));
    cuda_check_errors(cudaMemcpy(*d_rgba, *h_rgba, sizeof(uchar4) * np, cudaMemcpyHostToDevice));
}

cudaError_t cudaFitTransform(Mat& BRGImage, const int blvl) {
    cudaError_t cudaStatus = cudaSuccess;
    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
        //goto ERROR;
        return cudaStatus;
    }

    cvtColor(BRGImage, BRGImage, COLOR_BGR2BGRA);
    if (!BRGImage.isContinuous()) {
        cerr << "RGB image isn't continuous!! Exiting." << endl;
        return cudaStatus;
    }

    Mat greyImage;

    greyImage.create(BRGImage.rows, BRGImage.cols, CV_8UC1);
    if (!greyImage.isContinuous()) {
        cerr << "Grey image isn't continuous!! Exiting." << endl;
        exit(1);
    }

    uchar4 *h_rgba, *d_rgba;
    unsigned char* h_grey, * d_grey;

    getPointers(&h_rgba, &d_rgba, &h_grey, &d_grey, BRGImage, greyImage);
    
    lightSegmentationLauncher(d_rgba, d_grey, BRGImage.rows, BRGImage.cols, blvl);
    cudaDeviceSynchronize();
    cuda_check_errors(cudaGetLastError());

    objectDetectionLauncher(d_grey, BRGImage.rows, BRGImage.cols);
    cudaDeviceSynchronize();
    cuda_check_errors(cudaGetLastError());

    cuda_check_errors(cudaMemcpy(h_rgba, d_rgba, sizeof(uchar4) * numPixels(BRGImage), cudaMemcpyDeviceToHost));
    cuda_check_errors(cudaMemcpy(h_grey, d_grey, sizeof(unsigned char) * numPixels(BRGImage), cudaMemcpyDeviceToHost));

    imshow("grey", greyImage);
    cudaFree(d_rgba);
    cudaFree(d_grey);
    return cudaStatus;
}
