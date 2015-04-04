/*
 * main.cpp
 *
 *  Created on: Mar 25, 2015
 *      Author: Justin Werre
 */

#include <opencv2/opencv.hpp>
#include <iostream>
#include <new>
#include "../headers/DWT.h"
#include "../headers/helpers.h"
#include "../headers/quantizer.h"

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

	// copy the image into a wavelet object
	Wavelet wavelet( image.rows, std::vector<double>( image.cols ) );
	for( int x = 0; x < image.rows; x++ )
	{
		for( int y = 0; y < image.cols; y++ )
		{
			// subtract 128 in order to center the values around 0;
			wavelet.at( x ).at( y ) = static_cast<double>( image.data[ image.step * x + y ] ) - 128;
		}
	}

	DWT(wavelet, image.rows, image.cols);

	// copy the wavelets to a mat object
	Mat wavelet_image( image.rows, image.cols, CV_8U );
	for( int x = 0; x < wavelet_image.rows; x++ )
	{
		for( int y = 0; y < wavelet_image.cols; y++)
		{
			// increase the value of the wavelet by 128 so we can see what is going on with it easier.
			double t = wavelet.at( x ).at( y ) + 128;

			if( t < 0 )
			{
				t = 0;
			}else if( t > 255 )
			{
				t = 255;
			}

			wavelet_image.data[ wavelet_image.step * x + y ] = static_cast<uchar>( t );
		}
	}

	namedWindow( "DWT", WINDOW_AUTOSIZE );
	imshow( "DWT", wavelet_image );

	quantizer( wavelet, 3 );

	// copy the wavelets to a mat object
	for( int x = 0; x < wavelet_image.rows; x++ )
	{
		for( int y = 0; y < wavelet_image.cols; y++)
		{
			// increase the value of the wavelet by 128 so we can see what is going on with it easier.
			double t = wavelet.at( x ).at( y ) + 128;
			std::cout << t << std::endl;
			if( t < 0 )
			{
				t = 0;
			}else if( t > 255 )
			{
				t = 255;
			}

			wavelet_image.data[ wavelet_image.step * x + y ] = static_cast<uchar>( t );
		}
	}

	namedWindow( "Quantized", WINDOW_AUTOSIZE );
	imshow( "Quantized", wavelet_image );

	waitKey(0);

	return 0;
}
