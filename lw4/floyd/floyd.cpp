//16. Реализовать алгоритм поиска кратчайших путей Флойда и
//проиллюстрировать по шагам этапы его выполнения(9).
#include <iostream>
#include <fstream>
#include <string>
#include <optional>

using namespace std;
#define INT_MAX 99999

// Number of vertices in the graph
#define V 4

struct Args
{
	string inputFileName;
};

void printSolution(int graph[][V])
{
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
}

void floyd(int graph[V][V], int (& dest)[V][V])
{
	int i, j, k;

	for (k = 0; k < V; k++)
	{
		for (i = 0; i < V; i++)
		{
			for (j = 0; j < V; j++)
			{
				if (graph[i][k] + graph[k][j] < graph[i][j])
				{
					dest[i][j] = dest[i][k];
					graph[i][j] = graph[i][k] + graph[k][j];
					
					printSolution(graph);
					printSolution(dest);
					cout << endl;
				}
			}
		}
	}

	printSolution(graph);
	printSolution(dest);
	cout << endl;
}

void reedGraphFromFile(string fileName, int(&graph)[V][V])
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
	}
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

int main(int argc, char* argv[])
{
	auto args = ParseArgs(argc, argv);

	if (!args)
	{
		cout << "Invalid input file name" << endl;
		return -1;
	}

	int graph[V][V];
	int dest[V][V];
	for (int v = 0; v < V; v++)
	{
		for (int u = 0; u < V; u++)
		{
			if (v == u)
			{
				graph[v][u] = 0;
				dest[v][u] = v;
			}
			else
			{
				graph[v][u] = INT_MAX;
			}
		}
	}

	

	reedGraphFromFile(args->inputFileName, graph);

	for (int v = 0; v < V; v++)
	{
		for (int u = 0; u < V; u++)
		{
			if (graph[v][u] == 0 || graph[v][u] != INT_MAX)
			{
				dest[v][u] = u + 1;
			}
			else
			{
				dest[v][u] = 0;
			}
		}
	}

	bool end = false;
	while (!end)
	{
		int command;

		cout << "Print graph - 1" << endl;
		cout << "Prim's algorithm - 2" << endl;
		cout << "End work - 3" << endl;
		cout << "Enter command: ";
		cin >> command;
		cout << endl;

		switch (command)
		{
		case 1:
			printSolution(graph);
			break;
		case 2:
			printSolution(graph);
			printSolution(dest);

			floyd(graph, dest);
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

	return 0;
}