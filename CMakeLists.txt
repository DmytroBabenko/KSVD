cmake_minimum_required(VERSION 3.13)
project(KSVD)

set(CMAKE_CXX_STANDARD 17)


add_definitions(-DGLM_FORCE_INTRINSICS)

if((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    add_compile_options(-msse4.2)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Intel")
    add_compile_options(/QxSSE4.2)
elseif((CMAKE_CXX_COMPILER_ID MATCHES "MSVC") AND NOT CMAKE_CL_64)
    add_compile_options(/arch:SSE2) # VC doesn't support SSE4.2
endif()


find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} ThreadPool)

add_executable(${PROJECT_NAME} main.cpp
        Denoiser.h Denoiser.cpp
        ImageParser.h ImageParser.cpp
        KSVD.h KSVD.cpp
        LAOperator.h LAOperator.cpp
        Matrix.h
        Utils.h Utils.cpp
        ThreadPool/Worker.h ThreadPool/Worker.cpp
        ThreadPool/ThreadPool.h ThreadPool/ThreadPool.cpp
        SIMD_LAOperator.h SIMD_LAOperator.cpp
        )

target_link_libraries(${PROJECT_NAME}
        ${OpenCV_LIBS}
        pthread
        )
