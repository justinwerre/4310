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
			wavelets.at( x ).at( y ) = quantize( wavelets.at( x ).at( y ), 0, 0 );
		}
	}

	// Quantized the rest of the sub-bands
	for( int i = 0; i < MAX_TRANSFORM_DEPTH; i++ )
	{
		// The old sub-band dimensions are the current sub-bands
		// dead zone
		unsigned int dead_zone_height = subband_height;
		unsigned int dead_zone_width = subband_width;

		// Calculate the dimensions of the current sub-band
		subband_height *= 2;
		subband_width *= 2;

		for( unsigned int x = 0; x < subband_height; x++ )
		{
			for( unsigned int y = 0; y < subband_width; y++ )
			{
				// In each sub-band, we consider the lower sub-bands a
				// "dead zone" that doesn't need to be quantized. This
				// "dead zone" goes from (0, 0) to (subband_width / 2, subband_height / 2)
				if( x >= dead_zone_height && y >= dead_zone_width )
				{
					// HH sub-band, 2 analysis gain bits
					wavelets.at( x ).at( y ) = quantize( wavelets.at( x ).at( y ), i, 2 );
				}
				else if( x >= dead_zone_height || y >= dead_zone_width )
				{
					// LH or HL sub-band, 1 analysis gain bit
					wavelets.at( x ).at( y ) = quantize( wavelets.at( x ).at( y ), i, 1 );
				}
			}
		}
	}
}

void inverseQuantizer( Wavelet &wavelets )
{
	unsigned int subband_height = wavelets.size();
	unsigned int subband_width = wavelets.at( 0 ).size();

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
			wavelets.at( x ).at( y ) = ( wavelets.at( x ).at( y ) + RECONSTRUCTION_PARAMETER * sign( wavelets.at( x ).at( y ) ) ) * stepSize( 0, 0 );
		}
	}

	// Quantized the rest of the sub-bands
	for( int i = 0; i < MAX_TRANSFORM_DEPTH; i++ )
	{
		// The old sub-band dimensions are the current sub-bands
		// dead zone
		unsigned int dead_zone_height = subband_height;
		unsigned int dead_zone_width = subband_width;

		// Calculate the dimensions of the current sub-band
		subband_height *= 2;
		subband_width *= 2;

		for( unsigned int x = 0; x < subband_height; x++ )
		{
			for( unsigned int y = 0; y < subband_width; y++ )
			{

				// In each sub-band, we consider the lower sub-bands a
				// "dead zone" that doesn't need to be quantized. This
				// "dead zone" goes from (0, 0) to (subband_width / 2, subband_height / 2)
				if( x >= dead_zone_height && y >= dead_zone_width )
				{
					// HH sub-band, 2 analysis gain bits
					wavelets.at( x ).at( y ) = ( wavelets.at( x ).at( y ) + RECONSTRUCTION_PARAMETER * sign( wavelets.at( x ).at( y ) ) ) * stepSize( i, 2 );
				}
				else if( x >= dead_zone_height || y >= dead_zone_width )
				{
					// LH or HL sub-band, 1 analysis gain bit
					wavelets.at( x ).at( y ) = ( wavelets.at( x ).at( y ) + RECONSTRUCTION_PARAMETER * sign( wavelets.at( x ).at( y ) ) ) * stepSize( i, 1 );
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

double quantize( double value, int nb, int agb )
{
	return sign( value ) * floor( std::abs( value ) / stepSize( nb, agb ) );
}

double stepSize( int nb, int agb )
{
	// calculate the new step size using 8.2-64 from Digital Image Processing
	double exponent = EXPONENT + nb - MAX_TRANSFORM_DEPTH;
	return pow( 2, NORMAL_DYNAMIC_RANGE - exponent ) * ( 1 + MANTISSA / pow( 2, 11 ) );
}
