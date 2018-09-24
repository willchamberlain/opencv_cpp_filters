#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>

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

//  cc ./test__opencv_webcam.cpp -std=c++11 -o test__opencv_webcam__exe  -lstdc++ -lopencv_core -lopencv_video -lopencv_highgui -lopencv_imgproc -ltiff

//  compile cpp on command line :  https://stackoverflow.com/questions/20010244/undefined-reference-to-stdios-baseinitinit?rq=1

//  set up key binding to compile :  https://stackoverflow.com/questions/30269449/how-do-i-set-up-visual-studio-code-to-compile-c-code

//  gdb / Gnu debugger - basics of running :  https://stackoverflow.com/questions/18271363/line-by-line-c-c-code-debugging-in-linux-ubuntu

//  VSCode extension for very basic compiling :  https://marketplace.visualstudio.com/items?itemName=brapifra.c-compiler


//  in threads ?? :  https://stackoverflow.com/questions/27661353/opencv-camera-capture-from-within-a-thread


int main(int argc, char const *argv[])
{
    // Following procedure detects, how many webcams are accessible from 0 on upwards.
    int numberOfDevices = 0;
    bool noError = true;

    vector<VideoCaptureMeta> cameraDefs;

    while (numberOfDevices<100) //(noError)
    {
        if (0==numberOfDevices) {
            numberOfDevices++;
            continue;
        }
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

    int frameNum = 0;
    bool saveData = false;
    bool keepLooping = true;

    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);
    while(keepLooping) {
        frameNum++;
        for (VideoCaptureMeta& cameraDefn: cameraDefs) {
            cout << "Camera # " << cameraDefn.getDeviceNum() << " is open == " << cameraDefn.getDevice().isOpened() << endl;


            // ---- for each camera ---- : put this into each 'agent' 
            Mat image;
            //  image = imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file
            cameraDefn.getDevice().read(image) ;
            if(! image.data ) {                             // Check for invalid input
                cout <<  "Could not open or find the image" << std::endl ;
            } else {
                string window_name = "Displaying camera "+to_string(cameraDefn.getDeviceNum());
                namedWindow(window_name , WINDOW_AUTOSIZE );// Create a window for display.
                imshow( window_name , image );                   // Show our image inside it.
                //waitKey(0);                                          // Wait for a keystroke in the window                 
                // Press  ESC on keyboard to exit
                char c=(char)waitKey(25);   // Wait for a keystroke in the window for 30ms
                if(c==27) {
                    keepLooping = false;
                } else if (c==115 || c==83){ 
                    saveData = !saveData;
                    cout << "Toggled saveData to " << saveData << endl;
                }
                



                    // grab histogram of patch for the HSV model

                    // Calculate histogram
                        // Initialize parameters
                        int histSize = 256;    // bin size
                        float range[] = { 0, 255 };
                        const float *ranges[] = { range };
                    Mat gray;    
                    cvtColor(image, gray, CV_BGR2GRAY);    
                        //  imshow(window_name, gray) ;
                        //  waitKey(0) ;
                    Mat hist;
                    calcHist( &gray, 1, 0, Mat(), hist, 1, &histSize, ranges, true, false );


                    // grab texture of patch for the texture model 


                    // save data
                    if(saveData) {
                        imwrite("/mnt/nixbig/temp/camera_"+to_string(cameraDefn.getDeviceNum())+"/camera_"+to_string(cameraDefn.getDeviceNum())+"_"+to_string(frameNum)+".png", image, compression_params);
                        imwrite("/mnt/nixbig/temp/camera_"+to_string(cameraDefn.getDeviceNum())+"_bw/camera_"+to_string(cameraDefn.getDeviceNum())+"_bw_"+to_string(frameNum)+".png", gray, compression_params);
/*
mkdir -p /mnt/nixbig/temp/camera_1
mkdir -p /mnt/nixbig/temp/camera_1_bw
mkdir -p /mnt/nixbig/temp/camera_2
mkdir -p /mnt/nixbig/temp/camera_2_bw
mkdir -p /mnt/nixbig/temp/camera_3
mkdir -p /mnt/nixbig/temp/camera_3_bw
*/                        
                    }
                    
            }        
        }
    }


    

    /* code */
    cout << "-9000" << endl;
    return -9000;
}

