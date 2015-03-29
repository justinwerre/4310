/*
 * DWT.cpp
 *
 *  Created on: Mar 28, 2015
 *      Author: Justin Werre
 */

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include "../headers/DWT.h"
using namespace cv;

// performs a 2-d haar wavelet transform on the image
void DWT(Wavelet &wavelets)
{
	Wavelet temp( wavelets.size(), std::vector<double>( wavelets.at( 0 ).size(), 0 ) );
	Wavelet temp1( wavelets.size(), std::vector<double>( wavelets.at( 0 ).size(), 0 ) );

	double weight_factor = sqrt(2)/2;
	int half_height = wavelets.size() / 2;
	int half_width = wavelets.at( 0 ).size() /2;

	// perform wavelet transform on rows
	for( int x = 0; x < half_height - 1; x += 2 )
	{
		for( unsigned int y = 0; y < wavelets.at( x ).size(); y++ )
		{
			double current_pixel = wavelets.at( x * 2 ).at( y );
			double next_pixel = wavelets.at( x * 2 + 1).at( y );

			temp.at( x ).at( y ) = weight_factor * current_pixel + weight_factor * next_pixel;
			temp.at( x + half_height ).at( y ) = -weight_factor * next_pixel + weight_factor * current_pixel;
		}
	}

	// perform wavelet transform on columns
	for( unsigned int x = 0; x < wavelets.size(); x++ )
	{
		for( int y = 0; y < half_width; y += 2 )
		{
			double current_pixel = temp.at( x ).at( y * 2 );
			double next_pixel = temp.at( x ).at( y * 2 + 1 );

			temp1.at( x ).at( y ) = weight_factor * current_pixel + weight_factor * next_pixel;
			temp1.at( x ).at( y + half_width ) = -weight_factor * next_pixel + weight_factor * current_pixel;
		}
	}

	// copy the wavelets to a mat object
	Mat wavelet_image( wavelets.size(), wavelets.at( 0 ).size(), CV_8U );
	for( int x = 0; x < wavelet_image.rows; x++ )
	{
		for( int y = 0; y < wavelet_image.cols; y++)
		{
			double t = temp1.at( x ).at( y );

			if( t < 0 )
			{
				t = 0;
			}else if( t > 255 )
			{
				t = 255;
			}

			wavelet_image.data[wavelet_image.step * x + y] = static_cast<uchar>( t );
		}
	}

	namedWindow( "Display Image", WINDOW_AUTOSIZE );
	imshow( "Display Image", wavelet_image );
}
