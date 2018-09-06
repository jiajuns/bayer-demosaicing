#pragma once

#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

class Demosaic {
private:
    cv::Mat image;
    cv::Mat r;
    cv::Mat g;
    cv::Mat b;
    cv::Mat n;
    int rows;
    int cols;
    cv::Mat demosaicImageRGB;
    cv::Mat demosaicImageNir;
    // cv::Mat colorImage;
    // cv::Mat result;

public:
    explicit Demosaic(std::string filePath);
    void generateRGBComponents();
    void interpolate();
    void colorize();
    // void squaredDifference();
    // void modifiedInterpolation();
    void display();
};



