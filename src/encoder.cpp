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
#include <sstream>
#include <vector>
#include "../headers/compressor.h"
#include "../headers/encoder.h"
#include "../headers/decompressor.h"
#include "../headers/modelA.h"

void encode(Wavelet &image, std::string file_name)
{
	std::ofstream fout;
	std::string temp_file_name = "temp.txt";
	fout.open( temp_file_name.c_str() );

	// write the wavelets out to a temporary file
	for( unsigned int i = 0; i < image.size(); i++ )
	{
		for( unsigned int j = 0; j < image.at( i ).size(); j++ )
		{
			fout << image.at( i ).at( j ) << " ";
		}
		fout << std::endl;
	}

	fout.close();

	// compress the file
    std::ifstream input( temp_file_name.c_str(), std::ifstream::binary );
    std::ofstream output( file_name.c_str(), std::ofstream::binary );
    modelA<int, 16, 14> cmodel;
    cmodel.dump("cmodel", std::clog);
    compress(input, output, cmodel);

    input.close();
    output.close();
	std::remove( temp_file_name.c_str() );
}

void decode( Wavelet &image, std::string file_name )
{
	std::string temp_file_name = "temp.txt";
	std::ifstream input( file_name.c_str(), std::ifstream::binary );
	std::ofstream output( temp_file_name.c_str(), std::ofstream::binary );
	modelA<int, 16, 14> cmodel;
	cmodel.dump( "cmodel", std::clog );
	decompress( input, output, cmodel );
	input.close();
	output.close();

	// read file into a Wavelet
	int i = 0;
	std::ifstream fin;
	std::string line;
	fin.open( temp_file_name.c_str() );
	image.resize( 0 ); // make sure the image is empty
	while( std::getline( fin, line ) )
	{
		double temp;
		std::stringstream ss(line);
		image.push_back( std::vector<double>() );

		while( ss >> temp )
		{
			image.at( i ).push_back( temp );
		}

		i++;
	}

	// clean up
	fin.close();
	std::remove( temp_file_name.c_str() );
}
