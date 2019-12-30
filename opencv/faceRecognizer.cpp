#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>

using namespace cv;

// Tutorial: https://docs.opencv.org/master/db/d28/tutorial_cascade_classifier.html

void detectAndDisplay(Mat frame);

CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;

int main(int argc, const char** argv)
{
    CommandLineParser parser(argc, argv,
                             "{help h||}"
                             "{face_cascade|data/haarcascades/haarcascade_frontalface_alt.xml|Path to face cascade.}"
                             "{eyes_cascade|data/haarcascades/haarcascade_eye_tree_eyeglasses.xml|Path to eyes cascade.}"
                             "{camera|0|Camera device number.}");
    parser.about("This program detects face/eyes in a video stream using cv::CascadeClassifier\n");
    parser.printMessage();

    String face_cascade_name = samples::findFile(parser.get<String>("face_cascade"));
    String eyes_cascade_name = samples::findFile(parser.get<String>("eyes_cascade"));

    // Load cascades from files
    if (!face_cascade.load(face_cascade_name))
    {
        std::cerr << "ERROR: Could not load face cascade\n";
        return -1;
    }

    if (!eyes_cascade.load(eyes_cascade_name))
    {
        std::cerr << "ERROR: Could not load eyes cascade\n";
        return -1;
    }

    int camera_device = parser.get<int>("camera");
    VideoCapture capture;
    // Read the video stream
    capture.open(camera_device);

    if (!capture.isOpened())
    {
        std::cerr << "ERROR: Could not open video capture\n";
        return -1;
    }

    Mat frame;
    while (capture.read(frame))
    {
        // Keep reading frames from the VideoCapture until we are no longer able to

        if(frame.empty())
        {
            std::cout << "Stopped capturing frames, exiting loop.\n";
            break;
        }

        // Apply the classifier to the frame
        detectAndDisplay(frame);

        if(waitKey(10) == 27)
            break; // exits on the Escape key
    }

    return 0;
}

void detectAndDisplay(Mat frame)
{
    Mat frame_gray;
    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

    // Detect face(s)
    std::vector<Rect> faces;
    face_cascade.detectMultiScale(frame_gray, faces);

    for(size_t i = 0; i < faces.size(); ++i)
    {
        Point center(faces[i].x + faces[i].width / 2,   // x
                     faces[i].y + faces[i].height / 2); // y

        auto sz =  Size(faces[i].width / 2, faces[i].height / 2);
        auto purple = Scalar(255, 0, 255);
        // Create an ellipse around the face
        ellipse(frame, center, sz, 0, 0, 360, purple, 4);
        Mat faceROI = frame_gray(faces[i]);

        // In each faces, detect the eyes
        std::vector<Rect> eyes;
        eyes_cascade.detectMultiScale(faceROI, eyes);

        for(size_t j = 0; j < eyes.size(); j++)
        {
            Point eye_center(faces[i].x + eyes[j].x + eyes[j].width / 2,
                             faces[i].y + eyes[j].y + eyes[j].height / 2);
            int radius = cvRound((eyes[j].width + eyes[j].height) * 0.25);
            circle(frame, eye_center, radius, Scalar(255,0,0), 4);
        }
    }
    imshow("Capture - Face detection", frame);
}