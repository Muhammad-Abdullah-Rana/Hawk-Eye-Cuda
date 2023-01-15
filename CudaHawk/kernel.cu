#pragma once
#include "Segmentation.h"
#include "ObjectIdentification.cuh"

void cudaCall(VideoCapture& vid) {
    Mat img;
    cudaError_t cudaStatus;
    vid.read(img);
    while (!img.empty()) {
        cudaStatus = cudaFitTransform(img, 30);
        if (cudaStatus != cudaSuccess) {
            fprintf(stderr, "cuda failed!");
            return;
        }
        imshow("vid", img);
        waitKey(5);
        vid.read(img);
    }
}

int main()
{
    VideoCapture vid("vid001.y4m");
    cudaCall(vid);
    
    // cudaDeviceReset must be called before exiting in order for profiling and
    // tracing tools such as Nsight and Visual Profiler to show complete traces.
    cudaError_t cudaStatus = cudaDeviceReset();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceReset failed!");
        return 1;
    }

    return 0;
}
