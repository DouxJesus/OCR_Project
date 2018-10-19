cmake_minimum_required(VERSION 3.12)
project(OCR C)

set(CMAKE_C_STANDARD 99)

add_executable(OCR main.c ImageProcessing/BlackAndWhite.h ImageProcessing/BlackAndWhite.c Image_BMP/BMP.c Image_BMP/BMP.h Image_BMP/pixel_operations.c Image_BMP/pixel_operations.h)