/*
 * DWT.h
 *
 *  Created on: Mar 28, 2015
 *      Author: Justin Werre
 */

#ifndef HEADERS_DWT_H_
#define HEADERS_DWT_H_
#define MAX_TRANSFORM_DEPTH 3

typedef std::vector<std::vector<double> > Wavelet;

/*
 * Recursively performs a 2-d Haar wavelet transform on the wavelets.
 * wavelets: The wavelets to perform the transform on.
 * height: The height of the area to perform the transform on.
 * width: The width of the area to perform the transform on.
 * depth: The current depth transform.
 */
void DWT( Wavelet &wavelets, int height, int width, int depth = 0 );

/* Recursively performs an inverse 2-d Haar wavelet transform on the wavelets
 * wavelets: The wavelets to perform the transform on.
 * height: The height of the area to perform the transform on.
 * width: The width of the area to perform the transform on.
 * depth: The current depth transform.
 */
void inverseDWT( Wavelet &wavelets, int height, int width, int depth = 0 );

#endif /* HEADERS_DWT_H_ */
