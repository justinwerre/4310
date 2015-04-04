/*
 * quantizer.h
 *
 *  Created on: Apr 3, 2015
 *      Author: Justin Werre
 */

#include "DWT.h"

#ifndef HEADERS_QUANTIZER_H_
#define HEADERS_QUANTIZER_H_

/* quantizes the wavelets.
 * wavelets: The wavelets to be quantized.
 * depth: The number of sub-bands that were computed in the DWT.
 */
void quantizer( Wavelet & wavelets, int depth );

/* Computes the sign of the number.
 * Returns 1 if positive, -1 if negative. 0 is considered positive.
 */
int sign( double num );

#endif /* HEADERS_QUANTIZER_H_ */
