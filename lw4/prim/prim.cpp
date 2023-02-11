//16. Реализовать алгоритм поиска кратчайших путей Флойда и
//проиллюстрировать по шагам этапы его выполнения(9).
#include <limits>
#include <iostream>
#include <fstream>
#include <string>
#include <optional>

using namespace std;

// Number of vertices in the graph
#define V 6

struct Args
{
	string inputFileName;
};

// A utility function to find the vertex with
// minimum key value, from the set of vertices
// not yet included in MST
int minKey(int key[], bool mstSet[])
{
	// Initialize min value
	int min = INT_MAX, min_index;

	for (int v = 0; v < V; v++)
		if (mstSet[v] == false && key[v] < min)
			min = key[v], min_index = v;

	return min_index;
}

void printTable(int parent[], int key[], bool mstSet[])
{
	int sum = 0;
	for (int i = 0; i < V; i++)
	{
		if (key[i] == INT_MAX)
		{
			cout << "INF";
		}
		else
		{
			cout << key[i] << "(" << parent[i] << ")";
		}
		cout << "\t";

		if (mstSet[i])
		{
			sum += key[i];
		}
	}
	cout << sum;
	cout << endl;
}

// Function to construct and print MST for
// a graph represented using adjacency
// matrix representation
void primMST(int graph[V][V])
{
	cout << "N" << "\t";
	for (int i = 0; i < V; i++)
	{
		cout << i << "\t";
	}
	cout << "Sum";
	cout << endl;

	int n = 1;
	// Array to store constructed MST
	int parent[V];

	// Key values used to pick minimum weight edge in cut
	int key[V];

	// To represent set of vertices included in MST
	bool mstSet[V];

	// Initialize all keys as INFINITE
	for (int i = 0; i < V; i++)
		key[i] = INT_MAX, mstSet[i] = false;

	// Always include first 1st vertex in MST.
	// Make key 0 so that this vertex is picked as first
	// vertex.
	key[0] = 0;
	parent[0] = -1; // First node is always root of MST

	// The MST will have V vertices
	for (int count = 0; count < V - 1; count++) {
		// Pick the minimum key vertex from the
		// set of vertices not yet included in MST
		int u = minKey(key, mstSet);

		// Add the picked vertex to the MST Set
		mstSet[u] = true;

		// Update key value and parent index of
		// the adjacent vertices of the picked vertex.
		// Consider only those vertices which are not
		// yet included in MST
		for (int v = 0; v < V; v++)
		{
			// graph[u][v] is non zero only for adjacent
			// vertices of m mstSet[v] is false for vertices
			// not yet included in MST Update the key only
			// if graph[u][v] is smaller than key[v]
			if (graph[u][v] && mstSet[v] == false
				&& graph[u][v] < key[v])
			{
				parent[v] = u;
				key[v] = graph[u][v];
				cout << n << "\t";
				printTable(parent, key, mstSet);
				n++;
			}
		}	
	}

	for (int i = 0; i < V; i++)
		mstSet[i] = true;
	// print the constructed MST
	cout << n << "\t";
	printTable(parent, key, mstSet);
}

void reedGraphFromFile(string fileName, int (&graph)[V][V])
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

void printGraph(int graph[V][V])
{
	for (int v = 0; v < V; v++)
	{
		for (int u = 0; u < V; u++)
		{
			cout << graph[v][u] << " ";
		}
		cout << endl;
	}
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
	for (int v = 0; v < V; v++)
	{
		for (int u = 0; u < V; u++)
		{
			graph[v][u] = 0;
		}
	}

	reedGraphFromFile(args->inputFileName, graph);
	// Print the solution
	primMST(graph);

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
			printGraph(graph);
			break;
		case 2:
			primMST(graph);
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