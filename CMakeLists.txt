cmake_minimum_required(VERSION 3.12)
project(OCR C)

set(CMAKE_C_STANDARD 99)

add_executable(OCR main.c Bitmap/bitmap.c Bitmap/bitmap.h ImageProcessing/BlackAndWhite.h ImageProcessing/BlackAndWhite.c ImageProcessing/RSLA.c ImageProcessing/RSLA.h)