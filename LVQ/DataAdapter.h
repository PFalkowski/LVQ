/**
*  Written by Piotr Falkowski, 2014. piotr.falkowski.fm at gmail.com
*  this work is open source under agpl-3.0
**/
#ifndef DATAADAPTER_H_INCLUDED
#define DATAADAPTER_H_INCLUDED

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <utility>      // std::move
#include <cassert>


#include "Helpers.h"
#include "Normalization.h"
#include "LVQ.h"


template<class NeuronType, class NormalizationType = NoNormalization>
class NetworkFileAdapter : public LVQ<NeuronType>
{
	typedef typename NeuronType::value_type value_type;

	struct DataSample
	{
		std::vector<value_type> data;
		value_type expectedOutput;
		bool is_valid;

		DataSample() : data(), expectedOutput(), is_valid(true) {};

		std::istream& FromStream(std::istream& stream, size_t Length, bool training)
		{
			data.clear();
			value_type temp;
			bool lastValueIsOutput = false;

			size_t i = 0;
			for (; i < Length && stream.good(); ++i)
			{
#ifndef NDEBUG
				std::cerr << "i = " << i << ", next char in stream = " << stream.peek() ;
#endif

				stream >> temp;

#ifndef NDEBUG
				std::cerr << ", next value = " << temp << '\r';
#endif
				data.emplace_back(temp);
			}
			if (training && (stream.peek() != '\n') && stream.good())
			{
				stream >> expectedOutput;
				lastValueIsOutput = true;
#ifndef NDEBUG
				std::cerr << "Extracted following output = " << expectedOutput << '\r';

#endif
			}
			else
			{
#ifndef NDEBUG
				std::cerr << "No output to extract\n";
#endif
			}
			if (lastValueIsOutput!=training)///XOR
			{
				is_valid = false;
#ifndef NDEBUG
				std::stringstream errorMessage;
				errorMessage << std::boolalpha << "Expected training == " << training << " but it is == " << lastValueIsOutput;
				errorMessage << "\nFile invalidated.";
				throw std::runtime_error(errorMessage.str());
#endif
			}
			else
			{
#ifndef NDEBUG
				std::stringstream DebugMessage;
				DebugMessage << "is ";
				if (training)
				{
					DebugMessage << "training sample with " << data.size() <<" values, expected output = " << expectedOutput ;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
					/// This is windows, need to clear console after carriage return
				DebugMessage << "                      ";
#endif
				}
				else
				{
					DebugMessage << "sample with " << data.size() << " values, no expected output provided.";
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
					/// This is windows, need to clear console after carriage return
					DebugMessage << "                                            ";
#endif
				}
				std::cerr << DebugMessage.str() << '\n' << '\n';
#endif
			}
			if (is_valid)
				NormalizationType::Normalize(data.begin(), data.end(), data.begin());/// Invoke static function (that should be) common for all Normalization functors, do normalization 'in place'.
			return stream;
		}

		friend struct FileAdapter;
	};

	struct FileAdapter
	{
		friend class NetworkFileAdapter;


		std::vector< DataSample > samples;

		size_t samplesNo;
		size_t sampleLength;
		bool training;

		FileAdapter() : samples(), samplesNo(), sampleLength(), training(false) {};
		FileAdapter(const std::string& FilePath) : samples(), samplesNo(), sampleLength(), training(false) { ReadFile(FilePath); };
		virtual ~FileAdapter(){};

		void ReadFile(const std::string& FilePath)
		{
			std::ifstream File(FilePath);
			if (!File.is_open())
				throw std::runtime_error("Failed to open file " + FilePath + " in read mode.");
			else
			{
				DataSample temp;
				std::string line;
				std::stringstream buffor;

				std::getline(File, line);
				buffor << line;

				buffor >> samplesNo >> sampleLength;
				if (buffor.str().find('1') != std::string::npos)
					buffor >> training;				

				for (size_t i = 0; std::getline(File, line) && i < samplesNo; ++i)
				{
					buffor.str(std::string());
					buffor.clear();

					assert(buffor.str().size() == 0);// bufor must be empty

					buffor << line;
#ifndef NDEBUG
					std::cerr << "Line: ( " << line << ")\n";
#endif
					temp.FromStream(buffor, sampleLength, training);
					buffor.clear();
					if (temp.is_valid)
						samples.emplace_back(temp);
					else
						std::cerr << "Cannot add file " << FilePath << ", file invalid.\n";
				}
				File.close();
			}
		}
	};

	std::map<std::string, FileAdapter> allFiles;

public:

	NetworkFileAdapter() : allFiles() {}
	NetworkFileAdapter(const std::string& Path) : allFiles() 
	{ 
		try
		{
			CreateNetworkOnFile(Path);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	static std::string getFilenameWithoutExtension(std::string str)
	{
		auto pos = str.find_last_of("/\\");
		if (pos != std::string::npos)
			str.erase(str.begin(), str.begin() + pos + 1);
		pos = str.find_last_of(".");
		if (pos != std::string::npos)
			str.erase(str.begin() + pos, str.end());
		return str;
	}
	template <typename type>
	static std::string getPathFromStandardInput(const std::string& leading_to_what = "file") /// get valid path from user
	{
			std::string temporal;
			std::cout << "Enter " << leading_to_what << " path: ";
			std::cin >> temporal;
			std::ifstream file_test(temporal.c_str());
			while (!file_test.is_open() && temporal != "end")
			{
				std::cout << "File could not be opened (does it exist?). Enter path again or 'end' to exit: ";
				std::cin >> temporal;
				file_test.open(temporal.c_str());
			}
			file_test.close();
			return temporal;
		}
	void ReadFile(const std::string& FilePath)
	{

		auto name = NetworkFileAdapter::getFilenameWithoutExtension(FilePath);
		FileAdapter temp;
		try
		{
			std::cout << "Opening file " << FilePath << " ... " << '\r';
			temp = FileAdapter(FilePath);
			allFiles[name] = temp;
			std::cout << "Opened " << FilePath << " succesfully." << '\n';
		}
		catch (const std::runtime_error& re)
		{
			std::cerr << re.what() << '\n';
			return;
		}
		catch (const std::exception& ex)
		{
			std::cerr << ex.what() << '\n';
			return;
		}
	}

	void LearnFile(const std::string& FilePath)
	{
		std::cout << "Learning...\n";
		auto name = NetworkFileAdapter::getFilenameWithoutExtension(FilePath);

		if (allFiles.count(name) == 0)/// no need to re-open file if it already is in our map
			ReadFile(FilePath);

		if (LVQ<NeuronType>::network.empty())
		{
			CreateNetworkOnFile(FilePath);
		}
		else if (allFiles[name].training)
		{
			for (auto &sample : allFiles[name].samples)
			{
#ifndef NDEBUG
				std::cerr << "Learning " << sample.expectedOutput << ")\n";
#endif
				LVQ<NeuronType>::WTALearn(sample.data.begin(), sample.data.end(), sample.expectedOutput);
			}
			std::cout << "Learned " << name << " succesfully." << '\n';
		}
		else
		{
			throw std::invalid_argument(name + " is not validation nor training file, cannot learn it.");
		}
	}

	void CreateNetworkOnFile(const std::string& FilePath)
	{
		auto name = NetworkFileAdapter::getFilenameWithoutExtension(FilePath);

		if (allFiles.count(name) == 0)/// no need to re-open file if data already is in our map
			ReadFile(FilePath);
#ifndef NDEBUG
		std::cerr << "Number of files in memory = " << allFiles.size() << '\n';
		std::cerr << "Number of samples in file " << name << " = " << allFiles[name].samples.size() << '\n';
		const NeuronType * debugedNeuron;
#endif
		for (auto &sample : allFiles[name].samples)
		{
#ifndef NDEBUG
			std::cerr << "Adding neuron with " << sample.data.size() << " dendrites." << '\n';
#endif
			if (allFiles[name].training)/// let the output be known to neuron
			{
#ifndef NDEBUG

				std::cerr << "Learning data, desired output = " << sample.expectedOutput <<'\n';
				debugedNeuron = &
#endif
				LVQ<NeuronType>::AddNeuron(sample.data.begin(), sample.data.end(), sample.expectedOutput);
				assert(sample.expectedOutput == debugedNeuron->Output().Value());
				
			}
			else
			{
#ifndef NDEBUG
				std::cerr << "It's not learning data, desired output unknown\n";
				debugedNeuron = &/* line to be continued.. */
#endif
				LVQ<NeuronType>::AddNeuron(sample.data.begin(), sample.data.end()); /// it's not learning data, desired output unknown.
			}
			assert(sample.data.size() == debugedNeuron->Weights().size());
			assert(debugedNeuron->Weights() == sample.data);
#ifndef NDEBUG
			std::cerr << "Neuron with Output = " << static_cast<typename NeuronType::value_type>(debugedNeuron->Output()) << " added." << '\n';
#endif
		}

		std::cout << "Created network with "<< LVQ<NeuronType>::network.size() << " neurons, based on input from " << name << " succesfully." << '\n';
	}

	std::string Validate(const std::string& FilePath)
	{
		std::cout << "Starting validation:\n";
		if (LVQ<NeuronType>::network.empty()) throw std::logic_error("Nothing to validate, network has no neurons.");

		auto name = NetworkFileAdapter::getFilenameWithoutExtension(FilePath);

		if (allFiles.count(name) == 0)
			ReadFile(FilePath); /// no need to reopen file if data already is in our map
#ifndef NDEBUG
		std::cerr << "Number of files in memory = " << allFiles.size() << '\n';
		std::cerr << "Number of samples in file " << name << " = " << allFiles[name].samples.size() << '\n';
#endif
		if (!allFiles[name].training) throw std::invalid_argument(FilePath + " is not validation nor training file.");

		std::stringstream sstr;
		NeuronType * temp;
		auto i = 0;
		auto error = 0.0;
		auto temp_error = 0.0;
		for (auto &sample : allFiles[name].samples)
		{
			temp = &LVQ<NeuronType>::BMU(sample.data.begin(), sample.data.end());

			sstr << "Validation " << ++i << ": expected: " << sample.expectedOutput << " received: " << temp->Output().Value() << ", error: " << (temp_error = (pow(sample.expectedOutput - temp->Output().Value(), 2.0))) << '\n';
#ifndef NDEBUG
			std::cerr << "(std::cerr) Validation " << i << ": expected: " << sample.expectedOutput << " received: " << temp->Output().Value() << ", error: " << (temp_error = (pow(sample.expectedOutput - temp->Output().Value(), 2.0))) << '\n';
#endif
			error += temp_error;
		}

		std::cout << "Network validation yielded mean square error = " << error / i << '\n';
		std::cout << "Network size: " << LVQ<NeuronType>::network.size() << '\n';
		return sstr.str(); /// C++ 11 move
	}
};
#endif
