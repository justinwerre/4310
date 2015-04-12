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
#include "../headers/DWT.h"


void quantizer( Wavelet &wavelets )
{
	double exponent = EXPONENT;
	double step_size = calculateStepSize( 0, exponent );
	unsigned int subband_height = wavelets.size();
	unsigned int subband_width = wavelets.at( 0 ).size();

	// calculate the dimensions of the n-th level sub-bands
	for( unsigned int i = 0; i < MAX_TRANSFORM_DEPTH; i++ )
	{
		subband_height /= 2;
		subband_width /= 2;
	}

	// Quantized the LL sub-band. We do this one separately since it doesn't have a "dead zone"
	// that isn't quantized with the current step size.
	for( unsigned int x = 0; x < subband_height; x++ )
	{
		for( unsigned int y = 0; y < subband_width; y++ )
		{
			wavelets.at( x ).at( y ) = qunatize( wavelets.at( x ).at( y ), step_size );
		}
	}

	// Quantized the rest of the sub-bands
	for( int i = 0; i < MAX_TRANSFORM_DEPTH; i++ )
	{
		// The old sub-band dimensions are the current sub-bands
		// dead zone
		unsigned int deadZone_height = subband_height;
		unsigned int deadZone_width = subband_width;

		// Calculate the dimensions of the current sub-band
		subband_height *= 2;
		subband_width *= 2;
		step_size = calculateStepSize( i + 1, exponent );
		std::cout << "height: " << subband_height << " width: " << subband_width << " step size: " << step_size << std::endl;

		for( unsigned int x = 0; x < subband_height; x++ )
		{
			for( unsigned int y = 0; y < subband_width; y++ )
			{
				// In each sub-band, we consider the lower sub-bands a
				// "dead zone" that doesn't need to be quantized. This
				// "dead zone" goes from (0, 0) to (subband_width / 2, subband_height / 2)
				if( x >= deadZone_height || y >= deadZone_width )
				{
					// Since were not in the dead zone, quantize the value
					wavelets.at( x ).at( y ) = qunatize( wavelets.at( x ).at( y ), step_size );
				}
			}
		}
	}
}

void inverseQuantizer( Wavelet &wavelets )
{
	double exponent = EXPONENT;
	double step_size = calculateStepSize( 0, exponent );
	unsigned int subband_height = wavelets.size();
	unsigned int subband_width = wavelets.at( 0 ).size();

	std::cout << step_size << std::endl;

	// calculate the dimensions of the n-th level sub-bands
	for( unsigned int i = 0; i < MAX_TRANSFORM_DEPTH; i++ )
	{
		subband_height /= 2;
		subband_width /= 2;
	}

	// Inverse quantized the LL sub-band. We do this one separately since it doesn't have a "dead zone"
	// that isn't quantized with the current step size.
	for( unsigned int x = 0; x < subband_height; x++ )
	{
		for( unsigned int y = 0; y < subband_width; y++ )
		{
			wavelets.at( x ).at( y ) = qunatize( wavelets.at( x ).at( y ), step_size );
		}
	}

	// Quantized the rest of the sub-bands
	for( int i = 0; i < MAX_TRANSFORM_DEPTH; i++ )
	{
		// The old sub-band dimensions are the current sub-bands
		// dead zone
		unsigned int deadZone_height = subband_height;
		unsigned int deadZone_width = subband_width;

		// Calculate the dimensions of the current sub-band
		subband_height *= 2;
		subband_width *= 2;
		step_size = calculateStepSize( i + 1, exponent );
		std::cout << "height: " << subband_height << " width: " << subband_width << " step size: " << step_size << std::endl;

		for( unsigned int x = 0; x < subband_height; x++ )
		{
			for( unsigned int y = 0; y < subband_width; y++ )
			{
				// In each sub-band, we consider the lower sub-bands a
				// "dead zone" that doesn't need to be quantized. This
				// "dead zone" goes from (0, 0) to (subband_width / 2, subband_height / 2)
				if( x >= deadZone_height || y >= deadZone_width )
				{
					// Since were not in the dead zone, inverse quantize the value
					wavelets.at( x ).at( y ) = wavelets.at( x ).at( y ) * step_size ;
				}
			}
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

double qunatize( double value, double step_size )
{
	return sign( value ) * floor( std::abs( value) / step_size );
}

double calculateStepSize( int nb, double &exponent )
{
	int nl = MAX_TRANSFORM_DEPTH;

	// calculate the new exponent
	exponent = exponent + nb - nl;

	// calculate the new step size using 8.2-64 from Digital Image Processing
	double first = pow( 2, NORMAL_DYNAMIC_RANGE - exponent);
	double second = ( 1 + MANTISSA / pow( 2, 11 ) );
	std::cout << "first: " << first << " second: " << second << " exponent: " << exponent << std::endl;
	return first * second;
}
