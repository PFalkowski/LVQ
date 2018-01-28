/**
*  Written by Piotr Falkowski, 2014. piotr.falkowski.fm@gmail.com
*  Use as you like, as long as you acknowledge the source.
**/
#ifndef LVQ_H_INCLUDED
#define LVQ_H_INCLUDED

#ifndef NDEBUG
#include <type_traits>
#endif

#include "Helpers.h"
#include "Activations.h"
#include "ErrorPolicies.h"
#include "Learning.h"
#include "Output.h"
#include "Neuron.h"

template<class NeuronType>
class LVQ
{
protected:
	// inheriting from std::vector<NeuronType> is tempting, but unnacteptable.
	// Which is better: std::vector<NeuronType> / std::vector<NeuronType*>
	std::vector<NeuronType> network;
public:
	const std::vector<NeuronType>& Network() { return network; }
	const NeuronType& Network(const size_t& i) { if (i >= network.size()) throw std::out_of_range("request for elem. out of range."); return network[i]; }

	template <class iterator>
	inline NeuronType& BMU(iterator first, iterator last);

	template <class iterator>
	inline const typename NeuronType::output_type& Output(iterator first, iterator last);

	template <class iterator>
	inline void WTALearn(iterator first, iterator last);

	template <class iterator>
	inline void WTALearn(iterator first, iterator last, const typename  NeuronType::value_type& output);

	template <class iterator>
	inline const NeuronType& AddNeuron(iterator first, iterator last, const typename  NeuronType::value_type& output = 0);

	LVQ(unsigned NeronsNumber, unsigned DendritesPerNeuron);

	LVQ();

	virtual ~LVQ();

	std::ostream& ToStream(std::ostream& stream) const;
	std::istream& FromStream(std::istream& stream);

	void SaveToFile(const std::string& FilePath);
	void LoadFromFile(const std::string& FilePath);

};
template<class NeuronType>
LVQ<NeuronType>::LVQ(unsigned NeronsNumber, unsigned DendritesPerNeuron) : network(NeronsNumber)
{
	for (auto &it : network)
	{
		it = NeuronType(DendritesPerNeuron);/// Move assignment, no overhead, no pointers = no leaks
	}
}

template<class NeuronType>
LVQ<NeuronType>::LVQ() : network() {}


template<class NeuronType>
LVQ<NeuronType>::~LVQ(){}

template<class NeuronType>
template <class InputIterator>
void LVQ<NeuronType>::WTALearn(InputIterator first, InputIterator last)
{
	BMU(first, last).Learn(first, last);
}

template<class NeuronType>
template <class InputIterator>
void LVQ<NeuronType>::WTALearn(InputIterator first, InputIterator last, const typename  NeuronType::value_type& output)
{
	BMU(first, last).Learn(first, last, output);
}

template<class NeuronType>
template <class InputIterator>
const NeuronType& LVQ<NeuronType>::AddNeuron(InputIterator first, InputIterator last, const typename  NeuronType::value_type& output)
{
#ifndef NDEBUG
	std::cerr << "Adding neuron with output = " << output << '\n';
#endif
	network.emplace_back(NeuronType(first, last, output));/// Create in place. Should work with push_back in C++11 as well, since push_back(T&&) is defined and it's an anonymus object.

	return network.back();
}
template<class NeuronType>
std::ostream& LVQ<NeuronType>::ToStream(std::ostream& stream) const
{
	std::for_each(network.begin(), network.end(), [&stream](const NeuronType& t){stream << t << '\n'; });
	return stream;
}
template<class NeuronType>
std::istream& LVQ<NeuronType>::FromStream(std::istream& stream)
{
	network.clear();
	NeuronType temp;
	while (stream >> temp)
	{
		/// temp will be left in undefined, but valid state after move, so can be reused
		network.emplace_back(temp);
	}
	return stream;
}

template <class NeuronType>
template <class InputIterator>
NeuronType& LVQ<NeuronType>::BMU(InputIterator first, InputIterator last)
{
#ifndef NDEBUG
	std::cerr << "Searching for BMU...\n";
#endif
	auto distance = std::numeric_limits<double>::max();
	auto min = std::numeric_limits<double>::max();
	NeuronType* bmu_found = nullptr;
	for (auto &it : network)
	{
		distance = it.Distance(first, last);
		if (distance<min)
		{
			min = distance;
			bmu_found = &it;
#ifndef NDEBUG
			std::cerr << "Currently winning output = " << bmu_found->Output() << '\n';
#endif
		}
#ifndef NDEBUG
		std::cerr << "BMU is neuron with output = " << bmu_found->Output() << '\n';
#endif
	}
	if (bmu_found == nullptr)
		throw std::runtime_error("BMU not found. Network has not enough neurons.");//std::to_string(network.size())
	return *bmu_found; /// is it safe - i.e. wont the move occur?
}

template <class NeuronType>
template <class InputIterator>
const typename NeuronType::output_type& LVQ<NeuronType>::Output(InputIterator first, InputIterator last)
{
	return BMU(first, last).Output();
	/*NeuronType* bmu_found = &BMU(first, last);
	if (bmu_found)
	return bmu_found->Output();
	else throw std::runtime_error("BMU not found.");*/
}

template <class NeuronType>
void LVQ<NeuronType>::SaveToFile(const std::string& FilePath)
{
	std::cout << "Saving network to file " << FilePath << '\n';
	std::ofstream File(FilePath);
	if (!File.is_open()) throw std::runtime_error("Failed to open file " + FilePath + " in write mode.");

	File << *this; /// File << ;

	File.close();
	std::cout << "Succesfully saved " << network.size() << " neurons.\n";
}

template <class NeuronType>
void LVQ<NeuronType>::LoadFromFile(const std::string& FilePath)
{
	std::cout << "Loading network from file " << FilePath << '\n';

	std::ifstream File(FilePath);
	if (!File.is_open()) throw std::runtime_error("Failed to open file " + FilePath + " in read mode.");

	/// extract input from file, using chain of defined operators for this, and sub-classes
	File >> *this;


	std::cout << "Succesfully loaded " << network.size() << " neurons.\n";
#ifndef NDEBUG
	std::cerr << "Printing neurons read:\n" << *this << '\n';
#endif

	File.close();
}

template<class NeuronType>
inline std::istream& operator>> (std::istream& stream, LVQ<NeuronType>& n)
{
	return n.FromStream(stream);
}
template<class NeuronType>
inline std::ostream& operator<< (std::ostream& stream, const LVQ<NeuronType>& n)
{
	return n.ToStream(stream);
}


#endif // LVQ_H_INCLUDED
