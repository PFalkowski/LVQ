/**
*  Written by Piotr Falkowski, 2014 piotr.falkowski.fm at gmail.com
*  this work is open source under agpl-3.0
**/
#ifndef OUTPUT_H_INCLUDED
#define OUTPUT_H_INCLUDED


template<class T, class Learning, class ActivationType>
class BasicOutput
{
	T value;
	ActivationType activations;
	Learning learning;
public:
	inline void Activate(const T& in)  { activations.Activate(); learning(in, value, activations.Lrate()); }
	inline const T& Value() const { return value; }
	inline operator const T& () const { return value; }
	inline BasicOutput(const T& val) : value(val), activations(), learning() {}

	std::ostream& ToStream(std::ostream& stream) const { stream << value << ' ' << activations;	return stream; }
	std::istream& FromStream(std::istream& stream) { stream >> value >> activations;	return stream; }
	virtual ~BasicOutput(){}
};

template<class T, class L, class A>
inline std::ostream& operator<< (std::ostream& stream, const BasicOutput<T, L, A>& a)
{
	return a.ToStream(stream);
}
template<class T, class L, class A>
inline std::istream& operator>> (std::istream& stream, BasicOutput<T, L, A>& a)
{
	return a.FromStream(stream);
}

#endif // OUTPUT_H_INCLUDED