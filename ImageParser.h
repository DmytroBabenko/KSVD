//
// Created by Dmytro Babenko on 2019-06-11.
//

#ifndef KSVD_IMAGEPARSER_H
#define KSVD_IMAGEPARSER_H

#include "Matrix.h"

#include <string>
#include <vector>

typedef std::vector<std::vector<float>> Matrix;


class ImageParser {
public:
    static Matrix loadImage(const std::string& file);
    static void saveImage(const Matrix& imgMatrix, const std::string& file);

};


#endif //KSVD_IMAGEPARSER_H
