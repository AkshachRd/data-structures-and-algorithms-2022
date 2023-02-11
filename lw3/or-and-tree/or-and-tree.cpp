//22. Задано   И - ИЛИ   дерево, соответствующее   некоторому
//множеству изделий.Требуется:
//1) найти число изделий, записанное с помощью этого дерева;
//2) если  число  изделий  больше  N, организовать  усечение
//дерева  в  диалоге, спрашивая  пользователя, какие   сыновья
//ИЛИ - вершин отсекать и сообщая, как при этом сокращается число
//изделий.
//Если усекаются  все сыновья ИЛИ - вершины, то отсекается она
//сама и все поддерево, висящее на сыне первой  ИЛИ - вершины  по
//пути к корню дерева).
//УКАЗАНИЕ: если  корни   нескольких   поддеревьев   являются
//сыновьями   И - вершины, то   общее   число   элементов  равно
//произведению  числа  элементов  поддеревьев, а   если   корни
//поддеревьев - сыновья ИЛИ - вершины, то общее число элементов
//находится как сумма числа элементов в поддеревьях.
//Начальное и  конечное  деревья  выдать на экран в наглядном
//виде(13).
// Худяков Даниил ПС-23 Visual Studio 2022 C++ 20

#include <iostream>
#include <vector>
#include <optional>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <windows.h>

using namespace std;

constexpr char LEVEL_CHAR = '.';

enum class NodeType
{
	AND = 'a',
	OR = 'o',
	LEAF = 'l',
};

template<typename T>
struct Tree
{
	NodeType type;
	T value;
	int level;
	vector<Tree<T>*> sons;
	int varints_num;
};

struct Args
{
	string inputFileName;
	int n;
};

optional<Args> ParseArgs(int argc, char* argv[]);
Tree<wstring>* ReadTreeFromFile(string fileName);
void SetVariantsLeafs(Tree<wstring>* ptr);
void NullVariantsLeafs(Tree<wstring>* ptr);
void CountVariants(Tree<wstring>* ptr);
void PrintTree(wostream& out, Tree<wstring>* root);
void Dialog(wostream& out, Tree<wstring>* root, Tree<wstring>* node, Tree<wstring>* prevNode = nullptr);

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
		Tree<wstring>* root = ReadTreeFromFile(args->inputFileName);
		SetVariantsLeafs(root);
		CountVariants(root);

		if (args->n < root->varints_num)
		{
			wcout << "Число изделий, записанных с помощью этого дерева, (" << root->varints_num << ") больше, чем N (" << args->n << ")." << endl;
			PrintTree(wcout, root);
			wcout << endl;

			cin.get();
			Dialog(wcout, root, root);
		}
		else
		{
			wcout << "Число изделий, записанных с помощью этого дерева, (" << root->varints_num << ") меньше, чем N (" << args->n << ")." << endl;
			PrintTree(wcout, root);
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
	if (argc != 3)
	{
		return nullopt;
	}

	Args args;
	args.inputFileName = argv[1];
	args.n = strtol(argv[2], NULL, 10);

	return args;
}

NodeType CharToNodeType(wchar_t ch)
{
	switch (ch)
	{
	case 'o':
		return NodeType::OR;
	case 'a':
		return NodeType::AND;
	case 'l':
		return NodeType::LEAF;
	default:
		throw exception("unknown node type: " + ch);
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

	for (auto i = str.find_first_not_of(LEVEL_CHAR); i < str.find_last_of(' '); i++)
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
		currentNode->type = CharToNodeType(line.back());
		currentNode->value = ReadValueFromString(line);
		currentNode->level = ReadLevelFromString(line);

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

void SetVariantsLeafs(Tree<wstring>* ptr)
{
	for (auto& son : ptr->sons)
	{
		SetVariantsLeafs(son);
	}

	if (ptr->type == NodeType::LEAF)
	{
		ptr->varints_num = 1;
	}
}

void NullVariantsLeafs(Tree<wstring>* ptr)
{
	for (auto& son : ptr->sons)
	{
		NullVariantsLeafs(son);
	}

	if (ptr->type == NodeType::LEAF)
	{
		ptr->varints_num = 0;
	}
}

void CountVariants(Tree<wstring>* ptr)
{
	for (auto& son : ptr->sons)
	{
		CountVariants(son);
	}
	int sum = 0;
	int multi = 1;

	switch (ptr->type)
	{
	case NodeType::OR:
		for (auto& p: ptr->sons)
		{
			sum += p->varints_num;
		}
		ptr->varints_num = sum;
		break;
	case NodeType::AND:
		for (auto& p: ptr->sons)
		{
			if (p->varints_num != 0)
			{
				multi *= p->varints_num;
			}
		}
		ptr->varints_num = multi;
		break;
	case NodeType::LEAF:
		break;
	}
}

void PrintTree(wostream& out, Tree<wstring>* root)
{
	out << wstring(root->level, '.') << root->value << ' ' << root->varints_num << endl;
	for (auto& son: root->sons)
	{
		PrintTree(out, son);
	}
}

void Dialog(wostream& out, Tree<wstring>* root, Tree<wstring>* node, Tree<wstring>* prevNode)
{
	bool deleted = false;

	if (node->type == NodeType::OR)
	{
		auto it = node->sons.begin();
		while (!deleted && it != node->sons.end())
		{
			Tree<wstring>* son = *it;

			NullVariantsLeafs(son);
			CountVariants(root);
			out << "Удалить вершину "<< son->value << " (отец - " << node->value << ", останется " << root->varints_num << ") (Y / N) : ";
			SetVariantsLeafs(son);
			char response;
			cin >> response;
			switch (response)
			{
			case 'y':
			case 'Y':
				delete son;
				son = nullptr;
				it = node->sons.erase(it);
				break;
			case 'n':
			case 'N':
				++it;
				break;
			default:
				throw exception("Unknown response: " + response);
			}

			if (node->sons.empty())
			{
				prevNode->sons.erase(remove(prevNode->sons.begin(), prevNode->sons.end(), node), prevNode->sons.end());
				delete node;
				node = nullptr;
				deleted = true;
			}

			CountVariants(root);
			PrintTree(out, root);
			out << endl;
		}
	}

	if (!deleted)
	{
		for (auto& son : node->sons)
		{
			Dialog(out, root, son, node);
		}
	}
}