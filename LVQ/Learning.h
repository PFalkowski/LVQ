/**
*  Written by Piotr Falkowski, 2014 piotr.falkowski.fm at gmail.com
*  this work is open source under agpl-3.0
**/
#ifndef LEARNING_H_INCLUDED
#define LEARNING_H_INCLUDED

class PullLearning  /// : base interface class is redundant - Duck Typing
{
public:
	template <class InputIterator, class OutputIterator>
	inline void operator()(InputIterator InputBegin, InputIterator InputEnd, OutputIterator OutputBegin, const double& Lrate)
	{
		for (; InputBegin != InputEnd; ++InputBegin, ++OutputBegin)
			*OutputBegin += Lrate * ((*InputBegin) - (*OutputBegin));
	}
	template <class U>
	inline void operator()(const U& in, U& out, const double& Lrate)
	{
		out += Lrate * (in - out);
	}
};


#endif //LEARNING_H_INCLUDED