//5. Имеется дерево, корень которого соответствует основателю
//рода.Сыновья  каждой  вершины  задают  сыновей   и   дочерей
//соответствующего    человека.Указывается   имя   некоторого
//человека.Требуется выдать имена его детей, внуков, сестер и
//братьев, одного из родителей, дедушки или бабушки(9).
// <Фамилия Имя> <Группа>
// Visual Studio 2022 C++ 20

#include <iostream>
#include <vector>
#include <optional>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

constexpr char LEVEL_CHAR = '.';

template <typename T>
struct Tree
{
	T value;
	int level;
	vector<Tree<T>*> sons;
};

struct Relatives
{
	Tree<wstring>* grandparent;
	Tree<wstring>* parent;
	Tree<wstring>* person;
};

struct Args
{
	string inputFileName;
};

optional<Args> ParseArgs(int argc, char* argv[]);
Tree<wstring>* ReadTreeFromFile(string fileName);
void PrintTree(wostream& out, Tree<wstring>* root);
optional<Relatives> FindRelatives(wostream& out, Tree<wstring>* root, wstring const& name, Tree<wstring>** parent = nullptr, Tree<wstring>** grandparent = nullptr);
void PrintRelatives(wostream& out, optional<Relatives> const& relatives, wstring const& name);

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
			wstring name;
			optional<Relatives> relatives;
			int command;

			wcout << "Распечатать дерево - 1" << endl;
			wcout << "Найти родственников - 2" << endl;
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
				wcout << "Введите имя: ";
				wcin >> name;

				relatives = FindRelatives(wcout, root, name);
				if (!relatives)
				{
					wcout << "Человек не найден" << endl;
					break;
				}
				PrintRelatives(wcout, relatives, name);

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
		currentNode->value = ReadValueFromString(line);

		if (currentNode->level == 0)
		{
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
	out << wstring(root->level, '.') << root->value  << endl;

	for (auto& son : root->sons)
	{
		PrintTree(out, son);
	}
}

optional<Relatives> FindRelatives(wostream& out, Tree<wstring>* root, wstring const& name, Tree<wstring>** parent, Tree<wstring>** grandparent)
{
	optional<Relatives> result;
	Relatives res;

	// если имя нашлось
	if (root->value == name)
	{
		res.grandparent = grandparent ? *grandparent : nullptr;
		res.parent = parent ? *parent : nullptr;
		res.person = root;
		result = res;

		return result;
	}

	// смотрим всех детей рекурсивно
	// если есть результат, то возвращаем
	for (auto& son : root->sons)
	{
		result = FindRelatives(out, son, name, &root, parent);
		if (result)
		{
			return result;
		}
	}

	return result;
}

void PrintRelatives(wostream& out, optional<Relatives> const& relatives, wstring const& name)
{
	if (relatives->grandparent != nullptr)
	{
		out << "Дедушка или бабушка: " << relatives->grandparent->value << endl;
	}
	if (relatives->parent != nullptr)
	{
		out << "Родитель: " << relatives->parent->value << endl;
	}

	out << "Братья и сестры: ";
	for (auto& bro : relatives->parent->sons)
	{
		if (bro->value != name)
		{
			out << bro->value << ' ';
		}
	}
	out << endl;

	out << "Человек: " << relatives->person->value << endl;

	out << "Дети: ";
	for (auto& son : relatives->person->sons)
	{
		wcout << son->value << ' ';
	}
	out << endl;

	out << "Внуки: ";
	for (auto& son : relatives->person->sons)
	{
		for (auto& grandson : son->sons)
		{
			out << grandson->value << ' ';
		}
	}
	out << endl;
}