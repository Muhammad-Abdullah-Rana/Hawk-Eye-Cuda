#pragma once

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>

#include "Base.h"
#include "Object.h"

/// <summary>
/// Travers row and check all 255/ white color bounds
/// </summary>
/// <param name="img"> black white img of 2d array in 1d row*col </param>
/// <param name="size">size of img</param>
/// <param name="rows">toatal rows in img</param>
/// <param name="cols">total cols in img</param>
/// <param name="ref">rows ref for active nodes to return or update</param>
/// <returns></returns>

cudaError_t cuda_fit_transform(Mat& img,const int blvl) {

    cudaError_t cudaStatus;
    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
        //goto ERROR;
    }

    int gray = 0;
    ORow* rows = new ORow[img.rows];
    for (int r = 0; r < img.rows; r++) {
        rows[r] = ORow(r);
        for (int c = 0; c < img.cols; c++) {
            gray = (0.2989 * (int)img.at<Vec3b>(r, c)[0]) + (0.5870 * (int)img.at<Vec3b>(r, c)[1]) + (0.1140 * (int)img.at<Vec3b>(r, c)[2]);
            if ( gray < blvl ) rows[r].add(c);
            else {
                img.at<Vec3b>(r, c) = (Vec3b)((uchar)0, (uchar)0, (uchar)0);
            }
        }
    }


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
