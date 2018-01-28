/**
*  Written by Piotr Falkowski, 2014. piotr.falkowski.fm@gmail.com
*  Use as you like, as long as you acknowledge the source.
**/
#ifndef HELPERS_H_INCLUDED
#define HELPERS_H_INCLUDED

#include <vector>
#include <random>
#include <iostream>
#include <istream>
#include <ostream>
#include <limits>
#include <exception>
#include <chrono>
#include <string>
#include <sstream>
#include <functional>
#include <algorithm>
#include <cassert>
#include <numeric>
#include <array>


template<class T>
std::string AsMemory(const T& bytes)
{
	std::ostringstream strs;
	if (bytes < 1024)
	{
		strs << (bytes) << " bytes";
	}
	else if (bytes < (1024 * 1024))
	{
		strs << (bytes / 1024) << " Kb";
	}
	else if (bytes < (1024 * 1024 * 1024))
	{
		strs << (bytes / (1024 * 1024)) << " Mb";
	}
	else
	{
		strs << (bytes / (1024 * 1024 * 1024)) << " Gb";
	}
	return strs.str();
}
template<class T>
std::string AsTime(const T& milliseconds)
{
	std::ostringstream strs;
	auto hours = milliseconds / 3600000;
	auto minutes = (milliseconds % 3600000) / 60000;
	auto seconds = (milliseconds % 60000) / 1000;
	auto ms = (milliseconds % 1000);

	if (hours > 0)
	{
		strs << hours << " h, ";
	}
	if (minutes > 0)
	{
		strs << minutes << " min, ";
	}
	if (seconds > 0)
	{
		strs << seconds << " sec, ";
	}

	strs << ms << " ms ";

	return strs.str();
}
//
//template <class C>
//std::ostream& operator<<(std::ostream& os, const C& some_data)
//{
//	bool next_flag = false;
//	std::for_each(some_data.begin(), some_data.end(),
//		[&os, &next_flag](const typename some_data::value_type& t){ if (next_flag) os << ' '; os << t; next_flag = true; });
//	return os;
//}
template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& some_data)
{
	bool next_flag = false;
	std::for_each(some_data.begin(), some_data.end(),
		[&os, &next_flag](const T& t){ if (next_flag) os << ' '; os << t; next_flag = true; });
	return os;
}
template <class T, size_t N>
std::ostream& operator<<(std::ostream& os, const std::array<T, N>& some_data)
{
	bool next_flag = false;
	std::for_each(some_data.begin(), some_data.end(),
		[&os, &next_flag](const T& t){ if (next_flag) os << ' '; os << t; next_flag = true; });
	return os;
}
template <class T>
std::ostream& operator<<(std::ostream& os, const std::initializer_list<T>& some_data)
{
	bool next_flag = false;
	std::for_each(some_data.begin(), some_data.end(),
		[&os, &next_flag](const T& t){ if (next_flag) os << ' '; os << t; next_flag = true; });
	return os;
}

template<class T = double>
std::vector<T> GetNormalRandoms(const unsigned& howMany = 10, const T& Mean = 0.0, const T& SD = 1.0)
{
	static std::default_random_engine generator;
	std::normal_distribution<T> distribution(Mean, SD);
	std::vector<T> temp(howMany);
	for (unsigned i = 0; i < howMany; ++i)
		temp[i] = distribution(generator);
	return temp;
}
template<class InputIterator, class OutputIterator>
void Normalize(InputIterator InputBegin, InputIterator InputEnd, OutputIterator OutputBegin)
{
	typedef typename OutputIterator::value_type value_type;
	value_type sqrt_sum_of_powers = sqrt(std::accumulate(InputBegin, InputEnd, value_type(), [](const value_type& init, const value_type& t){ return init + t*t; }));
	for (; InputBegin != InputEnd; ++InputBegin, ++OutputBegin)
		*OutputBegin = (*InputBegin) / sqrt_sum_of_powers;
}
template<class OutputIterator>
void GetNormalRandoms(OutputIterator first, OutputIterator last, typename OutputIterator::value_type Mean = 0.0, typename OutputIterator::value_type SD = 1.0)
{
	static std::default_random_engine generator;
	std::normal_distribution<typename OutputIterator::value_type> distribution(Mean, SD);
	for (; first != last; ++first)
		*first = distribution(generator);
}
template <class InputIterator1, class InputIterator2 >
auto EuclideanDistance(InputIterator1 i_first1, InputIterator1 i_last1, InputIterator2 w_first2)->decltype((*i_first1)*(*w_first2))
{
	auto distance = 0.0;

	for (; i_first1 != i_last1; ++i_first1, ++w_first2)
	{
		distance += pow(((*i_first1) - (*w_first2)), 2);
	}
	return sqrt(distance);
}
class Timer
{
	typedef std::chrono::high_resolution_clock Clock;
	Clock::time_point first;

public:
	Timer() : first(Clock::now()) {}

	template<typename U>
	typename U::rep Elapsed() const
	{
		return std::chrono::duration_cast<U>(Clock::now() - first).count();
	}
	void Reset()
	{
		first = Clock::now();
	}
	friend std::ostream& operator<< (std::ostream& os, const Timer& t)
	{
		os << "It took " << AsTime(t.Elapsed<std::chrono::milliseconds>());
		return os;
	}
};
/** \brief Split string
* http://stackoverflow.com/a/9676623
* \param str as string to split
* \param delimiters
* \return Vector of substrings
*
*/

template<class T = std::string>
std::vector<T> Split(const T& str, const T& delimiters = " ,:\t")
{
	std::vector<T> v;
	typename T::size_type start = 0;
	auto pos = str.find_first_of(delimiters, start);
	while (pos != T::npos)
	{
		if (pos != start) // ignore empty tokens
			v.emplace_back(str, start, pos - start);
		start = pos + 1;
		pos = str.find_first_of(delimiters, start);
	}
	if (start < str.length()) // ignore trailing delimiter
		v.emplace_back(str, start, str.length() - start); // add what's left of the string
	return v;
}

inline std::string GetConfigurationInfo()
{
	std::stringstream output;
	output << "Hello! This is " << sizeof(int)* 8 << " bit environment and ISO C++ " << __cplusplus << '\n'
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
		<< "windows platform"
#elif defined(unix) || defined(__unix__) || defined(__unix) || defined(__linux) 
		<< "linux platform"
#elif __posix
		<< "posix platform"
#elif __APPLE__
		<< "iOS platform"
#endif

#ifdef _MSC_VER
		<< ", compiled using MSVC " << _MSC_VER << " version"
#endif
		<< '.';
	return output.str();
}


#endif