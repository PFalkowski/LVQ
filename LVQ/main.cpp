/**
*  Written by Piotr Falkowski, 2014. piotr.falkowski.fm@gmail.com
*  Use as you like, as long as you acknowledge the source.
**/
// LVQ dynamic.cpp : Defines the entry point for the console application.
//
#include "DataAdapter.h"
#include <cstdlib> // itoa
void testEasyData()
{
	std::vector<double> data = { 1, 2, 3, 4 };
	std::vector<double> data2 = { 1.0, 2.1, 2.0, 0.1 };
	LVQ<Neuron<double>> x(11, 4);
	std::cout << "Network:\n" << x;
	x.WTALearn(data.begin(), data.end(), 5);
	std::cout << "BMU after learning: " << x.BMU(data.begin(), data.end()) << std::endl;
	std::cout << "Network after learning:\n" << x;
	std::cout << "Output for data {" << data << "} = " << x.Output(data.begin(), data.end());
}

using std::cout;
using std::endl;

int main()
{
	/*typedef Neuron<double, ActivBase, PullLearning, DefaultErrorPolicy> LVQNeuron;
	typedef LVQ<LVQNeuron> LVQ1;*/

	cout << GetConfigurationInfo() << endl;
	try
	{
		NetworkFileAdapter<Neuron<double> > adapter("dane_uczace.txt");
		std::cout << adapter.Validate("dane_walidacyjne.txt");
		/*adapter.LearnFile("dane_walidacyjne.txt");
		result = adapter.Validate("dane_walidacyjne.txt");
		std::cout << result;*/

		adapter.SaveToFile("x.xtx");


		//data_from_file.OpenFile("dane_uczace.txt");
		/*data_from_file.Learn("dane_uczace.txt");
		data_from_file.Test("dane_testowe.txt");*/
		/*std::vector<double> data = { 1, 2, 3, 4 };
		std::vector<double> data2 = { 1.0, 2.1, 2.0, 0.1 };*/
		//Timer timer;
		//LVQ<Neuron<double>> x(11, 4);
		//x.LoadFromFile("x");

		//
		//std::cout << "Neuron size: " << AsMemory(sizeof(Neuron<double>)) << std::endl;
		//std::cout << "Network size = " << x.Network().size() << std::endl;
		//std::cout << "Network:\n" << x;
		//std::cout << "Data: " << data << std::endl;
		//Normalize(data.begin(), data.end(), data.begin());
		//std::cout << "Data after normalization: " << data << std::endl;
		//std::cout << "BMU " << x.BMU(data.begin(), data.end()) << std::endl;
		//x.WTALearn(data.begin(), data.end(), 5);
		//std::cout << "BMU after learning: " << x.BMU(data.begin(), data.end()) << std::endl;
		//std::cout << "Network after learning:\n" << x;
		//std::cout << "Output for data {" << data << "} = " << x.Output(data.begin(), data.end());
		//std::cout << timer << std::endl;

		//x.SaveToFile("x");



	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	/*
	auto x = GetNormalRandoms();
	for (int i = 0; i<x.size(); ++i)
	std::cout <<x[i] << std::endl;*/
	getchar();
	return 0;
}

