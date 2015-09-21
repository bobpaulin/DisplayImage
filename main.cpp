
#include <iostream> // for standard I/O
#include <string>   // for strings
#include <iomanip>  // for controlling float print precision
#include <sstream>  // string to number conversion
#include <thread>

#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>  // Gaussian Blur
#include <opencv2/highgui/highgui.hpp>  // OpenCV window I/O

using namespace std;
using namespace cv;

 /** Function Headers */
 void detectAndDisplay( );

 /** Global variables */
 String face_cascade_name = "haarcascade_frontalface_alt.xml";
 CascadeClassifier face_cascade;
 string window_name = "Capture - Face detection";
 Mat webCamMat;
 Point faceCenter[5];
 Size  faceSize[5];
 int numFacesFound = 0;
 bool faceFound = false;
 RNG rng(12345);

int main( int argc, char** argv )
{
    char c;
    int delay = 10;
    VideoCapture camera(0);
    if(!camera.isOpened())
    {
        cout << "Could not open camera" << endl;
        return -1;
    }
    
     //-- 1. Load the cascades
   if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
    
    namedWindow(window_name, CV_WINDOW_AUTOSIZE);
    
    thread first(detectAndDisplay);
    for(;;)
    {
        camera >> webCamMat;
        
        if( !webCamMat.empty() )
       {
             
            //detectAndDisplay( webCamMat ); 
       }
       else
       { printf(" --(!) No captured frame -- Break!"); break; }
        if(faceFound)
        {
            for(int i = 0; i < numFacesFound && i < 5; i++ )
            {
                ellipse( webCamMat, faceCenter[i], faceSize[i], 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
            }
        }
        
        imshow( window_name, webCamMat );
        c = (char) cvWaitKey(delay);
        if(c == 27) break;
    }
    
 return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay()
{
    while(true)
    {
        if(!webCamMat.empty())
        {
            Mat frame = webCamMat.clone();
            std::vector<Rect> faces;
            Mat frame_gray;

            cvtColor( frame, frame_gray, CV_BGR2GRAY );
            equalizeHist( frame_gray, frame_gray );

            //-- Detect faces
            face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );
            int numFaces = faces.size();
            if( numFaces > 0  )
            {
                cout << "Face Found!" << endl;
                for(int i = 0; i < numFaces && i < 5; i++ )
                {
                    Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
                    faceCenter[i] = center;
                    Size centerSize( faces[i].width*0.5, faces[i].height*0.5);
                    faceSize[i] = centerSize;
                }
                if( faces.size() < 5)
                {    
                    
                    numFacesFound = faces.size();
                }
                else
                {
                    numFacesFound = 5;
                }
                faceFound = true;
                
            }
            else{
                faceFound = false;
                numFacesFound = 0;
            }
        }
    }
 }