#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

class VideoCaptureMeta {
        VideoCapture v;
        int deviceNum;
    public:
        void setValues(VideoCapture,int) ;
        VideoCapture getDevice() ;
        int getDeviceNum() ;
} ;

void VideoCaptureMeta::setValues(VideoCapture v_, int deviceNum_) {
    v = v_ ;
    deviceNum = deviceNum_ ;
}

int VideoCaptureMeta::getDeviceNum() {
    return deviceNum;
}

VideoCapture VideoCaptureMeta::getDevice() {
    return v;
}

//  cc ./test__opencv_webcam.cpp -o test__opencv_webcam__exe  -lstdc++ -lopencv_core -lopencv_video -lopencv_highgui -lopencv_imgproc -ltiff

//  compile cpp on command line :  https://stackoverflow.com/questions/20010244/undefined-reference-to-stdios-baseinitinit?rq=1

//  set up key binding to compile :  https://stackoverflow.com/questions/30269449/how-do-i-set-up-visual-studio-code-to-compile-c-code

//  gdb / Gnu debugger - basics of running :  https://stackoverflow.com/questions/18271363/line-by-line-c-c-code-debugging-in-linux-ubuntu

//  VSCode extension for very basic compiling :  https://marketplace.visualstudio.com/items?itemName=brapifra.c-compiler

int main(int argc, char const *argv[])
{
    // Following procedure detects, how many webcams are accessible from 0 on upwards.
    int numberOfDevices = 0;
    bool noError = true;

    vector<VideoCaptureMeta> cameraDefs;

    while (numberOfDevices<100) //(noError)
    {
        try {
            // Check if camera is available.
            VideoCapture videoCapture(numberOfDevices); // Will crash if not available, hence try/catch.
            if (videoCapture.isOpened() ) {
                cout << "Device number " << numberOfDevices << " looks like a camera we can use. " << endl;
                VideoCaptureMeta camDefn;  camDefn.setValues(videoCapture,numberOfDevices);
                cameraDefs.push_back(camDefn);
            } else {
                cout << "Cannot open device number " << numberOfDevices << " to use. " << endl;
            }
            // ...
        } catch( cv::Exception& e ) {
            const char* message = e.what() ;
            cout << "caught a problem with "<< numberOfDevices << ": "<< message << endl;    
        } catch (...) {
            cout << "caught a problem with "<< numberOfDevices << endl;
            noError = false;
        }

        // If above call worked, we have found another camera.
        numberOfDevices++;
    }
    cout << "found " << cameraDefs.size() << " cameras:" << endl;
    for (VideoCaptureMeta& cameraDefn: cameraDefs) {
        cout << "Camera # " << cameraDefn.getDeviceNum() << " is open == " << cameraDefn.getDevice().isOpened() << endl;
    }



    /* code */
    cout << "-9000" << endl;
    return -9000;
}
