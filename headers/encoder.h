/*
 * encoder.h
 *
 *  Created on: Apr 3, 2015
 *      Author: Justin Werre
 */

#include <iostream>
#include <string>
#include "DWT.h"

#ifndef HEADERS_ENCODER_H_
#define HEADERS_ENCODER_H_

/* Writes the Wavelet image into a file named file_name using arithmetic coding
 * on each row in the Wavelet.
 * image: The wavelets to be encoded.
 * file_name: The name of the file to be written to. If the file exists, it will
 * 			  be over written.
 */
void encode( Wavelet & image, std::string file_name );

/* Reads in an encoded image into the wavelet.
 * image: An empty wavelet array to return the image in.
 * file_name: The name of the encoded image.
 */
void decode( Wavelet &image, std::string file_name );

#endif /* HEADERS_ENCODER_H_ */
