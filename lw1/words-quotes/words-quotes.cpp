#include <iostream>
#include <fstream>
#include <string>

using namespace std;

bool IsWordUpper(const string& word)
{
	for (auto ch : word)
	{
		if (!isupper(ch))
		{
			return false;
		}
	}

	return true;
}

string ToLowerWord(const string& word)
{
	string result;

	for (auto ch : word)
	{
		result += tolower(ch);
	}
	
	return result;
}

void QuoteWords(ifstream& input, ofstream& output)
{
	string word;
	bool period;
	while (input >> word)
	{
		period = false;
		if (word[word.size() - 1] == '.')
		{
			word = word.substr(0, word.size() - 1);
			period = true;
		}

		if (IsWordUpper(word))
		{
			output << "\"" << ToLowerWord(word) << "\"";
		}
		else
		{
			output << word;
		}

		if (period)
		{
			output << '.';
		}
		output << ' ';
	}
}

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cout << "Invalid arguments count\n";
		std::cout << "Usage <inputf file name> <output file name>\n";
		return 1;
	}

	string inputFileName = argv[1];
	string outputFileName = argv[2];

	std::ifstream input;
	input.open(inputFileName);
	if (!input.is_open())
	{
		std::cout << "Failed to open '" << inputFileName << "' for reading\n";
		return 1;
	}

	std::ofstream output;
	output.open(outputFileName);
	if (!output.is_open())
	{
		std::cout << "Failed to open '" << outputFileName << "' for writing\n";
	}

	QuoteWords(input, output);

	if (input.bad())
	{
		std::cout << "Failed to read data form input file\n";
		return 1;
	}

	if (!output.flush())
	{
		std::cout << "Failed to write data to output file\n";
		return 1;
	}

	return 0;
}