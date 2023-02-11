#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <optional>
#include <cmath>

using namespace std;

struct Args
{
	std::string inputFileName;
	std::string outputFileName;
};

std::optional<Args> ParseArgs(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cout << "Invalid arguments count\n";
		std::cout << "Usage copyfile.exe <inputf file name> <output file name>\n";
		return std::nullopt;
	}
	Args args;
	args.inputFileName = argv[1];
	args.outputFileName = argv[2];
	return args;
}

void scientificNumber(double num, int decimalPlaces)
{
	int eToThe = 0;
	if (abs(num) >= 1)
	{
		for (; abs(num) > 9; ++eToThe)
		{
			num /= 10;
		}
	}
	else
	{
		for (; abs(num) < 1; --eToThe)
		{
			num *= 10;
		}
	}
	

	cout << fixed << setprecision(decimalPlaces) << num << "e" << eToThe;
}

int main(int argc, char* argv[])
{
	auto args = ParseArgs(argc, argv);
	// Проверка правильности аргументов командной строки
	if (!args)
	{
		return 1;
	}

	// Открываем входной файл
	std::ifstream input;
	input.open(args->inputFileName);
	if (!input.is_open())
	{
		std::cout << "Failed to open '" << args->inputFileName << "' for reading\n";
		return 1;
	}

	double num = 0.0;
	int decimalPlaces;
	input >> num;
	input >> decimalPlaces;
	scientificNumber(num, decimalPlaces);
	
	if (input.bad())
	{
		std::cout << "Failed to read data form input file\n";
		return 1;
	}

	return 0;
}