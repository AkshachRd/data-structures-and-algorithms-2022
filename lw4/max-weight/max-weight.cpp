//19. Имеется  сеть  автомобильных  дорог.Для каждой дороги
//известна максимальная масса груза, которую можно  провезти  по
//этой   дороге.С   помощью  алгоритма  Дейкстры   определить
//максимальный   груз, который  можно  провезти  между   двумя
//указанными городам(10).
//
//Худяков Даниил, Visual Studio 2022, C++20
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <limits>

using namespace std;

class Matrix
{
public:
	Matrix() : m_data({}) {}

	Matrix(const int& rows, const int& cols)
	{
		Reset(rows, cols);
	}

	void Reset(const int& rows, const int& cols)
	{
		m_data.resize(rows);
		for (int i = 0; i < rows; ++i)
		{
			m_data.at(i).resize(cols);
		}
	}

	int At(const int& row, const int& col) const
	{
		return m_data.at(row).at(col);
	}

	int& At(const int& row, const int& col)
	{
		if (row >= GetNumRows())
		{
			Reset(row, row);
		}

		if (col >= GetNumColumns())
		{
			Reset(col, col);
		}
		return m_data.at(row - 1).at(col - 1);
	}

	int GetNumRows() const
	{
		return m_data.size();
	}

	int GetNumColumns() const
	{
		if (GetNumRows() > 0)
		{
			return m_data[0].size();
		}

		return 0;
	}

	vector<vector<int>> const& Data() const
	{
		return m_data;
	}
private:
	vector<vector<int>> m_data;
};

struct Args
{
	string inputFileName;
};

optional<Args> ParseArgs(int argc, char* argv[]);
optional<Matrix> ReadGraphFromFile(string const& filename);
string getMaxWeight(Matrix& graph, int src, int dest);

int main(int argc, char* argv[]) {
	auto args = ParseArgs(argc, argv);

	if (!args)
	{
		cout << "filename not found" << endl;
		return -1;
	}

	auto graph = ReadGraphFromFile(args->inputFileName);

	if (!graph)
	{
		cout << args->inputFileName << " invalid filename" << endl;
		return -1;
	}

	int source, destination;

	cout << "Enter source and destinition (<src> <dst>): ";
	cin >> source;
	cin >> destination;

	bool sourceOutOfRange = source - 1 >= graph->GetNumRows();
	bool destinationOutOfRange = destination - 1 >= graph->GetNumColumns();

	if (sourceOutOfRange || destinationOutOfRange)
	{
		cout << "invalid source or destination" << endl;
		return -1;
	}

	for (auto line : graph->Data())
	{
		for (auto vertex : line)
		{
			cout << vertex << " ";
		}
		cout << endl;
	}

	cout << "Max weight: " << getMaxWeight(*graph, source, destination);

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

optional<Matrix> ReadGraphFromFile(string const& filename)
{
	ifstream input(filename);

	if (!input.is_open())
	{
		return nullopt;
	}

	Matrix graph;

	while (!input.eof())
	{
		int v1, v2, weight;
		input >> v1;
		input >> v2;
		input >> weight;

		bool incorrectVertexIndex = v1 < 1 || v2 < 1;
		bool negativeWeight = weight < 0;

		if (incorrectVertexIndex || negativeWeight)
		{
			continue;
		}

		graph.At(v1, v2) = weight;
		graph.At(v2, v1) = weight;
	}

	return graph;
}

int maxWeight(vector<int> weight, vector<bool> sptSet)
{
	int max = -1;
	int max_index;

	for (size_t i = 0; i < weight.size(); i++)
	{
		if (sptSet[i] == false && weight[i] >= max)
		{
			max = weight[i];
			max_index = i;
		}
	}

	return max_index;
}

string getMaxWeight(Matrix& graph, int src, int dest)
{
	size_t size = graph.GetNumRows();
	constexpr int MAX_INT = numeric_limits<int>::max();

	vector<int> weight(size, -1);
	vector<int> path(size, 0);
	vector<bool> sptSet(size, false);

	weight[src - 1] = MAX_INT;

	int max_index;

	for (int count = 0; count < size - 1; count++)
	{
		max_index = maxWeight(weight, sptSet);
		sptSet[max_index] = true;

		for (int j = 0; j < size; j++)
		{
			int path_weight = graph.At(max_index + 1, j + 1);
			int min_path_weight = min(weight[max_index], path_weight);

			if (!sptSet[j] && path_weight && weight[max_index] != -1
				&& min_path_weight > weight[j])
			{
				weight[j] = min_path_weight;
				path[j] = max_index + 1;
			}
		}
	}

	if (weight[dest - 1] != -1)
	{
		string s = to_string(dest) + " - ";
		int v = path[dest - 1];
		while (v != src)
		{
			s = s + to_string(v) + " - ";
			v = path[v - 1];
		}
		s += to_string(v);
		reverse(s.begin(), s.end());
		return to_string(weight[dest - 1]) + "\n" + "Path: " + s;
	}

	return "No way (-1)";
}