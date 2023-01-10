#pragma once

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>

#include "Base.h"
#include "Object.cuh"

/// <summary>
/// Travers row and check all 255/ white color bounds
/// </summary>
/// <param name="img"> black white img of 2d array in 1d row*col </param>
/// <param name="size">size of img</param>
/// <param name="rows">toatal rows in img</param>
/// <param name="cols">total cols in img</param>
/// <param name="ref">rows ref for active nodes to return or update</param>
/// <returns></returns>
/// 


__global__ void lightKernel(const int blvl)
{
    int i = threadIdx.x;
    //c[i] = a[i] + b[i];
    //rows[i] = ORow(i);

    //cout << (int)(*tempMat).at<Vec3b>(i, 0)[0];
    int gray = 0;
    /*for (int c = 0; c < (*tempMat).cols; c++) {
        *gray = (0.2989 * (int)(*tempMat).at<Vec3b>(i, c)[0]) + (0.5870 * (int)(*tempMat).at<Vec3b>(i, c)[1]) + (0.1140 * (int)(*tempMat).at<Vec3b>(i, c)[2]);
        if (*gray < *blvl) rows[i].add(c);
        else {
            (*tempMat).at<Vec3b>(i, c) = (Vec3b)((uchar)0, (uchar)0, (uchar)0);
        }
    }*/
}
static inline void _safe_cuda_call(cudaError err, const char* msg, const char* file_name, const int line_number) {
    if (err != cudaSuccess) {
        fprintf(stderr, "%s\n\nFile: %s\n\nLine Number: %d\n\nReason: %s\n", msg, file_name, line_number, cudaGetErrorString(err));
        std::cin.get();
        exit(EXIT_FAILURE);
    }
}
__global__ void bgr_to_gray_kernel(unsigned char* input, unsigned char* output, int width, int height, int colorWidthStep, int grayWidthStep) {
    //2D Index of current thread
    const int xIndex = blockIdx.x * blockDim.x + threadIdx.x;
    const int yIndex = blockIdx.y * blockDim.y + threadIdx.y;

    //Only valid threads perform memory I/O
    if ((xIndex < width) && (yIndex < height))
    {
        //Location of colored pixel in input
        const int color_tid = yIndex * colorWidthStep + (3 * xIndex);

        //Location of gray pixel in output
        const int gray_tid = yIndex * grayWidthStep + xIndex;

        const unsigned char blue = input[color_tid];
        const unsigned char green = input[color_tid + 1];
        const unsigned char red = input[color_tid + 2];

        const float gray = red * 0.3f + green * 0.59f + blue * 0.11f;

        output[gray_tid] = static_cast<unsigned char>(gray);
    }
}
#define SAFE_CALL(call,msg) _safe_cuda_call((call),(msg),__FILE__,__LINE__)
cudaError_t lightSegmentation(Mat& img, const int blvl) {
    Mat output;
    const int colorBytes = img.step * img.rows;
    const int grayBytes = output.step * output.rows;

    unsigned char* d_input, * d_output;

    // Allocate device memory
    SAFE_CALL(cudaMalloc<unsigned char>(&d_input, colorBytes), "CUDA Malloc Failed");
    SAFE_CALL(cudaMalloc<unsigned char>(&d_output, grayBytes), "CUDA Malloc Failed");

    // Copy data from OpenCV input image to device memory
    SAFE_CALL(cudaMemcpy(d_input, img.ptr(), colorBytes, cudaMemcpyHostToDevice), "CUDA Memcpy Host To Device Failed");

    // Specify a reasonable block size
    const dim3 block(16, 16);

    // Calculate grid size to cover the whole image
    const dim3 grid((img.cols + block.x - 1) / block.x, (img.rows + block.y - 1) / block.y);

    // Launch the color conversion kernel
    bgr_to_gray_kernel << <grid, block >> > (d_input, d_output, img.cols, img.rows, img.step, output.step);

    // Synchronize to check for any kernel launch errors
    cudaDeviceSynchronize();

    // Copy back data from destination device meory to OpenCV output image
    SAFE_CALL(cudaMemcpy(output.ptr(), d_output, grayBytes, cudaMemcpyDeviceToHost), "CUDA Memcpy Host To Device Failed");

    // Free the device memory
    SAFE_CALL(cudaFree(d_input), "CUDA Free Failed");
    SAFE_CALL(cudaFree(d_output), "CUDA Free Failed");
    imshow("vid", output);
    waitKey(5);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //cudaError_t cudaStatus;
    //ORow *rows = 0;
    //Mat *tempMat = 0;
    //int *tempBlvl = 0;

    //// Allocate GPU buffers for three vectors (two input, one output)
    //cudaStatus = cudaMalloc((void**)&tempMat, sizeof(Mat));
    //if (cudaStatus != cudaSuccess) {
    //    fprintf(stderr, "cudaMalloc failed!");
    //    return cudaStatus;
    //}
    //cudaStatus = cudaMalloc((void**)&rows, img.rows * sizeof(ORow));
    //if (cudaStatus != cudaSuccess) {
    //    fprintf(stderr, "cudaMalloc failed!");
    //    return cudaStatus;
    //}
    //cudaStatus = cudaMalloc((void**)&tempBlvl, sizeof(int));
    //if (cudaStatus != cudaSuccess) {
    //    fprintf(stderr, "cudaMalloc failed!");
    //    return cudaStatus;
    //}
    //cudaMalloc<
    //lightKernel<<<1, img.rows>>>(*tempBlvl);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /*int grayI = 0;
    ORow* rowsI = new ORow[img.rows];
    for (int r = 0; r < img.rows; r++) {
        rowsI[r] = ORow(r);
        for (int c = 0; c < img.cols; c++) {
            grayI = (0.2989 * (int)img.at<Vec3b>(r, c)[0]) + (0.5870 * (int)img.at<Vec3b>(r, c)[1]) + (0.1140 * (int)img.at<Vec3b>(r, c)[2]);
            if (grayI < blvl) rowsI[r].add(c);
            else {
                img.at<Vec3b>(r, c) = (Vec3b)((uchar)0, (uchar)0, (uchar)0);
            }
        }
    }*/

    return cudaSuccess;
}



cudaError_t cuda_fit_transform(Mat& img, const int blvl) {


    cudaError_t cudaStatus;
    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
        //goto ERROR;
    }

    lightSegmentation(img, blvl);

    //int gray = 0;
    //ORow* rows = new ORow[img.rows];
    //for (int r = 0; r < img.rows; r++) {
    //    rows[r] = ORow(r);
    //    for (int c = 0; c < img.cols; c++) {
    //        gray = (0.2989 * (int)img.at<Vec3b>(r, c)[0]) + (0.5870 * (int)img.at<Vec3b>(r, c)[1]) + (0.1140 * (int)img.at<Vec3b>(r, c)[2]);
    //        if ( gray < blvl ) rows[r].add(c);
    //        else {
    //            img.at<Vec3b>(r, c) = (Vec3b)((uchar)0, (uchar)0, (uchar)0);
    //        }
    //    }
    //}


 /*
    // assigning ids
    cudaStatus = cudaMalloc((void**)&rows, img.rows * sizeof(ORow));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto ERROR;
    }


 */

 //for (int r = 0; r < img.rows; r++) {
 //    for (int c = 0; c < rows[r].getSize(); c++) {
 //        for(int b=0;b<rows[r].getBound(c).width();b++)
 //        if (!rows[r].hasIt(c)) {
 //            img.at<Vec3b>(r, c) = (Vec3b)((uchar)0, (uchar)0, (uchar)0);
 //        }
 //    }
 //}

ERROR:
    return cudaStatus;
}

/*
// Helper function for using CUDA to add vectors in parallel.
cudaError_t addWithCuda(int* c, const int* a, const int* b, unsigned int size)
{
    int* dev_a = 0;
    int* dev_b = 0;
    int* dev_c = 0;
    cudaError_t cudaStatus;

    // Choose which GPU to run on, change this on a multi-GPU system.
    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
        goto Error;
    }

    // Allocate GPU buffers for three vectors (two input, one output)    .
    cudaStatus = cudaMalloc((void**)&dev_c, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_a, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_b, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    // Copy input vectors from host memory to GPU buffers.
    cudaStatus = cudaMemcpy(dev_a, a, size * sizeof(int), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

    cudaStatus = cudaMemcpy(dev_b, b, size * sizeof(int), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

    // Launch a kernel on the GPU with one thread for each element.
    addKernel << <1, size >> > (dev_c, dev_a, dev_b);

    // Check for any errors launching the kernel
    cudaStatus = cudaGetLastError();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
        goto Error;
    }

    // cudaDeviceSynchronize waits for the kernel to finish, and returns
    // any errors encountered during the launch.
    cudaStatus = cudaDeviceSynchronize();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching addKernel!\n", cudaStatus);
        goto Error;
    }

    // Copy output vector from GPU buffer to host memory.
    cudaStatus = cudaMemcpy(c, dev_c, size * sizeof(int), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

Error:
    cudaFree(dev_c);
    cudaFree(dev_a);
    cudaFree(dev_b);

    return cudaStatus;
}
*/
