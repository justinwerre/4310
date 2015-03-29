/*
 * helpers.cpp
 *
 *  Created on: Mar 28, 2015
 *      Author: Justin Werre
 */

#include "../headers/helpers.h"
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>


// function from http://stackoverflow.com/a/17820615
void type2str(int type) {
  std::string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  std::cout << r << std::endl;
}


