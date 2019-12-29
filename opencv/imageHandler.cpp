#include <iostream>
#include <opencv2/opencv.hpp>

// Scratchpad for learning OpenCV
// Currently looking at https://docs.opencv.org/master/d4/d94/tutorial_camera_calibration.html
// OpenCV uses 5 distortion parameters as a one-row matrix with 5 columns: k1, k2, p1, p2, k3
// The camera matrix contains the camera focal length parameters as well as the optical centers
// expressed as pixel coordinates

// Determing these two matrices is the calibration, OpenCV can use a few different patterns for this

// * Determine distortion matrix
// * Determine camera matrix
// * Take input from file list
// * Read configuration from XML or YAML file
// * Save configuration into XML or YAML file
// * Calculate re-projection error

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Expects a single argument\n";
        return 1;
    }

    cv::Mat image = cv::imread(argv[1], 1);

    if (!image.data)
    {
        std::cerr << "No image data\n";
        return 1;
    }

    cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Display Image", image);

    cv::waitKey(0);
    return 0;
}