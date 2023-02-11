#include <fstream>
#include <iostream>
#include <optional>
#include <limits>

//18. Реализовать алгоритм поиска кратчайших путей Дейкстры и
//проиллюстрировать по шагам этапы его выполнения(10).
//Артемий Арсибеков, Visual Studio 2022 C++20

using namespace std;

// Число вершин в матрице
#define V 4

struct Args
{
	string inputFileName;
};

optional<Args> ParseArgs(int argc, char* argv[]);
void ReadGraphFromFile(string fileName, int(&graph)[V][V]);
void Dijkstra(int graph[V][V], int src);

int main(int argc, char* argv[]) {
	auto args = ParseArgs(argc, argv);

	if (!args)
	{
		cout << "filename not found" << endl;
		return -1;
	}

	int graph[V][V] = {0};
	ReadGraphFromFile(args->inputFileName, graph);

	bool endLoop = false;
	while (!endLoop)
	{
		int cmnd;

		cout << "Print graph - 1" << endl;
		cout << "Dijkstra's algorithm - 2" << endl;
		cout << "End work - 3" << endl;
		cout << "Enter command: ";
		cin >> cmnd;
		cout << endl;

		switch (cmnd)
		{
		case 1:
			for (int i = 0; i < V; i++) {
				for (int j = 0; j < V; j++) {
					if (graph[i][j] == INT_MAX)
						printf("%7s", "INF");
					else
						printf("%7d", graph[i][j]);
				}
				printf("\n");
			}
			printf("\n");
			break;
		case 2:
			Dijkstra(graph, 0);
			break;
		case 3:
			endLoop = true;
			break;
		default:
			cout << "unknown command: " << cmnd << endl;
			break;
		}
		cout << endl;
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

void ReadGraphFromFile(string fileName, int(&graph)[V][V])
{
	ifstream inputFile(fileName);

	if (!inputFile.is_open())
	{
		throw exception("cannot open input file");
	}

	int src, dest, weight;

	while (inputFile >> src >> dest >> weight)
	{
		graph[src - 1][dest - 1] = weight;
		graph[dest - 1][src - 1] = weight;
	}
}

int MinDistance(int dist[], bool sptSet[])
{
	int min = INT_MAX, min_index;

	for (int v = 0; v < V; v++)
		if (sptSet[v] == false && dist[v] <= min)
			min = dist[v], min_index = v;

	return min_index;
}

optional<Args> ParseArgs(int argc, char* argv[]);

void PrintSolution(int n, int dist[], int path[])
{
	cout << n << "\t";
	for (int i = 0; i < V; i++)
	{
		if (dist[i] == INT_MAX)
		{
			cout << "INF ";
		}
		else
		{
			cout << dist[i];
		}

		if (path[i] != INT_MAX)
		{
			cout << "(" << path[i] + 1 << ")" << "\t";
		}
		else
		{
			cout << "\t";
		}

	}
	cout << endl;
}

void Dijkstra(int graph[V][V], int src)
{
	int dist[V];
	int path[V];
	bool sptSet[V];
	int n = 1;

	for (int i = 0; i < V; i++)
		dist[i] = INT_MAX, sptSet[i] = false, path[i] = INT_MAX;

	dist[src] = 0;
	path[src] = 0;

	cout << "N\t";
	for (int i = 0; i < V; i++)
	{
		cout << i + 1 << "\t";
	}
	cout << endl << endl;
	PrintSolution(n, dist, path);
	n++;

	for (int count = 0; count < V - 1; count++) {
		int u = MinDistance(dist, sptSet);

		sptSet[u] = true;

		for (int v = 0; v < V; v++)
		{
			if (!sptSet[v] && graph[u][v]
				&& dist[u] != INT_MAX
				&& dist[u] + graph[u][v] < dist[v])
			{
				dist[v] = dist[u] + graph[u][v];
				path[v] = u;
			}

			if (!sptSet[v])
			{
				PrintSolution(n, dist, path);
				n++;
			}
		}
	}

	PrintSolution(n, dist, path);
	n++;
}