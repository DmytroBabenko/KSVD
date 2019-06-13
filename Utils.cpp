//
// Created by Dmytro Babenko on 2019-06-11.
//

#include "Utils.h"

#include <opencv2/opencv.hpp>

Matrix Utils::cvMat2Matrix(const cv::Mat &image)
{
    //TODO: optimize it
    std::vector<float> row(image.cols, 0);
    Matrix matrix(image.rows, row);

    for (int i = 0; i < image.rows; ++i)
    {
        for (int j = 0; j < image.cols; ++j)
        {
            matrix[i][j] = image.at<float>(i, j);
        }
    }

    return matrix;

}

cv::Mat Utils::Matrix2cvMat(const Matrix &matrix)
{
    const size_t rows = matrix.size();
    const size_t cols = matrix[0].size();

    cv::Mat cvMatrix(rows, cols, CV_32FC1);

    for (size_t i= 0; i < rows; ++i)
    {
        for (size_t j = 0; j < cols; ++j)
        {
            cvMatrix.at<float>(i, j) = matrix[i][j];
        }
    }
    return cvMatrix;
}
