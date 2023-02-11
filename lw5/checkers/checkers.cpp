//20. На  шахматном поле расположены N черных  и  одна  белая
//шашка.Требуется  написать  программу, которая  по  заданному
//расположению шашек определяет, какое  максимальное  количество
//шашек может взять белая шашка за один ход.
//Ввод из файла INPUT.TXT.Задается 8 строк, каждая  содержит
//8 символов.Символом ‘0’ обозначается пустая клетка, символом
//‘1’ положение черной шашки и символом ‘2’ положение белой шашки.
//Вывод  в  файл  OUTPUT.TXT.Вывести  единственное  число –
//максимальное количество черных шашек, которых можно  взять  за
//один ход(11).
// Худяков Даниил ПС-23
// Visual Studio 2022 C++ 20
#include <iostream>
#include <optional>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

constexpr int FIELD_SIZE = 8;

constexpr char EMPTY_CELL = '0';
constexpr char WHITE_CHECKER = '1';
constexpr char BLACK_CHECKER = '2';

using Field = array<char, FIELD_SIZE* FIELD_SIZE>;

struct Args
{
	string inputFileName;
};

template<typename T>
struct GraphVertex
{
	T value;
	vector<GraphVertex<T>*> sons;
};

template <typename T>
T FindMaxCapturing(GraphVertex<T>* graph)
{
	T result = graph->value;
	T value;

	for (auto& son : graph->sons)
	{
		value = FindMaxCapturing(son);
		if (result < value)
		{
			result = value;
		}
	}

	return result;
}

void PrintCheckboard(Field field)
{
	for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++)
	{
		cout << field[i];
		if ((i + 1) % FIELD_SIZE == 0)
		{
			cout << endl;
		}
	}
}

optional<Args> ParseArgs(int argc, char* argv[]);
Field ReadFieldFromFile(string fileName);
void GenerateCaptures(Field const& field, int checkerIndex, GraphVertex<int>* graph);

int main(int argc, char* argv[])
{
	auto args = ParseArgs(argc, argv);

	if (!args)
	{
		cout << "Invalid input file name" << endl;
		return -1;
	}

	try
	{
		Field field = ReadFieldFromFile(args->inputFileName);

		int blackCheckerIndex = distance(field.begin(), std::find(field.begin(), field.end(), '2'));
		GraphVertex<int> graph{ 0 };

		bool end = false;
		while (!end)
		{
			int command;

			wcout << "Print checkboard - 1" << endl;
			wcout << "Find max capturing - 2" << endl;
			wcout << "End work - 3" << endl;
			wcout << "Enter command: ";
			cin >> command;
			cout << endl;

			switch (command)
			{
			case 1:
				PrintCheckboard(field);
				break;
			case 2:
				GenerateCaptures(field, blackCheckerIndex, &graph);

				cout << "Answer: " << FindMaxCapturing(&graph) << endl;
				break;
			case 3:
				end = true;
				break;
			default:
				cout << "Unknown command: " << command << endl;
				break;
			}
			cout << endl;
		}
	}
	catch (exception const& e)
	{
		cout << e.what() << endl;
	}

	return 0;
}

optional<Args> ParseArgs(int argc, char* argv[])
{
	if (argc != 2)
	{
		return nullopt;
	}

	Args args;
	args.inputFileName = argv[1];

	return args;
}

Field ReadFieldFromFile(istream& file)
{
	string line;
	Field field;

	int i = 0;
	while (getline(file, line))
	{
		if (line.empty() && line.length() < FIELD_SIZE)
		{
			continue;
		}

		for (int j = 0; j < FIELD_SIZE; j++)
		{
			field[i * FIELD_SIZE + j] = line[j];
		}

		i++;
	}

	return field;
}

Field ReadFieldFromFile(string fileName)
{
	ifstream inputFile(fileName);

	if (!inputFile.is_open())
	{
		throw exception("cannot open input file");
	}

	return ReadFieldFromFile(inputFile);
}

bool CanMoveToIndex(int index)
{
	return index >= 0 && index <= 63;
}

constexpr int RIGHT_TOP_MOVE = -7;
constexpr int RIGHT_BOTTOM_MOVE = 9;
constexpr int LEFT_TOP_MOVE = -9;
constexpr int LEFT_BOTTOM_MOVE = 7;

void GenerateCaptures(Field const& field, int checkerPosIndex, GraphVertex<int>* graph)
{
	int rightTopPosIndex = checkerPosIndex + RIGHT_TOP_MOVE;
	if (CanMoveToIndex(rightTopPosIndex))
	{
		int posAfterCepturing = rightTopPosIndex + RIGHT_TOP_MOVE;
		bool isBorderPos = ((rightTopPosIndex + 1) % FIELD_SIZE == 0);
		bool canCapture = field[rightTopPosIndex] == WHITE_CHECKER
			&& CanMoveToIndex(posAfterCepturing)
			&& field[posAfterCepturing] == EMPTY_CELL
			&& !isBorderPos
			&& posAfterCepturing != 0;
		if (canCapture)
		{
			GraphVertex<int>* newCapture = new GraphVertex<int>({ graph->value + 1 });
			graph->sons.push_back(newCapture);
			Field newField = field;

			newField[checkerPosIndex] = EMPTY_CELL;
			newField[rightTopPosIndex] = EMPTY_CELL;
			newField[posAfterCepturing] = BLACK_CHECKER;

			GenerateCaptures(newField, posAfterCepturing, newCapture);
		}
	}

	int rightBottomPosIndex = checkerPosIndex + RIGHT_BOTTOM_MOVE;
	if (CanMoveToIndex(rightBottomPosIndex))
	{
		int posAfterCapturing = rightBottomPosIndex + RIGHT_BOTTOM_MOVE;
		bool isBorderPos = ((rightBottomPosIndex + 1) % FIELD_SIZE == 0);
		bool canCapture = field[rightBottomPosIndex] == WHITE_CHECKER
			&& CanMoveToIndex(posAfterCapturing)
			&& !isBorderPos
			&& field[posAfterCapturing] == EMPTY_CELL;
		if (canCapture)
		{
			GraphVertex<int>* newCapture = new GraphVertex<int>({ graph->value + 1 });
			graph->sons.push_back(newCapture);
			Field newField = field;

			newField[checkerPosIndex] = EMPTY_CELL;
			newField[rightBottomPosIndex] = EMPTY_CELL;
			newField[posAfterCapturing] = BLACK_CHECKER;

			GenerateCaptures(newField, posAfterCapturing, newCapture);
		}
	}

	int leftTopPosIndex = checkerPosIndex + LEFT_TOP_MOVE;
	if (CanMoveToIndex(leftTopPosIndex))
	{
		int posAfterCapturing = leftTopPosIndex + LEFT_TOP_MOVE;
		bool isBorderPos = (leftTopPosIndex % FIELD_SIZE == 0);
		bool canCapture = field[leftTopPosIndex] == WHITE_CHECKER
			&& CanMoveToIndex(posAfterCapturing)
			&& posAfterCapturing != 63
			&& field[posAfterCapturing] == EMPTY_CELL;
		if (canCapture)
		{
			GraphVertex<int>* newCapture = new GraphVertex<int>({ graph->value + 1 });
			graph->sons.push_back(newCapture);
			Field newField = field;

			newField[checkerPosIndex] = EMPTY_CELL;
			newField[leftTopPosIndex] = EMPTY_CELL;
			newField[posAfterCapturing] = BLACK_CHECKER;

			GenerateCaptures(newField, posAfterCapturing, newCapture);
		}
	}

	int leftBottomPosIndex = checkerPosIndex + LEFT_BOTTOM_MOVE;
	if (CanMoveToIndex(leftBottomPosIndex))
	{
		int posAfterCapturing = leftBottomPosIndex + LEFT_BOTTOM_MOVE;
		bool isBorderPos = (leftBottomPosIndex % FIELD_SIZE == 0);
		bool canCapture = field[leftBottomPosIndex] == WHITE_CHECKER
			&& CanMoveToIndex(posAfterCapturing)
			&& !isBorderPos
			&& field[posAfterCapturing] == EMPTY_CELL;
		if (canCapture)
		{
			GraphVertex<int>* newCapture = new GraphVertex<int>({ graph->value + 1 });
			graph->sons.push_back(newCapture);
			Field newField = field;

			newField[checkerPosIndex] = EMPTY_CELL;
			newField[leftBottomPosIndex] = EMPTY_CELL;
			newField[posAfterCapturing] = BLACK_CHECKER;

			GenerateCaptures(newField, posAfterCapturing, newCapture);
		}
	}
}