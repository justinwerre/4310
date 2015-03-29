/*
 * DWT.h
 *
 *  Created on: Mar 28, 2015
 *      Author: Justin Werre
 */

#include <opencv2/opencv.hpp>

#ifndef HEADERS_DWT_H_
#define HEADERS_DWT_H_

typedef std::vector<std::vector<double> > Wavelet;

void DWT(Wavelet &image);

#endif /* HEADERS_DWT_H_ */
