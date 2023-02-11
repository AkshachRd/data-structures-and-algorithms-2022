#include <iostream>
#include <vector>
#include <optional>
#include <fstream>
#include <string>
#include <windows.h>
#include <limits>

using namespace std;

constexpr char LEVEL_CHAR = '.';

template <typename T>
struct Tree
{
	T value;
	int mass;
	int level;
	vector<Tree<T>*> sons;
};

void PrintTree(ostream& out, Tree<string>* root);
Tree<string>* ReadTreeFromFile(ifstream& file, Tree<string>*& root, int currentLevel = 0, Tree<string>** prevNode = nullptr);
void PrintConstructionMaxMass(ostream& out, int massLimit, Tree<string>* root);

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		cout << "Invalid input file name" << endl;
		return -1;
	}

	string inputFileName = argv[1];

	ifstream inputFile(inputFileName);
	if (!inputFile.is_open())
	{
		throw exception("cannot open input file");
	}

	try
	{
		int massLimit;
		inputFile >> massLimit;

		Tree<string>* root = nullptr;
		if (ReadTreeFromFile(inputFile, root))
		{
			throw exception("something went wrong");
		}

		bool end = false;
		while (!end)
		{
			int command;

			cout << "Print tree - 1" << endl;
			cout << "Print construnction max mass - 2" << endl;
			cout << "End work - 3" << endl;
			cout << "Enter command: ";
			cin >> command;
			cout << endl;

			switch (command)
			{
			case 1:
				PrintTree(cout, root);
				break;
			case 2:
				PrintConstructionMaxMass(cout, massLimit, root);
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

int ReadLevelFromString(string const& str)
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

string ReadValueFromString(string const& str)
{
	string value;

	for (auto i = str.find_first_not_of(LEVEL_CHAR); i < str.length(); i++)
	{
		value += str[i];
	}

	return value;
}

Tree<string>* ReadTreeFromFile(ifstream& file, Tree<string>*& root, int currentLevel, Tree<string>** prevNode)
{
	Tree<string>* currTreeNode = nullptr;
	Tree<string>* prevTreeNode = prevNode ? *prevNode : nullptr;

	string line;
	while (getline(file, line))
	{
		if (line.empty())
		{
			continue;
		}
		currTreeNode = new Tree<string>();
		currTreeNode->level = ReadLevelFromString(line);
		string val = ReadValueFromString(line);
		size_t pos = val.find(' ');
		currTreeNode->value = val.substr(0, pos);
		currTreeNode->mass = stoi(val.substr(pos + 1, val.size() - pos));

		if (currTreeNode->level == 0)
		{
			root = currTreeNode;
			prevTreeNode = root;
			continue;
		}

		if ((currTreeNode->level - currentLevel) == 1)
		{
			root->sons.push_back(currTreeNode);
		}

		if ((currTreeNode->level - currentLevel) > 1)
		{
			prevTreeNode->sons.push_back(currTreeNode);
			Tree<string>* notChildNode = ReadTreeFromFile(file, prevTreeNode, currentLevel + 1, &currTreeNode);

			if (notChildNode)
			{
				if ((notChildNode->level - currentLevel) < 1)
				{
					return notChildNode;
				}
				else
				{
					root->sons.push_back(notChildNode);
					prevTreeNode = notChildNode;
					continue;
				}
			}
		}

		if ((currTreeNode->level - currentLevel) < 1)
		{
			return currTreeNode;
		}
		prevTreeNode = currTreeNode;
	}

	return nullptr;
}

void PrintTree(ostream& out, Tree<string>* root)
{
	out << string(root->level, '.') << root->value << ' ' << root->mass << endl;

	for (auto& son : root->sons)
	{
		PrintTree(out, son);
	}
}

int FindConstructionMaxMass(Tree<string>* root)
{
	int maxMass = root->mass;

	for (auto son : root->sons)
	{
		maxMass = max(FindConstructionMaxMass(son), maxMass);
	}

	root->mass = max(root->mass, maxMass);
	return maxMass;
}

void PrintConstructionMaxMass(ostream& out, int massLimit, Tree<string>* root)
{
	FindConstructionMaxMass(root);
	if (root->mass <= massLimit)
	{
		cout << "Construction mass <= mass limit: " << root->mass << " <= " << massLimit;
	}
	else
	{
		cout << "Construction mass > mass limit: " << root->mass << " > " << massLimit;
	}
	cout << endl;
	PrintTree(out, root);
}