/**
*  Written by Piotr Falkowski, 2014 piotr.falkowski.fm at gmail.com
*  this work is open source under agpl-3.0
**/
#ifndef NEURON_H_INCLUDED
#define NEURON_H_INCLUDED


/**
*  Written by Piotr Falkowski, 2014. piotr.falkowski.fm@gmail.com
*  Use as you like, as long as you acknowledge the source.
*
*  Programming principles:
*  No pointers - NO LEAKS. Bjarne Stroustup: "I left the pointers in c++ because of backward compatibility" See:http://www.stroustrup.com/bs_faq2.html#pointers-and-references
*  DRY - hence templates and policy classes
*  RAII - hence the file menaging and parsing in constructor
*  No Overhead - hence Move semantics, pass-by-reference-to-const and copy-and-swap idiom use
*  SRP, No YAGNI & KISS - hence the straight-forward design.
*  OCP - hence virtual dtors and non-virtual functions calling policy classes (duck-typed polimorphism + Non-virtual Interface)
*  No 3 star programming - see http://c2.com/cgi/wiki?ThreeStarProgrammer
*  No Unneded inheritance -
*     B.Stroustup: Inheritance has been seriously and systematically overused and misused:
*     “When your only tool is a hammer everything looks like a nail” ref: http://channel9.msdn.com/Events/GoingNative/2013/Opening-Keynote-Bjarne-Stroustrup
*	  also see: http://blog.codinghorror.com/inherits-nothing/
*
*/

#ifndef __cplusplus
#error "Not C++"
// #define __cplusplus 0L
#endif // __cplusplus

#if __cplusplus < 201103L
#ifndef _MSC_VER
#define nullptr NULL
#define noexcept
#elif _MSC_VER <= 1800
#define noexcept
#endif // _MSC_VER
#endif // __cplusplus

//#define NDEBUG

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
#include <typeinfo>
#include <fstream>

#include "Helpers.h"
#include "Activations.h"
#include "ErrorPolicies.h"
#include "Learning.h"
#include "Output.h"


template <class T = double,
class ActivationType = ActivBase,
class Learning = PullLearning,
class ErrorPolicy = DefaultErrorPolicy,
class OutputType = BasicOutput<T, Learning, ActivationType> >
class Neuron
{
protected:
	std::vector<T> weights;
	ActivationType activations;
	Learning learning;
	OutputType output;
public:
	typedef T value_type;
	typedef OutputType output_type;

	const inline std::vector<T>& Weights() const { return weights; }
	const inline ActivationType& Activations() const { return activations; }
	const inline OutputType& Output() const { return output; }

	template <class InputIterator>
	inline void Learn(InputIterator begin, InputIterator end);

	template <class InputIterator, class U>
	inline void Learn(InputIterator begin, InputIterator end, const U& realVal);

	template <class InputIterator>
	inline double Distance(InputIterator begin, InputIterator end) const;

	template <class InputIterator>
	inline Neuron(InputIterator begin, InputIterator end, const T& ExpectedOutput = 0.0, const T& Lrate = 1.0);

	template <class U>
	inline explicit Neuron(const std::vector<U>& data, const T& expected_output, const T& Lrate = 1.0);

	inline explicit Neuron(unsigned NoDendrites = 2, const T& expected_output = 0, const T& Lrate = 1.0);
	virtual ~Neuron();

	std::ostream& ToStream(std::ostream& stream) const;
	std::istream& FromStream(std::istream& stream);

	/**
	*  Copy ctor.
	*/
	inline Neuron(const Neuron&) = default;
	/**
	*  Move ctor.
	*/
	inline Neuron(Neuron&& anonymous);
	/**
	*  This is assignment operator AND move assignment operator; see http://stackoverflow.com/a/11540204/3638932
	*/
	inline Neuron& operator=(Neuron source);/// use copy constructor to make a copy

};

template <class T, class A, class L, class E, class O>
template <class InputIterator>
void Neuron<T, A, L, E, O>::Learn(InputIterator begin, InputIterator end)
{
	E::AssertRange(begin, end, weights.begin(), weights.end());
	activations.Activate();
	learning(begin, end, weights.begin(), activations.Lrate());
}

template <class T, class A, class L, class E, class O>
template <class InputIterator, class U>
void Neuron<T, A, L, E, O>::Learn(InputIterator begin, InputIterator end, const U& realVal)
{
	Learn(begin, end);
	output.Activate(realVal);
}

template <class T, class A, class L, class E, class O>
template <class InputIterator>
double Neuron<T, A, L, E, O>::Distance(InputIterator begin, InputIterator end) const
{
	E::AssertRange(begin, end, weights.begin(), weights.end());/// Duck - typing
	return EuclideanDistance(begin, end, weights.begin());
}

template <class T, class A, class L, class E, class O>
template <class InputIterator>
Neuron<T, A, L, E, O>::Neuron(InputIterator begin, InputIterator end, const T& expected_output, const T& Lrate)
: weights(begin, end), activations(Lrate), learning(), output(expected_output) {}

template <class T, class A, class L, class E, class O>
template <class U>
Neuron<T, A, L, E, O>::Neuron(const std::vector<U>& data, const T& expected_output, const T& Lrate)
: Neuron(data.begin(), data.end(), expected_output, Lrate) {}

template <class T, class A, class L, class E, class O>
Neuron<T, A, L, E, O>::Neuron(unsigned NoDendrites, const T& expected_output, const T& Lrate)
: Neuron(GetNormalRandoms(NoDendrites), expected_output, Lrate) {}

template <class T, class A, class L, class E, class O>
Neuron<T, A, L, E, O>::Neuron(Neuron&& anonymous)
: weights(std::move(anonymous.weights)), activations(std::move(anonymous.activations)), learning(std::move(anonymous.learning)), output(std::move(anonymous.output))
{
#ifndef NDEBUG
	std::cerr << "***move ctor\n";
#endif
}
/**
*  This is assignment operator AND move assignment operator; see http://stackoverflow.com/a/11540204/3638932
*/
template <class T, class A, class L, class E, class O>
inline Neuron<T, A, L, E, O>& Neuron<T, A, L, E, O>::operator=(Neuron<T, A, L, E, O> source)/// use copy constructor to make a copy
{
#ifndef NDEBUG
	std::cerr << "assignment\n";
#endif
	std::swap(weights, source.weights);/// move, not copy - source copy will cease to exist when function returns to a caller
	std::swap(activations, source.activations);/// move, not copy
	std::swap(learning, source.learning);/// move, not copy
	std::swap(output, source.output);/// move, not copy
	return *this;
}///source goes out of scope, source copy cease to exist

template <class T, class A, class L, class E, class O>
Neuron<T, A, L, E, O>::~Neuron(){}

template <class T, class A, class L, class E, class O>
std::ostream& Neuron<T, A, L, E, O>::ToStream(std::ostream& stream) const
{
	stream << activations << '\t';
	stream << output << '\t';

	bool next_flag = false;
	std::for_each(weights.begin(), weights.end(),
		[&stream, &next_flag](const T& t){ if (next_flag) stream << ' '; stream << t; next_flag = true; });

	return stream;
}

template <class T, class A, class L, class E, class O>
std::istream& Neuron<T, A, L, E, O>::FromStream(std::istream& stream)
{
	typename std::vector<T>::value_type temp;// or just T
	weights.clear();
	stream >> activations >> output;
	while ((stream.peek() != '\n') && stream.good())
	{
		stream >> temp;
		weights.emplace_back(temp);
	}
	//    if (stream.peek()=='\n') stream.ignore(1); <- redundant
	return stream;
}

template <class T, class A, class L, class E, class O>
inline std::ostream& operator<< (std::ostream& stream, const Neuron<T, A, L, E, O>& n)
{
	return n.ToStream(stream);
}

template <class T, class A, class L, class E, class O>
inline std::istream& operator>> (std::istream& stream, Neuron<T, A, L, E, O>& n)
{
	return n.FromStream(stream);
}
#endif //NEURON_H_INCLUDED
