/*
 * helpers.cpp
 *
 *  Created on: Mar 28, 2015
 *      Author: Justin Werre
 */

#include "../headers/helpers.h"
#include "../headers/DWT.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
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

void histogram( Wavelet &wavelets, std::string file_name )
{
	int max = 0;
	int min = 0;

	for( unsigned int x = 0; x < wavelets.size(); x++ )
	{
		for( unsigned int y = 0; y < wavelets.at( x ).size(); y++ )
		{
			max = std::max( static_cast<int>( wavelets.at( x ).at( y ) ), max );
			min = std::min( static_cast<int>( wavelets.at( x ).at( y ) ), min );
		}
	}

	std::vector<int> histogram( max - min + 1, 0 );
	int offset = std::abs( min );

	for( unsigned int x = 0; x < wavelets.size(); x++ )
	{
		for( unsigned int y = 0; y < wavelets.at( x ).size(); y++ )
		{
			int value = static_cast<int>( wavelets.at( x ).at( y ) ) + offset;
			histogram.at( value )++;
		}
	}

	std::ofstream fout;
	fout.open( file_name.c_str() );

	for( int i = 0; i < histogram.size(); i++ )
	{
		fout << i - offset << ", " << histogram.at( i ) << std::endl;
	}

	fout.close();
}
