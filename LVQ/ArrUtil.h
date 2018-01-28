#ifndef ARRHELPER_H
#define ARRHELPER_H

template<class T>
void Show(const T &a, bool show_elements = false)
{

	std::cout << "Size of Arr: " << a.size() << std::endl;
	std::cout << "Capacity of Arr: " << a.capacity() << std::endl;
//	if (show_elements)
//            Show(a.begin(), a.end());
}

template<class InputIterator>
void Show(InputIterator first, InputIterator last)
{
    size_t i = 0;
    while(first!=last)
		std::cout << "Element[" << i++ << "]\t= " << *(first++) << std::endl;
}

template<class T>
void Randomize(T &a)
{
	srand(time(NULL));
	for (unsigned i = 0; i<a.size(); ++i)
		a[i] = rand();
}

#endif
