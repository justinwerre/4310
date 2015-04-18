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
	double weight_factor = 1 / sqrt(2);
	int half_height = height / 2;
	int half_width = width /2;

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
			double a = wavelets.at( x ).at( y ) * sqrt( 2 );
			double b = wavelets.at( x + half_height ).at( y ) * sqrt( 2 );

			temp.at( 2 * x ).at( y ) = ( a - b ) / 2;
			temp.at( 2 * x + 1 ).at( y ) = ( a + b ) /  2;
		}
	}

	// perform inverse wavelet transform on columns
	for( int x = 0; x < height; x++ )
	{
		for( int y = 0; y < half_width; y++ )
		{
			double a = temp.at( x ).at( y ) * sqrt( 2 );
			double b = temp.at( x ).at( y + half_width ) * sqrt( 2 );

			wavelets.at( x ).at( 2 * y ) = ( a - b ) / 2;
			wavelets.at( x ).at( 2 * y + 1 ) = ( a + b ) /  2;
		}
	}
}

void liftingDWT( Wavelet &wavelets, int height, int width, int depth )
{
	int half_height = height / 2;
	int half_width = width / 2;

	// work on the rows first
	for( int x = 0; x < height; x++ )
	{
		// do the prediction step
		for( int y = 1; y < width; y += 2 )
		{
			double d = wavelets.at( x ).at ( y );
			double s0 = wavelets.at( x ).at( y - 1 );
			double s1 = -128;

			if( y == width - 1 )
				std::cout << d << std::endl;

			// make sure s1 is in the array bounds
			if( y + 1 < wavelets.at( x ).size() )
			{
				s1 = wavelets.at( x ).at( y + 1 );
			}

			// di = di - 1/2( s0 + s1 )
			wavelets.at( x ).at( y ) = d - 0.5 * ( s0 + s1 );
		}

		// do the update step
		for( int y = 0; y < width; y += 2 )
		{
			double s = wavelets.at( x ).at( y );
			double d0 = 0;
			double d1 = 0;

			// make sure d0 is in the array bounds
			if( y - 1 > 0 )
			{
				d0 = wavelets.at( x ).at( y - 1 );
			}

			// make sure d1 is in the array bounds
			if( y + 1 < wavelets.at( x ).size() )
			{
				d1 = wavelets.at( x ).at( y + 1 );
			}

			// si = si + 1/4( d0 + d1 )
			 wavelets.at( x ).at( y ) = s + 0.25 * ( d0 + d1 );
		}

		// separate highpass and lowpass output, technically this should be done
		// first, since this is the lazy wavelet transform of the algorithm
		std::vector<double> row( wavelets.at( x ) );

		// move evens
		for( int y = 0; y < half_width; y++ )
		{
			wavelets.at( x ).at( y ) = row.at( 2 * y );
		}

		// move odds
		for( int y = 0; y < half_width; y++ )
		{
			wavelets.at( x ).at( y + half_width ) = row.at( 2 * y + 1 );
		}
	}

	// work on columns now
	for( int y = 0; y < width; y++ )
	{
		// do the prediction step
		for( int x = 1; x < height; x += 2 )
		{
			double d = wavelets.at( x ).at ( y );
			double s0 = wavelets.at( x - 1 ).at( y );
			double s1 = -128;

			// make sure s1 is in the array bounds
			if( x + 1 < wavelets.size() )
			{
				s1 = wavelets.at( x + 1 ).at( y );
			}

			// di = di - 1/2( s0 + s1 )
			wavelets.at( x ).at( y ) =  d - 0.5 * ( s0 + s1 );
		}

		// do the update step
		for( int x = 0; x < height; x += 2 )
		{
			double s = wavelets.at( x ).at( y );
			double d0 = 0;
			double d1 = 0;

			// make sure d0 is in the array bounds
			if( x - 1 > 0 )
			{
				d0 = wavelets.at( x - 1 ).at( y );
			}

			// make sure d1 is in the array bounds
			if( x + 1 < wavelets.size() )
			{
				d1 = wavelets.at( x + 1 ).at( y );
			}

			// si = si + 1/4( d0 + d1 )
			 wavelets.at( x ).at( y ) = s + 0.25 * ( d0 + d1 );
		}

		// separate highpass and lowpass output, technically this should be done
		// first, since this is the lazy wavelet transform of the algorithm
		std::vector<double> col;

		for( int i = 0; i < height; i++ )
		{
			col.push_back( wavelets.at( i ).at( y ) );
		}

		// move evens
		for( int x = 0; x < half_height; x++ )
		{
			wavelets.at( x ).at( y ) = col.at( 2 * x );
		}

		// move odds
		for( int x = 0; x < half_height; x++ )
		{
			wavelets.at( x + half_height ).at( y ) = col.at( 2 * x + 1 );
		}
	}

	// perform this operation recursively
	if( depth + 1 < MAX_TRANSFORM_DEPTH )
	{
		liftingDWT( wavelets, half_height, half_height, depth + 1 );
	}
}
