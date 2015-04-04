/*
 * quantizer.cpp
 *
 *  Created on: Apr 3, 2015
 *      Author: Justin Werre
 */

#include <iostream>
#include <vector>
#include <cmath>
#include "../headers/quantizer.h"


void quantizer( Wavelet &wavelets, int dept )
{
	double step_size = 1;
	for( unsigned int x = 0; x < wavelets.size(); x++ )
	{
		for( unsigned int y = 0; y < wavelets.at( x ).size(); y++ )
		{
			double value = wavelets.at( x ).at( y );
			wavelets.at( x ).at( y ) = sign( value ) * floor( std::abs( value) / step_size );
		}
	}
}

int sign( double num )
{
	int value = 1;

	if( num < 0)
	{
		value = -1;
	}

	return value;
}
