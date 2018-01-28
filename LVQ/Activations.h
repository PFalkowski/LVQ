/**
*  Written by Piotr Falkowski, 2014. piotr.falkowski.fm@gmail.com
*  Use as you like, as long as you acknowledge the source.
**/
#ifndef ACTIVATIONS_H_INCLUDED
#define ACTIVATIONS_H_INCLUDED

#include <iostream>

class ActivBase /// : base interface class is redundant - Duck Typing
{
	unsigned all;
	double LrateBase;
public:
	const unsigned& Activations() const{ return all; }// one does not simply copy
	void Activate() { ++all; }
	double Lrate() const
	{
		if (all == 0) return 1;
		return LrateBase / sqrt(all);
	}
	explicit ActivBase(const double& StartingLearningRate = 1.0) : all(), LrateBase(StartingLearningRate) {}
	virtual ~ActivBase() {}

	inline std::ostream& ToStream(std::ostream& stream) const
	{
		stream << all << ' ' << LrateBase;
		return stream;
	}
	inline std::istream& FromStream(std::istream& stream)
	{
		stream >> all >> LrateBase;
		return stream;
	}
};
inline std::ostream& operator<< (std::ostream& stream, const ActivBase& a)
{
	return a.ToStream(stream);
}
inline std::istream& operator>> (std::istream& stream, ActivBase& a)
{
	return a.FromStream(stream);
}


#endif//ACTIVATIONS_H_INCLUDED