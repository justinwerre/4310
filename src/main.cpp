/*
 * test.cpp
 *
 *  Created on: Mar 25, 2015
 *      Author: justin
 */

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include "../headers/DWT.h"

using namespace cv;

int main( int argc, char** argv )
{
  Mat image;
  image = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE );

  if( argc != 2 || !image.data )
    {
      std::cout << "No image data \n" ;
      return -1;
    }

//  std::cout << image;
  DWT(image);

//  namedWindow( "Display Image", WINDOW_AUTOSIZE );
//  imshow( "Display Image", image );

  waitKey(0);

  return 0;
}
