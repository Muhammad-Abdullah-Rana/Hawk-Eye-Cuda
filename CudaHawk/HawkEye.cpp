#include "Base.h"
#include "Segmentation.h"
#include "ObjectIdentification.h"
#include "Converter.h"





// Pevious edition code not being used now
/*

#include "Cleaning.h"

#include "Comparetor.h"
#include "GreyImage.h"
#include "HImage.h"
#include "Header.h"


void outputMat(int** val, int x, int y) {
    Mat newMat = Mat(x, y, 16);
    for (int i = 0; i < x; i++){
        for (int j = 0; j < y; j++) {
            newMat.at<Vec3b>(i, j).val[0] = val[i][j];
            newMat.at<Vec3b>(i, j).val[1] = val[i][j];
            newMat.at<Vec3b>(i, j).val[2] = val[i][j];
        }
    }
    imshow("name", newMat);
    //For pics
    //waitKey(0);
    //For vid
    waitKey(10);
}
int** averageArrays(int*** arr, int x, int y) {
    int** averageFrame = new int* [x];
    int modePixel, modePixelCount, currPixel, currPixelCount, currFrame;
    bool isInVec;
    int visitedFrames[6];
    for (int i = 0; i < x; i++) {
        averageFrame[i] = new int[y];
        for (int j = 0; j < y; j++) {
            //empties the visited array
            for (int k = 0; k < 6; k++)
                visitedFrames[i] = -1;
            modePixel = currPixel = arr[0][i][j];
            modePixelCount = currPixelCount = 1;
            currFrame = 0;
            isInVec = false;

            for (int currFrame = 0; currFrame < 6; currFrame++) {
                for (int currVecI = 0; currVecI < 6; currVecI) {
                    if (visitedFrames[currVecI] == arr[currFrame][i][j])
                        if (currFrame < 6)
                            currVecI++;
                        else
                            isInVec = true;
                    if (!isInVec && arr[currFrame][i][j] == currPixel) {
                        currPixelCount++;
                        if (currPixel != modePixel && currPixelCount > modePixelCount) {
                            modePixel = currPixel;
                            modePixelCount = currPixelCount;
                        }
                    } currPixelCount++;

                }
                averageFrame[i][j] = currFrame;
            }
        }
    }
    return averageFrame;
}

Mat getFrame(int c) {
    switch (c) {
    case 0: {return imread("img1.jpg"); }
    case 1: {return imread("img2.jpg"); }
    case 2: {return imread("img3.jpg"); }
    case 3: {return imread("img4.jpg"); }
    case 4: {return imread("img9.jpg"); }
    case 5: {return imread("img6.jpg"); }
    case 6: {return imread("img7.jpg"); }
    case 7: {return imread("img8.jpg"); }
    case 8: {return imread("img11.jpg"); }
    case 10: {return imread("test.jpg"); }
    default: {return Mat(); }
    }
}

void videoCodeLevel1(String path) {
    VideoCapture vid(path);
    Mat myImg;
    vid.read(myImg);
   // Mat myImg2;

    while (!myImg.empty()) {
        imshow("vid", myImg);
        waitKey(15);
        vid.read(myImg);
        //myImg = Comparetor().difference(myImg, myImg2);
    }

}
void videoCodeLevel2(String path) {
    VideoCapture vid(path);

    Mat matArr[10];
    int matSize = 0;

    Mat img;

    int count = 0;
    int*** thresould;
    //int** campared;


    vid.read(img);

    while (!img.empty()) {

        thresould = Converter().convert_Color(img);

        Mat imgl = Converter().convert_Mat(thresould, img.rows, img.cols);
        Converter().convert_Grey(imgl);
        imshow("vid", imgl);
        waitKey(5);
        vid.read(img);

        //count++;
        //if (count == 100) {
        //    count == 0;
        //    matSize = 0;
        //}

    }


}
void videoCodeLevel3(String path) {
    VideoCapture vid(path);
    Mat frm;
    vid.read(frm);

    GreyImage img1 = GreyImage(frm);

    while (img1.pixelCount()) {
        img1.display(5);
        vid.read(frm);
        img1.setImage(frm);

    }

}
void videoCodeLevel4(String path) {
    VideoCapture vid(path);
    Mat frm;
    vid.read(frm);

    HawkFrame prev = HawkFrame(frm);

    while (!prev.isNull()) {
        prev.show();
        vid.read(frm);

        HawkFrame curr = HawkFrame(frm);
        if (!curr.isNull())
            prev.compare_update(curr);
        else break;
    }

}


void temp(String path) {
    VideoCapture vid(path);
    Mat frm;
    vid.read(frm);

   GreyImage img1 = GreyImage(frm);

   while (img1.pixelCount()) {
       img1.display(5);
       vid.read(frm);
       img1.setImage(frm);
   }
}

void GreyFrame(VideoCapture& vid) {
    Cleaning clean;
    Mat frame;
    do {
        clean.Change_Grey_BW(vid, frame);
        if (frame.empty())
            break;
        //clean.objectCleaning(frame, PERSON_WIDTH, PERSON_HEIGHT);

        imshow("vid",frame);
        waitKey(5);
    } while (!frame.empty());
}
void ColorFrame(VideoCapture& vid) {
    //Cleaning clean;
    //Mat frame;
   //// BlackWhiteFrame bw;
    //
    //do {
    //
    //    if (clean.Change_Color_Mat_BW(vid, frame)) {
    //        bw.set(frame);
    //        bw.travers();
    //        imshow("vid", bw.getFrame());
    //        waitKey(5);
    //    }
    //    else {
    //        break;
    //    }
    //} while (!frame.empty());
}
void HawkImageFrame(VideoCapture& vid) {
    Cleaning clean;
    HawkFrame frame;
    do {
        if (clean.Change_Hawk_BW(vid, frame))
            frame.show();

    } while (!frame.isNull());
}


*/


void fast(VideoCapture& vid) {
    Converter convert;
    Segmentation segment;

    ObjectIdentification identity;

    // img = orignal colored img 
    // uImg = processed img
    Mat img, uImg;
    int ids = 0;
    while (true) {
        img = segment._fit(vid);
        if (segment._shouldEnd()) {
            break;
        }
        // this call will only work before or without transform if segmentation is wth default method
        uImg = segment._getForeGroundFrame(); 
       // uImg = segment._transform();

        //identity._fit(uImg);

        // this function is working on recursive function... which is not working in my system
        //identity._transform();
        //ids=identity._transform(uImg);
        //cout << ids << endl;
        // this function is based on row by rows detection which is quite complecated and not yet completed
        //identity._transform_updated();

        if (!uImg.empty()) {
            imshow("uimg", uImg);
            waitKey(10);
        }

        //if (!img.empty()) {
        //    imshow("img", img);
        //    waitKey(10);
        //}

    }

}

void slow(VideoCapture& vid) {
    Converter convert;
    Segmentation segment;
    ObjectIdentification identity;
    Image img, uImg;



    while (true) {
        img = segment.fit(vid);
        if (segment.shouldEnd()) {
            break;
        }
        uImg = segment.transform();
        //uImg = segment.getCurFrame();

        //identity.fit(uImg);

        //identity.transform();

        //img=segment.fit_transform(img);
        if (!uImg.empty()) {
            imshow("img", convert.convert_Mat(uImg));
            waitKey(10);
        }

    }

}


int main() {

    VideoCapture vid;
    vid.open("vid001.mp4");
    if (!vid.isOpened())
    {
        std::cout << "can not open" << std::endl;
        return -1;
    }
    // this block is to remove the first noise of the video that is being used else not needed
    {
        Mat f;
        int id = 0;
        while (id < 30) {
            vid.read(f);
            id++;
        }

    }


    fast(vid);
    // slow(vid);

    return 0;
}

