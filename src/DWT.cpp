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

			// a = (sqrt(2)c + sqrt(2)d)/2
			temp.at( x ).at( y ) = weight_factor * current_pixel + weight_factor * next_pixel;

			// b = (-sqrt(2)c + sqrt(2)d)/2
			temp.at( x + half_height ).at( y ) = -weight_factor * current_pixel + weight_factor * next_pixel;
		}
	}

	// perform wavelet transform on columns
	for( int x = 0; x < height; x++ )
	{
		for( int y = 0; y < half_width; y++ )
		{
			double current_pixel = temp.at( x ).at( y * 2 );
			double next_pixel = temp.at( x ).at( y * 2 + 1 );

			// a = (sqrt(2)c + sqrt(2)d)/2
			wavelets.at( x ).at( y ) = weight_factor * current_pixel + weight_factor * next_pixel;

			// b = (-sqrt(2)c + sqrt(2)d)/2
			wavelets.at( x ).at( y + half_width ) = -weight_factor * current_pixel + weight_factor * next_pixel;
		}
	}

	// recursively perform the Haar wavelet transform to the max transform depth
	if( depth + 1 < MAX_TRANSFORM_DEPTH )
	{
		DWT(wavelets, half_height, half_width, depth + 1 );
	}
}

void inverseDWT( Wavelet &wavelets, int height, int width, int depth )
{
	Wavelet temp( height, std::vector<double>( width, 0 ) );
	int half_height = height / 2;
	int half_width = width /2;

	// Recursively perform the inverse transform
	if( depth + 1 < MAX_TRANSFORM_DEPTH )
	{
		inverseDWT( wavelets, half_height, half_width, depth + 1 );
	}

	// perform inverse wavelet transform on rows
	for( int x = 0; x < half_height; x++ )
	{
		for( int y = 0; y < width; y++ )
		{
			double a = wavelets.at( x ).at( y );
			double b = wavelets.at( x + half_height ).at( y );

			temp.at( 2 * x ).at( y ) = ( a - b ) / sqrt( 2 );
			temp.at( 2 * x + 1 ).at( y ) = ( a + b ) / sqrt( 2 );
		}
	}

	// perform inverse wavelet transform on columns
	for( int x = 0; x < height; x++ )
	{
		for( int y = 0; y < half_width; y++ )
		{
			double a = temp.at( x ).at( y );
			double b = temp.at( x ).at( y + half_width );

			wavelets.at( x ).at( 2 * y ) = ( a - b ) / sqrt( 2 );
			wavelets.at( x ).at( 2 * y + 1 ) = ( a + b ) / sqrt( 2 );
		}
	}
}
