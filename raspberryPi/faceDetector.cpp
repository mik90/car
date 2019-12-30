#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>

#include "faceDetector.hpp"

using namespace cv;

// Tutorial: https://docs.opencv.org/master/db/d28/tutorial_cascade_classifier.html

namespace Car
{

FaceDetector::FaceDetector(const String& faceFilename, const String& eyesFilename)
{
    // Note: if these classifiers are too slow, try out the LBP classifiers
    // instead of the Haar classifiers

    // Use OpenCV to search for the file as well
    if (!m_faceCascade.load(samples::findFile(faceFilename)))
    {
        std::cerr << "ERROR: Could not load face cascade\n";
        std::exit(1);
    }

    if (!m_eyeCascade.load(samples::findFile(eyesFilename)))
    {
        std::cerr << "ERROR: Could not load eye cascade\n";
        std::exit(1);
    }

    m_captureDevice.open(0);
    if (!m_captureDevice.isOpened())
    {
        std::cerr << "ERROR: Could not open video capture\n";
        std::exit(1);
    }
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
    {
        // We've detected at least once face

        for(size_t i = 0; i < faces.size(); ++i)
        {
            // For each face, figure out if there are eyes
            // It's harder to get a false positive if there are
            // eyes along with the face

            // ROI - Region Of Interest
            Mat faceROI = frame_gray(faces[i]);

            // In each faces, detect the eyes
            std::vector<Rect> eyes;
            m_eyeCascade.detectMultiScale(faceROI, eyes);

            if (!eyes.empty())
                // I could specify the amount of eyes, but glasses
                // and hair can obscure them so I'll just let it be
                return true;
        }
    }

    return false;
}

}
