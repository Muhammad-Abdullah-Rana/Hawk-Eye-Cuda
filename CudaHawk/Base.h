#pragma once
# include <iostream>
# include <String>
#include <vector>

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

#define THRESH_IMG 15

#define RANGE 5

#define BLACK 0
#define WHITE 255
#define MIDDEL 127

#define UNCHANGED  -1
#define CHANGED   0
#define FLIKER 1

#define OBJECT_MAX_GAP 3
#define OBJECT_MIN_PXS 150
#define PERSON_MIN_WIDTH 4
#define PERSON_MAX_WIDTH 40
#define PERSON_MIN_HEIGHT 20
#define PERSON_MAX_HEIGHT 60


#define PERSON_WIDTH 4
#define PERSON_HEIGHT 35

#define BITE_BOUND(val) val< 0 ? 0 : val>255?255:val

class Base {
public:

    int mode(vector<int> arr) {
        std::sort(arr.begin(), arr.end());

        //finding max frequency  
        int max_count = 1, res = arr.at(0), count = 1;
        for (int i = 1; i < arr.size(); i++) {
            if (arr.at(i) == arr.at(i - 1))
                count++;
            else {
                if (count > max_count) {
                    max_count = count;
                    res = arr.at(i - 1);
                }
                count = 1;
            }
        }

        // when the last element is most frequent 
        if (count > max_count)
        {
            max_count = count;
            res = arr.at(arr.size() - 1);
        }

        return res;
    }



    /// <summary>
    /// 
    /// </summary>
    /// <param name="arr"> Array of integar values </param>
    /// <param name="size"> Size of the array</param>
    /// <returns> mean of array</returns>
    int mean(int* arr, int n);
    /// <summary>
    /// 
    /// </summary>
    /// <param name="arr"> Array of integar values </param>
    /// <param name="size"> Size of the array</param>
    /// <returns> median of array</returns>
    int median(int* arr, int n);
    /// <summary>
    /// 
    /// </summary>
    /// <param name="arr"> Array of integar values </param>
    /// <param name="size"> Size of the array</param>
    /// <returns> mode of array</returns>
    int mode(int* arr, int size);
    /// <summary>
    /// 
    /// </summary>
    /// <param name="arr"> Array of integar values </param>
    /// <param name="size"> Size of the array</param>
    /// <param name="lastMode"> last calculted mode  for comparison if '-1', will be ignored</param>
    /// <returns> mode of array</returns>
    int modeOfArr(int* arr, int size, int lastMode);
    /// <summary>
    /// 
    /// </summary>
    /// <param name="arr"> Array of integar values </param>
    /// <param name="size"> Size of the array</param>
    /// <returns> Most repeated number in array</returns>
    int mostFrequent(int* arr, int size);
    /// <summary>
    /// Count max time a number is reptead in an array
    /// </summary>
    /// <param name="arr"> Array of integar values </param>
    /// <param name="size"> Size of the array</param>
    /// <param name="val"> value to be checked</param>
    /// <returns> count of val in array</returns>
    int maxCount(int* arr, int size,int val);
    
    void destroy(int*** arr, int x, int y);

};
