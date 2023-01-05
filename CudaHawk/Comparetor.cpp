#include "Comparetor.h"

bool Comparetor::checkInRange(int o, int n, int err) { return (abs(o - n) <= err);}
int  Comparetor::valueChanged(int n,int o, int po, int error) { 
    if (!checkInRange(o, n, error)) {
        if (!checkInRange(o, po, error)) {
            return UNCHANGED;
        }
        else {
            return FLIKER;
        }
    }
    return CHANGED;
}

int**  Comparetor::compareFrame_BW(int*** t, Mat cur) {   // return compared part
    int** arr = new int* [cur.rows];
    for (int i = 0; i < cur.rows; i++) {
        arr[i] = new int[cur.cols];
        for (int j = 0; j < cur.cols; j++) {
            int* oC = t[i][j];  
            uchar* nC = cur.at<Vec3b>(i, j).val; 
            if (checkInRange(oC[0],(int) nC[0], RANGE) && checkInRange(oC[1], (int)nC[1], RANGE) && checkInRange(oC[2], (int)nC[2], RANGE)) {
                arr[i][j] = WHITE;
            }
            else {
                arr[i][j] = BLACK;
            }
        }
    }
    return arr;
}
void Comparetor::compareFrame_Color(Mat& cur,int*** t ) {   // return compared part
    for (int i = 0; i < cur.rows; i++) {
        for (int j = 0; j < cur.cols; j++) {
            uchar* nC = cur.at<Vec3b>(i, j).val;   // BGR  new Colors
            for (int k = 0; k < cur.channels(); k++) {
                if (checkInRange(t[i][j][k], nC[k], RANGE)) {
                    nC[k] = BLACK;
                }
            }
        }
    }
}



void Comparetor::compareFrame_Color(Mat& cur, int*** thresh, int*** sthresh){
    for (int i = 0; i < cur.rows; i++) {
        for (int j = 0; j < cur.cols; j++) {
            uchar* nC = cur.at<Vec3b>(i, j).val;   // BGR  new Colors
            for (int k = 0; k < cur.channels(); k++) {
                if (checkInRange(thresh[i][j][k], nC[k], RANGE)) {

                }
                else {
                    nC[k] = BLACK;
                }
            }
        }
    }
}

void Comparetor::objectify(int*** cur, int*** thresh, int*** firstThresh,int row,int col,int channel) {
    bool change;
    int* arr = new int[channel] {};
    for (int r = 0; r < row; r++) {
        for (int c = 0; c < col; c++) {
            for (int cc = 0; cc < channel; cc++) {
                arr[cc]= checkInRange(cur[r][c][cc], thresh[r][c][cc], RANGE);
            }

            if (!Base().mostFrequent(arr, channel)) {

                for (int cc = 0; cc < channel; cc++) {
                    arr[cc] = checkInRange(thresh[r][c][cc], firstThresh[r][c][cc], RANGE);
                }

                if (Base().mostFrequent(arr, channel)) {
                    for (int cc = 0; cc < channel; cc++) {
                        cur[r][c][cc] = BLACK;
                    }
                }

            }
            else {
                for (int cc = 0; cc < channel; cc++) {
                    cur[r][c][cc] = BLACK;
                }
            }           
        }
    }
}
Mat Comparetor::objectify(Mat cur, Mat thresh, Mat firstThresh) {
    Mat img(cur.rows, cur.cols, 16);
    uchar* in, * io,*i;
    for (int r = 0; r <cur. rows; r++) {
        for (int c = 0; c < cur.cols; c++) {
            in = cur.at<Vec3b>(r, c).val;
            io = thresh.at<Vec3b>(r, c).val;
            i = img.at<Vec3b>(r, c).val;
            if (!checkInRange(in[0], (int)io[0], RANGE) && !checkInRange(in[1], (int)io[1], RANGE) && !checkInRange(in[2], (int)io[2], RANGE)) {
                i[0] = (uchar)WHITE;
                i[1] = (uchar)WHITE;
                i[2] = (uchar)WHITE;
            }
            else {
                i[0] = (uchar)BLACK;
                i[1] = (uchar)BLACK;
                i[2] = (uchar)BLACK;
            }
            /*
            if (Base().maxCount(arr, cur.channels(),true)<cur.channels()/2) {
                //ifo = firstThresh.at<Vec3b>(r, c).val;
                //
                //for (int cc = 0; cc < cur.channels(); cc++) {
                //    arr[cc] = checkInRange(io[cc], ifo[cc], RANGE);
                //}
                //
                //if (Base().mostFrequent(arr, cur.channels())) {
                //    for (int cc = 0; cc < cur.channels(); cc++) {
                //        in[cc] = (uchar)BLACK;
                //    }
                //}

            }
            else {
                for (int cc = 0; cc < cur.channels(); cc++) {
                    in[cc] = (uchar)BLACK;
                }
            }  
            */
        }
    }
    return img;
}



void Comparetor::compareFrame_BW(Mat& cur,Mat prev) {   // cur will be updated 
    for (int i = 0; i < cur.rows; i++) {
        for (int j = 0; j < cur.cols; j++) {
            uchar* nC = cur.at<Vec3b>(i, j).val;  
            uchar* oC = prev.at<Vec3b>(i, j).val;   
            if (checkInRange(oC[0], nC[0], RANGE) && checkInRange(oC[1], nC[1], RANGE) && checkInRange(oC[2], nC[2], RANGE)) {
                nC[0] = WHITE;
                nC[1] = WHITE;
                nC[2] = WHITE;
            }
            else {
                nC[0] = BLACK;
                nC[1] = BLACK;
                nC[2] = BLACK;
            }
        }
    }
}
void Comparetor::compareFrame_BW(int** cur, int** prev, int x, int y) {   // cur will be updated 
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            if (checkInRange(cur[i][j], prev[i][j], RANGE)) {

                cur[i][j] = WHITE;
            }
            else {
                cur[i][j] = BLACK;
            }
        }
    }
}
void Comparetor::compareFrame_BW(int*** cur, int*** prev, int x, int y, int z) {
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            if (checkInRange(cur[i][j][0], prev[i][j][0], RANGE)
                && checkInRange(cur[i][j][1], prev[i][j][1], RANGE)
                && checkInRange(cur[i][j][2], prev[i][j][2], RANGE)) {
                cur[i][j][0] = WHITE;
                cur[i][j][1] = WHITE;
                cur[i][j][2] = WHITE;
            }
            else {
                cur[i][j][0] = BLACK;
                cur[i][j][1] = BLACK;
                cur[i][j][2] = BLACK;
            }
        }
    }
}



void Comparetor::compareFrame_BW(Mat& cur, Mat prev,Mat pprev) {   // previous will be updated 
    for (int i = 0; i < cur.rows; i++) {
        for (int j = 0; j < cur.cols; j++) {
            uchar* nC = cur.at<Vec3b>(i, j).val;
            uchar* oC = prev.at<Vec3b>(i, j).val;
            uchar* poC = pprev.at<Vec3b>(i, j).val;
            if (checkInRange(poC[0], nC[0], RANGE) && checkInRange(poC[1], nC[1], RANGE) && checkInRange(poC[2], nC[2], RANGE)) {

                if (checkInRange(oC[0], nC[0], RANGE) && checkInRange(oC[1], nC[1], RANGE) && checkInRange(oC[2], nC[2], RANGE)) {
                    nC[0] = 0;
                    nC[1] = 0;
                    nC[2] = 0;

                }
                else {
                    nC[0] = 255;
                    nC[1] = 255;
                    nC[2] = 255;
                  
                }
                
            }
            else {
                nC[0] = 0;
                nC[1] = 0;
                nC[2] = 0;
            }

        }
    }
}


Mat Comparetor::difference(const Mat& f1, const Mat& f2){
    if (f1.channels() != f2.channels())
        return f1;

    int row = fmin(f1.rows, f2.rows);
    int col = fmin(f1.cols, f2.cols);
    Mat fr = Mat(row, col, 16);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            uchar *px=fr.at<Vec3b>(i, j).val;
            for (int k = 0; k < f1.channels(); k++) {
                int dif = abs((int)f1.at<Vec3b>(i, j).val[k]-(int) f2.at<Vec3b>(i, j).val[k]);
                if (dif > RANGE)
                    px[k] = f2.at<Vec3b>(i, j).val[k];
                else
                    px[k] = f1.at<Vec3b>(i, j).val[k];
            }
        }
    }
    return fr;
}
void Comparetor::difference_Grey(const Mat& f1, Mat& f2) {
    if (f1.channels() != f2.channels())
        return;

    int row = fmin(f1.rows, f2.rows);
    int col = fmin(f1.cols, f2.cols);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            uchar* px = f2.at<Vec3b>(i, j).val;
            for (int k = 0; k < f1.channels(); k++) {
                int dif = abs((int)f1.at<Vec3b>(i, j).val[k] -(int) px[k]);
                if (dif > RANGE)
                    px[k] = (uchar)0;
                else
                    px[k] =(uchar)255;
            }
        }
    }
}
int** Comparetor::diffrenceBW(const Mat& f1,const Mat& f2){
    if (f1.channels() != f2.channels())
        return nullptr;

    int row = fmin(f1.rows, f2.rows);
    int col = fmin(f1.cols, f2.cols);
    int** fr = new int* [row];
    for (int i = 0; i < row; i++) {
        fr[i] = new int[col];
        for (int j = 0; j < col; j++) {
            for (int k = 0; k < f1.channels(); k++) {
                int dif = abs((int)f1.at<Vec3b>(i, j).val[k]- (int) f2.at<Vec3b>(i, j).val[k]);
                if (dif > RANGE)
                    fr[i][j] = WHITE;
                else
                    fr[i][j] = BLACK;
            }
        }
    }
    return fr;
}
int*** Comparetor::diffrenceColor(const Mat& f1,const Mat& f2){
    if (f1.channels() != f2.channels())
        return nullptr;

    int row = fmin(f1.rows, f2.rows);
    int col = fmin(f1.cols, f2.cols);
    int*** fr = new int** [row];
    for (int i = 0; i < row; i++) {
        fr[i] = new int*[col];
        for (int j = 0; j < col; j++) {
            fr[i][j] = new int [f1.channels()]; 
            for (int k = 0; k < f1.channels(); k++) {
                int dif = abs((int)f1.at<Vec3b>(i, j).val[k]-(int) f2.at<Vec3b>(i, j).val[k]);
                fr[i][j][k] = dif;
            }
        }
    }
    return fr;
}


