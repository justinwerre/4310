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
void DWT(Wavelet &wavelets, int height, int width, int depth)
{
	Wavelet temp( height, std::vector<double>( width, 0 ) );
	Wavelet temp1( height, std::vector<double>( width, 0 ) );
	double weight_factor = sqrt(2)/2;
	int half_height = height / 2;
	int half_width = width /2;

	std::cout << "depth: " << depth << " height: " << height << " width: " << width << std::endl;

	// perform wavelet transform on rows
	for( int x = 0; x < half_height; x++ )
	{
		for( int y = 0; y < width; y++ )
		{
			double current_pixel = wavelets.at( x * 2 ).at( y );
			double next_pixel = wavelets.at( x * 2 + 1).at( y );

			temp.at( x ).at( y ) = weight_factor * current_pixel + weight_factor * next_pixel;
			temp.at( x + half_height ).at( y ) = -weight_factor * next_pixel + weight_factor * current_pixel;
		}
	}

	// perform wavelet transform on columns
	for( int x = 0; x < height; x++ )
	{
		for( int y = 0; y < half_width; y++ )
		{
			double current_pixel = temp.at( x ).at( y * 2 );
			double next_pixel = temp.at( x ).at( y * 2 + 1 );

			temp1.at( x ).at( y ) = weight_factor * current_pixel + weight_factor * next_pixel;
			temp1.at( x ).at( y + half_width ) = -weight_factor * next_pixel + weight_factor * current_pixel;
		}
	}

	// copy temp1 back into the wavelet vector
	for( int x = 0; x < height; x++ )
	{
		for( int y = 0; y < width; y++ )
		{
			wavelets.at( x ).at( y ) = temp1.at( x ).at( y );
		}
	}

	// recursively perform the Haar wavelet transform to a depth of three
	if( depth + 1 < MAX_TRANSFORM_DEPTH )
	{
		DWT(wavelets, half_height, half_width, depth + 1 );
	}
}
