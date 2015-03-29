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
using namespace cv;

void DWT(Mat &image);

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

// performs a 2-d wavelet transform on the image
void DWT(Mat &image)
{
	std::vector<std::vector<double> > wavelets( image.rows * 2, std::vector<double>( image.cols * 2 ) );
	double weight_factor = sqrt(2)/2;

	// perform wavelet transform on rows
	for( int x = 0; x < image.rows; x++ )
	{
		for( int y = 0; y < image.cols - 1; y++ )
		{
			double current_pixel = static_cast<double>(image.data[image.step * x + y]);
			double next_pixel = static_cast<double>(image.data[image.step * x + y + 1]);
			wavelets.at( x ).at( y ) = weight_factor * current_pixel + weight_factor * next_pixel;
			wavelets.at( x ).at( y + image.cols ) = -weight_factor * next_pixel + weight_factor * current_pixel;
		}
	}

	// perform wavelet transform on columns
	for( int x = 0; x < image.rows - 1; x++ )
	{
		for( unsigned int y = 0; y < wavelets.at(x).size(); y++ )
		{
			double current_pixel = wavelets.at( x ).at( y );
			double next_pixel = wavelets.at( x + 1 ).at( y );
			wavelets.at( x  ).at( y ) = weight_factor * current_pixel + weight_factor * next_pixel;
			wavelets.at( x + image.rows ).at( y ) = -weight_factor * next_pixel + weight_factor * current_pixel;
		}
	}

	// copy the wavelets to a mat object
	Mat wavelet_image( wavelets.size(), wavelets.at( 0 ).size(), image.type() );
	for( int x = 0; x < wavelet_image.rows; x++ )
	{
		for( int y = 0; y < wavelet_image.cols; y++)
		{
			// std::cout << wavelets.at( x ).at( y ) << std::endl;
			wavelet_image.data[wavelet_image.step * x + y] = static_cast<uchar>(wavelets.at( x ).at( y ));
		}
	}

	namedWindow( "Display Image", WINDOW_AUTOSIZE );
	imshow( "Display Image", wavelet_image );
}
