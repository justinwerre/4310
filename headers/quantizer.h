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
#define MANTISSA 7
#define EXPONENT 12.5
#define NORMAL_DYNAMIC_RANGE 8

/* quantizes the wavelets.
 * wavelets: The wavelets to be quantized.
 */
void quantizer( Wavelet & wavelets );

/* Computes the sign of the number.
 * Returns 1 if positive, -1 if negative. 0 is considered positive.
 */
int sign( double num );

/* quantizes the value based on the step size
 * value: The value to be quantized
 * step_size: The step size to use during quantization
 */
double qunatize( double value, double step_size );

/* Calculates the step size based on the exponent, mantissa and
 * current sub-band decomposition level.
 * nb: The number of sub-band decomposition levels from the original
 * 	   image tile component to the current sub-band
 * exponent: The exponent value used to calculate the last sub-bands
 * 			 step size.
 */
double calculateStepSize( int nb, double &exponent );

#endif /* HEADERS_QUANTIZER_H_ */
