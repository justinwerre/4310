/*
 * quantizer.h
 *
 *  Created on: Apr 3, 2015
 *      Author: Justin Werre
 */

#include "DWT.h"

#ifndef HEADERS_QUANTIZER_H_
#define HEADERS_QUANTIZER_H_

// Used for calculating the step size
#define MANTISSA 12
#define EXPONENT 5
#define NORMAL_DYNAMIC_RANGE 8
#define RECONSTRUCTION_PARAMETER 0.75

/* quantizes the wavelets.
 * wavelets: The wavelets to be quantized.
 */
void quantizer( Wavelet &wavelets );

/* Attempts to reverse the quantizer process
 * wavelets: The wavelets to be inverse quantized
 */
void inverseQuantizer( Wavelet &wavelets );

/* Computes the sign of the number.
 * Returns 1 if positive, -1 if negative. 0 is considered positive.
 */
int sign( double num );

/* quantizes the value based on the step size
 * value: The value to be quantized
 * nb: The number of sub-band decomposition levels from the original
 * 	   image tile component to the current sub-band
 * agb: The analysis gain bit for the current sub-band
 */
double quantize( double value, int nb, int agb );

/* Calculates the step size for the given sub-band
 * nb: The number of sub-band decomposition levels from the original
 * 	   image tile component to the current sub-band
 * agb: The analysis gain bit for the current sub-band
 */
double stepSize( int nb, int agb );

#endif /* HEADERS_QUANTIZER_H_ */
