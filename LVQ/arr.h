#ifndef ARR_H
#define ARR_H

template<class T>
class Arr
{
    size_t sz;//!< Member variable "sz" is size
    size_t cpc;//!< Member variable "cpc" is capacity
    T* elem;//!< Internal storage of dynamically allocated array

//    T* _end;

public:
    /** Default constructor */
    Arr<T>(size_t s);
    /** Copy constructor
    *  \param other Arr<T> to copy from
    */
    Arr<T>(const Arr<T> &other);
    /** Assignment operator
    *  \param other Object to assign from
    *  \return A reference to this
    */
    Arr<T>& operator=(const Arr<T> &other);
//    bool Arr<T>::operator==(const Arr<T> &other);
    /** Array start
    *  \return Address of first element
    */
    T* begin()
    {
        return elem;
    }
    T* begin() const
    {
        return elem;
    }
    /** Array end
    *  \return Address of one element pass the last element
    */
    T* end()
    {
        return (elem+sz);
    }
    T* end() const
    {
        return (elem+sz);
    }
    T& operator[ ](size_t n)
    {
        if (n<sz)
            return elem[n];
        else
            throw "Attempt to acces out of range";
    }
    T& operator[ ](size_t n) const
    {
        if (n<sz)
            return elem[n];
        else
            throw "Attempt to acces out of range";
    }
    /** Access size
    * \return The current value of size
    */
    size_t size() const
    {
        return sz;
    }
    /** Access capacity
    * \return The current value of capacity
    */
    size_t capacity() const
    {
        return cpc;
    }
    /** Add value of type T
    */
    void add(T elem);

    /** Reserve space for n elements
    */
    void reserve(const size_t &n);
    /** Resize and dynamically reallocate internal array
    * if neccesary
    */
    void resize(const size_t &n);
    /** Default destructor */
    virtual ~Arr<T>()
    {
        delete[] elem;
    }

};
template<class T>
Arr<T>::Arr(size_t s = 256) : sz(), cpc(s), elem(new T[s])
{

}

template<class T>
Arr<T>& Arr<T>::operator=(const Arr<T> &other)
{
    for(size_t i=0; i<sz; ++i)
        elem[i] = other.elem[i];
    sz = other.sz;
    cpc = other.cpc;
    return *this;
}

template<class T>
Arr<T>::Arr(const Arr<T> &other) : sz(other.sz), cpc(other.cpc), elem(new T[sz])
{
    for(size_t i=0; i<sz; ++i)
        elem[i] = other.elem[i];
//    _end = &(elem+sz);
}

//template<class T>
//bool Arr<T>::operator==(const Arr<T> &other)
//{
//    if (sz!=other.size()) return false;
//    for(size_t i=0; i<sz; ++i)
//        if (elem[i] != other[i]) return false;
//    return true;
//}
template<class T>
void Arr<T>::add(T value)
{
    if (sz >= cpc)
    {
        reserve(sz*3);
    }
    elem[sz++] = value;
//  ++_end;
}

template<class T>
void Arr<T>::reserve(const size_t &n)
{
    if (cpc < n)
    {
        T* temp = elem;
        elem = new T[n];
        cpc = n;
        for(size_t i=0; i<sz; ++i)
            elem[i] = temp[i];
        delete[] temp;

    }

}

template<class T>
void Arr<T>::resize(const size_t &n)
{
    reserve(n);
    sz = n;
//    _end=&elem[sz];
}

#endif // ARR_H
