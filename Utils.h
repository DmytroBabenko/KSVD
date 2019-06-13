//
// Created by Dmytro Babenko on 2019-06-11.
//

#ifndef KSVD_UTILS_H
#define KSVD_UTILS_H

//forward declaration
namespace cv {class Mat;}

#include "Matrix.h"

class Utils {

public:
    static Matrix cvMat2Matrix(const cv::Mat &image);
    static cv::Mat Matrix2cvMat(const Matrix& matrix);

};


#endif //KSVD_UTILS_H
