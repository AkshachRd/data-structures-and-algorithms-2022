//13. В   некотором   институте   информация   об   имеющихся
//компьютерах  задана   деревом.Сыновьям   корневой   вершины
//соответствуют факультеты, факультеты в  свою очередь  делятся
//на  кафедры, кафедры  могут иметь в своем составе лаборатории.
//Компьютеры могут быть установлены в общих факультетских классах,
//на кафедрах, в  лабораториях  и идентифицируются  уникальными
//номерами.Требуется найти :
//1) факультеты с минимальным и максимальным числом компьютеров;
//2) кафедры с минимальным и максимальным числом компьютеров(11).
// <Фамилия Имя> <Группа>
// Visual Studio 2022 C++ 20

#include <iostream>
#include <vector>
#include <optional>
#include <fstream>
#include <string>
#include <windows.h>
#include <limits>

using namespace std;

constexpr char LEVEL_CHAR = '.';
const int MAX_INT = (numeric_limits<int>::max)();

enum class NodeType
{
	INSTITUTE = 'i',
	FACULTY = 'f',
	DEPARTMENT = 'd',
	LABORATORY = 'l',
	COMPUTER = 'c',
};

template <typename T>
struct Tree
{
	NodeType type;
	T value;
	int level;
	vector<Tree<T>*> sons;
};

template <typename T>
struct MinMaxValue
{
	T minVal;
	T maxVal;
	wstring minName;
	wstring maxName;
};

struct Args
{
	string inputFileName;
};

optional<Args> ParseArgs(int argc, char* argv[]);
Tree<wstring>* ReadTreeFromFile(string fileName);
void PrintTree(wostream& out, Tree<wstring>* root);
void PrintMinMax(wostream& out, Tree<wstring>* root);

int main(int argc, char* argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	auto args = ParseArgs(argc, argv);

	if (!args)
	{
		cout << "Invalid input file name" << endl;
		return -1;
	}

	try
	{
		// чтение дерева из файла
		Tree<wstring>* root = ReadTreeFromFile(args->inputFileName);
		
		bool end = false;
		while (!end)
		{
			int command;

			wcout << "Распечатать дерево - 1" << endl;
			wcout << "Распечатать MinMax - 2" << endl;
			wcout << "Завершить работу - 3" << endl;
			wcout << "Введите комманду: ";
			cin >> command;
			cout << endl;

			switch (command)
			{
			case 1:
				PrintTree(wcout, root);
				break;
			case 2:
				PrintMinMax(wcout, root);
				break;
			case 3:
				end = true;
				break;
			default:
				wcout << "Неизвестная комманда: " << command << endl;
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

wchar_t NodeTypeToChar(NodeType type)
{
	switch (type)
	{
	case NodeType::INSTITUTE:
		return 'i';
	case NodeType::FACULTY:
		return 'f';
	case NodeType::DEPARTMENT:
		return 'd';
	case NodeType::LABORATORY:
		return 'l';
	case NodeType::COMPUTER:
		return 'c';
	default:
		throw exception("unknown node type");
	}
}

int ReadLevelFromString(wstring const& str)
{
	int level = 0;

	for (auto ch : str)
	{
		if (ch != LEVEL_CHAR)
		{
			break;
		}
		level++;
	}

	return level;
}

wstring ReadValueFromString(wstring const& str)
{
	wstring value;

	for (auto i = str.find_first_not_of(LEVEL_CHAR); i < str.length(); i++)
	{
		value += str[i];
	}

	return value;
}

NodeType NodeTypeByLevel(int level)
{
	switch (level)
	{
	case 0:
		return NodeType::INSTITUTE;
	case 1:
		return NodeType::FACULTY;
	case 2:
		return NodeType::DEPARTMENT;
	case 3:
		return NodeType::LABORATORY;
	case 4:
		return NodeType::COMPUTER;
	default:
		throw exception("unknown level node type: " + level);
	}
}

int NodeTypeToLevel(NodeType type)
{
	switch (type)
	{
	case NodeType::INSTITUTE:
		return 0;
	case NodeType::FACULTY:
		return 1;
	case NodeType::DEPARTMENT:
		return 2;
	case NodeType::LABORATORY:
		return 3;
	case NodeType::COMPUTER:
		return 4;
	default:
		throw exception("unknown node type");
	}
}

Tree<wstring>* ReadTreeFromFile(wifstream& file, Tree<wstring>*& root, int currentLevel = 0, Tree<wstring>** prevNode = nullptr)
{
	wstring line;
	Tree<wstring>* currentNode = nullptr;
	Tree<wstring>* previousNode = prevNode ? *prevNode : nullptr;

	while (getline(file, line))
	{
		if (line.empty())
		{
			continue;
		}
		currentNode = new Tree<wstring>();
		currentNode->level = ReadLevelFromString(line);
		currentNode->type = NodeType::COMPUTER;
		currentNode->value = ReadValueFromString(line);

		if (currentNode->level == 0)
		{
			currentNode->type = NodeTypeByLevel(currentNode->level);
			root = currentNode;
			previousNode = root;
			continue;
		}

		if ((currentNode->level - currentLevel) == 1)
		{
			root->sons.push_back(currentNode);
		}

		if ((currentNode->level - currentLevel) > 1)
		{
			previousNode->type = NodeTypeByLevel(previousNode->level);
			previousNode->sons.push_back(currentNode);
			Tree<wstring>* notChildNode = ReadTreeFromFile(file, previousNode, currentLevel + 1, &currentNode);

			if (notChildNode)
			{
				if ((notChildNode->level - currentLevel) < 1)
				{
					return notChildNode;
				}
				else
				{
					root->sons.push_back(notChildNode);
					previousNode = notChildNode;
					continue;
				}
			}
		}

		if ((currentNode->level - currentLevel) < 1)
		{
			return currentNode;
		}
		previousNode = currentNode;
	}

	return nullptr;
}

Tree<wstring>* ReadTreeFromFile(string fileName)
{
	Tree<wstring>* root = nullptr;

	wifstream inputFile(fileName);

	if (!inputFile.is_open())
	{
		throw exception("cannot open input file");
	}

	if (ReadTreeFromFile(inputFile, root))
	{
		throw exception("something went wrong");
	}

	return root;
}

void PrintTree(wostream& out, Tree<wstring>* root)
{
	out << wstring(root->level, '.') << root->value << ' ' << NodeTypeToChar(root->type) << endl;

	for (auto& son : root->sons)
	{
		PrintTree(out, son);
	}
}

int CountComputers(Tree<wstring>* root)
{
	int count = 0;

	for (auto& son : root->sons)
	{
		if (son->type == NodeType::COMPUTER)
		{
			count++;
		}

		count += CountComputers(son);
	}

	return count;
}

MinMaxValue<int> FindMinMaxComputers(vector<Tree<wstring>*> const& nodes)
{
	int maxComps = 0;
	wstring maxCompsName;
	int minComps = MAX_INT;
	wstring minCompsName;

	for (auto& node : nodes)
	{
		// подсчёт количества компьютеров в поддереве узла
		int compsCount = CountComputers(node);

		if (maxComps < compsCount)
		{
			maxComps = compsCount;
			maxCompsName = node->value;
		}

		if (compsCount < minComps)
		{
			minComps = compsCount;
			minCompsName = node->value;
		}
	}

	MinMaxValue<int> comps;
	comps.maxVal = maxComps;
	comps.maxName = maxCompsName;
	comps.minVal = minComps;
	comps.minName = minCompsName;

	return comps;
}

void PrintMinMax(wostream& out, Tree<wstring>* root)
{
	// получить min max значения и названия факультетов
	MinMaxValue<int> facComps = FindMinMaxComputers(root->sons);

	// проверка остались ли значения начальными
	if (facComps.maxVal == 0 || facComps.minVal == MAX_INT)
	{
		out << "Факультет не найден" << endl;
	}
	else
	{
		out << "Факультет:" << endl;
		out << "Max: " << facComps.maxName << " " << facComps.maxVal << endl;
		out << "Min: " << facComps.minName << " " << facComps.minVal << endl;
	}

	MinMaxValue<int> depComps;
	depComps.maxVal = 0;
	depComps.minVal = MAX_INT;

	for (auto& fac : root->sons)
	{
		// выделение в вектор кафедр из вектора кафедр и факультетских комьютеров
		vector<Tree<wstring>*> deps;
		for (auto& dep : fac->sons)
		{
			if (dep->type == NodeType::DEPARTMENT)
			{
				deps.push_back(dep);
			}
		}

		// получить min max значения и названия кафедр
		MinMaxValue<int> depCompsTemp = FindMinMaxComputers(deps);

		// сравнение min max значений из полученных с текущими
		if (depComps.maxVal < depCompsTemp.maxVal)
		{
			depComps.maxVal = depCompsTemp.maxVal;
			depComps.maxName = depCompsTemp.maxName;
		}

		if (depComps.minVal > depCompsTemp.minVal)
		{
			depComps.minVal = depCompsTemp.minVal;
			depComps.minName = depCompsTemp.minName;
		}
	}

	// проверка остались ли значения начальными
	if (depComps.maxVal == 0 || depComps.minVal == MAX_INT)
	{
		out << "Кафедра не найдена" << endl;
	}
	else
	{
		out << "Кафедра:" << endl;
		out << "Max: " << depComps.maxName << " " << depComps.maxVal << endl;
		out << "Min: " << depComps.minName << " " << depComps.minVal << endl;
	}
}