/*
 * main.cpp
 *
 *  Created on: Mar 25, 2015
 *      Author: Justin Werre
 */

#include <opencv2/opencv.hpp>
#include <stdexcept>
#include <iostream>
#include "../headers/DWT.h"
#include "../headers/helpers.h"
#include "../headers/quantizer.h"
#include "../headers/encoder.h"

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
	Wavelet dWavelet;
	for( int x = 0; x < image.rows; x++ )
	{
		for( int y = 0; y < image.cols; y++ )
		{
			// subtract 128 in order to center the values around 0;
			wavelet.at( x ).at( y ) = static_cast<double>( image.data[ image.step * x + y ] ) - 128;
		}
	}

	try
	{
		liftingDWT( wavelet, image.rows, image.cols );
	}catch( std::out_of_range &e )
	{
		std::cout << "lifing out of range\n";
		exit( 1 );
	}

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
	imshow( "DWT", image );

	quantizer( wavelet);

	histogram( wavelet, "qhist.csv" );

	try{
		encode( wavelet, "lena.jp2" );
		decode( dWavelet, "lena.jp2" );
	}catch( std::out_of_range &e ){
		std::cerr << "why?\n" << e.what();
	}

	try{
		inverseQuantizer( dWavelet );
	}catch( std::out_of_range &e )
	{
		std::cout << "iquantizer\n";
	}

	try{
		inverseDWT( dWavelet, image.rows, image.cols );
	}catch( std::out_of_range &e ){
		std::cout << "idwt\n";
	}

	// copy the wavelets to a mat object
	for( int x = 0; x < wavelet_image.rows; x++ )
	{
		for( int y = 0; y < wavelet_image.cols; y++)
		{
			// increase the value of the wavelet by 128 so we can see what is going on with it easier.
			double t = dWavelet.at( x ).at( y ) + 128;

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

	namedWindow( "Inverse", WINDOW_AUTOSIZE );
	imshow( "Inverse", wavelet_image );

	imwrite( "lena.png", wavelet_image );

	waitKey(0);

	return 0;
}
