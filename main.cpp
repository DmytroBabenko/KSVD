#include "ImageParser.h"
#include "Denoiser.h"

#include <iostream>
#include <string>



int main(int argc, char** argv) {

    auto start = std::chrono::system_clock::now();


    if (argc < 3 || argc > 5) {
        std::cerr << "Invalid number of input parameters" << std::endl;
        return -1;
    }


    try
    {
        const std::string inputImage = argv[1];
        const std::string outputImage = argv[2];

        bool useParallel = false;
        bool useSIMD = false;

        //TODO: make this parse more elegant
        if (argc  == 4)
        {
            useParallel = strcmp(argv[3], "-p") == 0;
            useSIMD = strcmp(argv[3], "-s") == 0;
        }
        else if (argc  == 5)
        {
            useParallel = strcmp(argv[3], "-p") == 0 || strcmp(argv[4], "-p") == 0;
            useSIMD = strcmp(argv[3], "-s") == 0 || strcmp(argv[4], "-s") == 0;
        }

        Matrix imgMatrix = ImageParser::loadImage(inputImage);

        Denoiser denoiser(useParallel, useSIMD);
        Matrix denoisedImage = denoiser.denoise(imgMatrix);

        ImageParser::saveImage(denoisedImage, outputImage);

    }
    catch (const std::exception& ex)
    {
        std::cerr<<"Exception was thrown: "<<ex.what()<<std::endl;
        return -2;
    }
    catch (...)
    {
        std::cerr<<"Undefined exception was thrown"<<std::endl;
        return -3;
    }

    auto end = std::chrono::system_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Executing time: " <<elapsed.count() << " s"<<std::endl;

    return 0;
}