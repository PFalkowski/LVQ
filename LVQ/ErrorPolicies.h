/**
*  Written by Piotr Falkowski, 2014. piotr.falkowski.fm at gmail.com
*  this work is open source under agpl-3.0
**/
#ifndef ERRORPOLICIES_H_INCLUDED
#define ERRORPOLICIES_H_INCLUDED

/*
* Policy classes for error handling.
* Reference: http://wazniak.mimuw.edu.pl/index.php?title=Zaawansowane_CPP/Wyk%C5%82ad_7:_Klasy_wytycznych
*/
struct NoThrowPolicy /// : base interface class is redundant - Duck Typing
{
	/// Note that those are assertions, so the release should not throw
	template <class iterator1, class iterator2>
	static void AssertRange(iterator1 begin1, iterator1 end1, iterator2 begin2, iterator2 end2)
	{
		assert(std::distance(begin1, end1) == std::distance(begin2, end2));
	}
};
struct DefaultErrorPolicy /// : base interface class is redundant - Duck Typing
{
	template <class iterator1, class iterator2>
	static void AssertRange(iterator1 begin1, iterator1 end1, iterator2 begin2, iterator2 end2)
	{
		if (!(std::distance(begin1, end1) == std::distance(begin2, end2)))
			throw std::out_of_range("Ranges are not equal.");
	}
};

struct RepairErrorPolicy /// : base interface class is redundant - Duck Typing
{
	template <class iterator1, class iterator2>
	static void AssertRange(iterator1 begin1, iterator1 end1, iterator2 begin2, iterator2 end2)
	{
		if (!(std::distance(begin1, end1) == std::distance(begin2, end2)))
		{
			//**TO DO//
		}

	}
};

#endif//ERRORPOLICIES_H_INCLUDED