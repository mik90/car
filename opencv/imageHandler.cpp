#include <iostream>
#include <opencv2/opencv.hpp>

// Scratchpad for learning OpenCV
// Currently looking at https://docs.opencv.org/master/d4/d94/tutorial_camera_calibration.html

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