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
#include "../headers/encoder.h"

void encode(Wavelet &image, std::string file_name)
{
	std::ofstream fout;
	fout.open( file_name.c_str() );

	for( unsigned int i = 0; i < image.size(); i++ ){
		int min = 255;
		int max = -255;

		// find the min and max
		for( unsigned int j = 0; j < image.at( i ).size(); j++){
			min = std::min( min, static_cast<int>( image.at( i ).at( j ) ) );
			max = std::max( max, static_cast<int>( image.at( i ).at( j ) ) );
		}

		int value_range = max - min + 1;
		std::vector<int> frequency( value_range, 10 );
		int total = value_range * 10;
		double lower_bound = 0;
		double upper_bound = 1;

		fout << min << " " << max << " ";

		// encode the vector
		for( unsigned int j = 0; j < image.at( i ).size(); j++ )
		{
			int current_symbol = static_cast<int>( image.at( i ).at( j ) );
			int probability = sum( frequency, current_symbol + labs( min ) );
			double range = upper_bound - lower_bound;

			// compute the new lower and upper bound
			lower_bound = lower_bound + ( range * probability / total );
			upper_bound = lower_bound + ( range * frequency.at( current_symbol + labs( min ) ) / total );

			foo( lower_bound, upper_bound, fout );

		}
		fout << std::endl;
	}

	fout.close();
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
