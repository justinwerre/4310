/*
 * main.cpp
 *
 *  Created on: Mar 25, 2015
 *      Author: Justin Werre
 */

#include <opencv2/opencv.hpp>
#include <iostream>
#include "../headers/DWT.h"
#include "../headers/helpers.h"

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

  Wavelet wavelet( image.rows, std::vector<double>( image.cols ) );

  for( int x = 0; x < image.rows; x++ )
  {
	  for( int y = 0; y < image.cols; y++ )
	  {
		  wavelet.at( x ).at( y ) = static_cast<double>( image.data[image.step * x + y] );
	  }
  }
  type2str( image.type() );

//  std::cout << image;
  DWT(wavelet);

//  namedWindow( "Display Image", WINDOW_AUTOSIZE );
//  imshow( "Display Image", image );

  waitKey(0);

  return 0;
}
