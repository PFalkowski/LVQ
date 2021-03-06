/**
*  Written by Piotr Falkowski, 2014 piotr.falkowski.fm at gmail.com
*  this work is open source under agpl-3.0
**/
#ifndef NORMALIZATION_H_INCLUDED
#define NORMALIZATION_H_INCLUDED

///***** TO DO -> implement appropriate normalization for your needs. *****///

struct NoNormalization
{
	/*
	* When input files are in normalized range <-1,1> normalization is not important
	* so this is a placeholder that does noting and will be optimized by most compilers to plain nothing (as any empty function).
	*/

	template<class InputIterator, class OutputIterator>
	static void Normalize(InputIterator InputBegin, InputIterator InputEnd, OutputIterator OutputBegin){}
};

#endif //NORMALIZATION_H_INCLUDED