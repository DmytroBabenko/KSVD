//
// Created by Dmytro Babenko on 2019-06-11.
//

#include "ImageParser.h"
#include "Utils.h"

#include <opencv2/opencv.hpp>


void ImageParser::saveImage(const Matrix &imgMatrix, const std::string &file)
{
    cv::Mat image = Utils::Matrix2cvMat(imgMatrix).t();

//    cv::imwrite(file + "1.png", image);
//    normalize(image, image, 0.0, 1.0, 32, CV_32F);
    cv::imwrite(file, image);

//    cv::imshow("recover image",image);
//    cv::waitKey(0);

}

Matrix ImageParser::loadImage(const std::string &file) {
    cv::Mat imageMat = cv::imread(file);
    cv::cvtColor(imageMat, imageMat, cv::COLOR_BGR2GRAY );


    if (imageMat.empty())
        throw std::invalid_argument("Could not load the image");


    imageMat.convertTo(imageMat, CV_32FC1);

    imageMat = imageMat.t();
    Matrix matrix = Utils::cvMat2Matrix(imageMat);

    return matrix;

}
