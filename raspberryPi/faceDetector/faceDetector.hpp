#ifndef FACE_DETECTOR_HPP
#define FACE_DETECTOR_HPP

#include <memory>
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/objdetect.hpp"

namespace Car
{

class FaceDetector
{
    public:
        // Loads the classifier data
        FaceDetector(const cv::String& faceFilename);
        // Checks the capture device to see if face/eyes are visible
        bool areFacesAndEyesVisible();
    private:
        cv::CascadeClassifier m_faceCascade;
        cv::VideoCapture      m_captureDevice;
};

}

#endif