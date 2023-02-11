//9. Структура  некоторого института задана деревом.Сыновьям
//корневой вершины соответствуют факультеты, факультеты в  свою
//очередь  делятся  на  кафедры, которые  могут  иметь филиалы.
//Листьям   дерева    соответствуют    преподаватели.Выявить
//преподавателей, ведущих занятия на трех и более кафедрах(10).
// <Фамилия Имя> <Группа>
// Visual Studio 2022 C++ 20

#include <iostream>
#include <vector>
#include <optional>
#include <fstream>
#include <string>
#include <windows.h>
#include <algorithm>
#include <set>

using namespace std;

constexpr char LEVEL_CHAR = '.';
const int TEACHER_MAX_DEP_NUM = 3;

enum class NodeType
{
	INSTITUTE = 'i',
	FACULTY = 'f',
	DEPARTMENT = 'd',
	BRANCH = 'b',
	TEACHER = 't',
};

template <typename T>
struct Tree
{
	NodeType type;
	T value;
	int level;
	vector<Tree<T>*> sons;
};

struct Teacher
{
	wstring name;
	set<wstring> deps;
};

struct Args
{
	string inputFileName;
};

optional<Args> ParseArgs(int argc, char* argv[]);
Tree<wstring>* ReadTreeFromFile(string fileName);
void PrintTree(wostream& out, Tree<wstring>* root);
void PrintTeachers(wostream& out, Tree<wstring>* root);

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
			wcout << "Распечатать учителей - 2" << endl;
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
				PrintTeachers(wcout, root);
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
	case NodeType::BRANCH:
		return 'b';
	case NodeType::TEACHER:
		return 't';
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
		return NodeType::BRANCH;
	case 4:
		return NodeType::TEACHER;
	default:
		throw exception("unknown level node type: " + level);
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
		currentNode->type = NodeTypeByLevel(currentNode->level);
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
	out << wstring(root->level, '.') << root->value << ' ' << NodeTypeToChar(root->type) << endl;
	for (auto& son : root->sons)
	{
		PrintTree(out, son);
	}
}

void CountTeacherDeps(Tree<wstring>* root, vector<Teacher>& teachers, wstring& currDep)
{
	// текущая кафедра
	if (root->type == NodeType::DEPARTMENT)
	{
		currDep = root->value;
	}

	// если учитель
	if (root->type == NodeType::TEACHER)
	{
		wstring teacherName = root->value;
		auto it = find_if(teachers.begin(), teachers.end(), [teacherName](Teacher tchr) { return tchr.name == teacherName; });
		
		// если нашелся учитель в вектрое учителей
		if (it != teachers.end())
		{
			// добавление кафедры в множество кафедр
			teachers[distance(teachers.begin(), it)].deps.insert(currDep);
		}
		else
		{
			// добавить нового учителя в вектор
			Teacher tchr;
			tchr.name = root->value;
			tchr.deps.insert(currDep);
			teachers.push_back(tchr);
		}
	}

	for (auto& son : root->sons)
	{
		CountTeacherDeps(son, teachers, currDep);
	}
}

void PrintTeachers(wostream& out, Tree<wstring>* root)
{
	vector<Teacher> teachers;
	wstring currDep;

	// просчитать какой учитель на каких кафедрах работает
	CountTeacherDeps(root, teachers, currDep);

	for (auto& teacher : teachers)
	{
		int teacherDepNum = teacher.deps.size();

		// если больше чем на 3 кафедрах - печать
		if (teacherDepNum >= TEACHER_MAX_DEP_NUM)
		{
			out << teacher.name << ' ' << teacherDepNum << endl;

		}
	}
}