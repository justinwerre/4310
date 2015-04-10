/*
 * encoder.cpp
 *
 *  Created on: Apr 3, 2015
 *      Author: Justin Werre
 */

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "../headers/compressor.h"
#include "../headers/encoder.h"
#include "../headers/modelA.h"

void encode(Wavelet &image, std::string file_name)
{
	std::ofstream fout;
	std::string temp_file_name = "temp.txt";
	fout.open( temp_file_name.c_str() );

	// write the wavelets out to a temporary file
	for( unsigned int i = 0; i < image.size(); i++ )
	{
		for( unsigned int j = 0; j < image.at( i ).size(); j++ )
		{
			fout << image.at( i ).at( j ) << " ";
		}
		fout << std::endl;
	}

	fout.close();

	// compress the file
    std::ifstream input( temp_file_name.c_str() , std::ifstream::binary );
    std::ofstream output( file_name.c_str(), std::ofstream::binary );
    modelA<int, 16, 14> cmodel;
    cmodel.dump("cmodel", std::clog);
    compress(input, output, cmodel);

    input.close();
    output.close();
	std::remove( temp_file_name.c_str() );
}

int sum( std::vector<int> frequency, int value )
{
	int sum = 0;

	for( int i = 0; i < value; i++ )
	{
		sum += frequency.at( i );
	}

	return sum;
}

void foo( double &lower, double &upper, std::ostream &fout )
{
	// remove any shared digits between the upper and lower bound
	int lower_digit = static_cast<int>( lower * 10 );
	int upper_digit = static_cast<int>( upper * 10 );

	while( upper_digit == lower_digit )
	{
		// write out the shared digit
		fout << lower_digit;

		// remove the shared digit from the upper and lower bound
		lower = lower * 10 - lower_digit;
		upper = upper * 10 - upper_digit;

		// recompute the leading digit
		lower_digit = static_cast<int>( lower * 10 );
		upper_digit = static_cast<int>( upper * 10 );
	}
}
