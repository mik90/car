#include <iostream>

#include "faceDetector.hpp"

using namespace cv;

// Tutorial: https://docs.opencv.org/master/db/d28/tutorial_cascade_classifier.html

namespace Car
{

FaceDetector::FaceDetector(const String& faceFilename)
{
    // Note: if these classifiers are too slow, try out the LBP classifiers
    // instead of the Haar classifiers

    // Use OpenCV to search for the file as well
    if (!m_faceCascade.load(samples::findFile(faceFilename)))
    {
        std::cerr << "ERROR: Could not load face cascade\n";
        std::exit(1);
    }

    m_captureDevice.open(0);
    if (!m_captureDevice.isOpened())
    {
        std::cerr << "ERROR: Could not open video capture\n";
        std::exit(1);
    }

    std::cout << "Initialized FaceDetector\n";
}


bool FaceDetector::areFacesAndEyesVisible()
{
    Mat frame;
    m_captureDevice.read(frame);
    if (frame.empty())
    {
        std::cerr << "ERROR:" << __PRETTY_FUNCTION__  << " - frame is empty, exiting...\n";
        std::exit(1);
    }

    Mat frame_gray;
    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

    std::vector<Rect> faces;
    m_faceCascade.detectMultiScale(frame_gray, faces);

    if (!faces.empty())
        // We've detected at least once face
        return true;
    else
        return false;
}

}
