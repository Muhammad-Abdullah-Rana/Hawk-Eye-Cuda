#include "Base.h"

int Base::mode(int* arr, int size) {
    sort(arr, arr + size);

    //finding max frequency  
    int max_count = 1, res = arr[0], count = 1;
    for (int i = 1; i < size; i++) {
        if (arr[i] == arr[i - 1])
            count++;
        else {
            if (count > max_count) {
                max_count = count;
                res = arr[i - 1];
            }
            count = 1;
        }
    }

    // when the last element is most frequent 
    if (count > max_count)
    {
        max_count = count;
        res = arr[size - 1];
    }

    return res;
}
int Base::modeOfArr(int* arr, int size, int lastMode) { // mode or median if not repeated values

    int curMode = mode(arr,size);
    if (lastMode == -1)
        return curMode;
    if (lastMode != curMode) {
        if (lastMode > curMode) {
            if ((lastMode - 5) > curMode) {
                return curMode;
            }
        }
        else {
            if ((lastMode + 5) < curMode) {
                return curMode;
            }
        }
    }
    return lastMode;
}


int Base::mostFrequent(int* arr, int size) {
    int res = 0;
    int count = 1;
    for (int i = 1; i < size; i++) {
        if (arr[i] == arr[res])
            count++;
        else
            count--;
        if (count == 0) {
            res = i;
            count = 1;
        }
    }
    return arr[res];
}
int Base::maxCount(int* arr, int size,int val) {
    int count = 0;
    for (int i = 1; i < size; i++) {
        if (arr[i] == val)
            count++;
    }
    return count;
}
int Base::mean(int* arr, int n) {
    int sum = 0;
    for (int i = 0; i < n; i++)
        sum += arr[i];

    return sum / n;
}
int Base::median(int* arr, int n) {
    sort(arr, arr + n);
    if (n % 2 == 0)
        return (arr[n / 2 - 1] + arr[n / 2]) / 2;
    return arr[n / 2];
}
void Base::destroy(int*** arr, int x, int y) {
    if (arr != nullptr) {
        for (int i = 0; i < x; i++) {
            if (arr[i] != nullptr) {
                for (int j = 0; j < y; j++) {
                    if (arr[i][j] != nullptr)
                        delete arr[i][j];
                }
                delete arr[i];
            }
        }
        delete[]arr;
    }
    

    
}
