//7. Имеется  текстовый  файл  с  некоторым  описанием.Все
//предложения   заканчиваются   точкой.Требуется   проверить,
//является  ли  первая  буква  каждого  предложения   прописной.
//Исправить обнаруженные ошибки(6). Сорокин Роман ПС-23. Microsoft Visual Studio.

#include <iostream>
#include <fstream>
#include <string>
#include <cwctype>
#include <windows.h>

using namespace std;

wchar_t Rtoupper(wchar_t s)
{
	if (224 <= (unsigned char)s && (unsigned char)s <= 255) // диапазон [224, 255] маленьких русских букв
		s -= 32; // сдвиг на большие русские буквы
	else if ((unsigned char)s == 184) // номер ё
		s -= 16; // номер больших ё
	else if ((65 + 32) <= s && s <= (90 + 32)) // латинские буквы
		s -= 32; // сдвиг на большие латинские буквы
	return s;
}

void CapitalizeSentence(wifstream& input, wofstream& output)
{
	wstring sentence;
	while (!input.eof())
	{
		getline(input, sentence);
		int i, j;
		sentence[0] = Rtoupper(sentence[0]);

		for (i = 0; i < sentence.length(); i++)
		{
			j = i;

			if (sentence[i] == '.' || sentence[i] == '?' || sentence[i] == '!')
			{
				i++;
				j++;
				sentence[j] = Rtoupper(sentence[j]);

				if (sentence[j] == ' ')
				{
					j++;
					sentence[j] = Rtoupper(sentence[j]);
				}
			}
		}

		output << sentence << endl;
	}
}

int main(int argc, char* argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Russian");

	if (argc != 3)
	{
		std::cout << "Invalid arguments count\n";
		std::cout << "Usage <inputf file name> <output file name>\n";
		return 1;
	}

	string inputFileName = argv[1];
	string outputFileName = argv[2];

	std::wifstream input;
	input.open(inputFileName);
	if (!input.is_open())
	{
		std::cout << "Failed to open '" << inputFileName << "' for reading\n";
		return 1;
	}

	std::wofstream output;
	output.open(outputFileName);
	if (!output.is_open())
	{
		std::cout << "Failed to open '" << outputFileName << "' for writing\n";
	}

	CapitalizeSentence(input, output);
	
	if (input.bad())
	{
		std::cout << "Failed to read data from input file\n";
		return 1;
	}

	if (!output.flush())
	{
		std::cout << "Failed to write data to output file\n";
		return 1;
	}

	return 0;
} 