#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Passanger
{
	string name;
	int service_time;
};

template <typename T>
struct Node
{
	Node(T data)
		: key(data), next(nullptr)
	{}

	Node<T>* next;
	T key;
};

template <typename T>
struct Queue
{
	Queue()
		: head(nullptr), tail(nullptr)
	{}

	Node<T>* head;
	Node<T>* tail;
};

namespace QueueUtils
{
	template <typename T>
	bool IsEmpty(Queue<T>* queue)
	{
		return queue->head == nullptr && queue->tail == nullptr;
	}

	template <typename T>
	void Push(Queue<T>* root, T data)
	{
		Node<T>* queueNode = new Node<T>(data);
		if (root->tail != nullptr)
		{
			root->tail->next = queueNode;
		}
		else
		{
			root->head = queueNode;
		}

		root->tail = queueNode;
	}

	template <typename T>
	void PushToHead(Queue<T>* root, T data)
	{
		Node<T>* queueNode = new Node<T>(data);
		queueNode->next = root->head->next;
		root->head = queueNode;
		if (root->tail == nullptr)
		{
			root->tail = queueNode;
		}
	}

	template <typename T>
	T Pop(Queue<T>* root)
	{
		if (IsEmpty(root))
			throw exception("failed to pop element from empty queue");
		Node<T>* temp = root->head;
		root->head = temp->next;
		T popped = temp->key;

		if (root->head == nullptr)
		{
			root->tail = nullptr;
		}

		delete temp;
		return popped;
	}

	template <typename T>
	T PeekHead(Queue<T>* root)
	{
		if (IsEmpty(root))
			throw exception("failed to peek element from empty queue");
		return root->head->key;
	}

	template <typename T>
	T PeekTail(Queue<T>* root)
	{
		if (IsEmpty(root))
			throw exception("failed to peek element from empty queue");
		return root->tail->key;
	}
}

void ReadQueues(ifstream& input, Queue<Passanger>* firstQueue, Queue<Passanger>* secondQueue)
{
	short queue_num;
	string name;
	int service_time;
	while (input >> queue_num >> name >> service_time)
	{
		Passanger passanger;
		passanger.name = name;
		passanger.service_time = service_time;

		switch (queue_num)
		{
		case 1:
			QueueUtils::Push(firstQueue, passanger);
			break;
		case 2:
			QueueUtils::Push(secondQueue, passanger);
			break;
		default:
			throw exception("unknown queue");
		}
	}
}

void ProcessCheckouts(Queue<Passanger>* firstQueue, Queue<Passanger>* secondQueue)
{
	Passanger firstPassanger;
	Passanger secondPassanger;
	int firstTimer = 0;
	int secondTimer = 0;
	int time = 0;

	firstPassanger = QueueUtils::PeekHead(firstQueue);
	secondPassanger = QueueUtils::PeekHead(secondQueue);

	while (!QueueUtils::IsEmpty(firstQueue) || !QueueUtils::IsEmpty(secondQueue))
	{
		if (firstPassanger.name == secondPassanger.name)
		{
			QueueUtils::Pop(secondQueue);
			if (!QueueUtils::IsEmpty(secondQueue))
			{
				secondPassanger = QueueUtils::PeekHead(secondQueue);
			}
		}

		while (!QueueUtils::IsEmpty(firstQueue) && (firstPassanger.service_time <= (time - firstTimer)))
		{
			firstTimer += firstPassanger.service_time;
			QueueUtils::Pop(firstQueue);
			cout << 1 << "\t" << time << "\t" << firstPassanger.name << "\t" << firstPassanger.service_time << endl;
			if (!QueueUtils::IsEmpty(firstQueue))
			{
				firstPassanger = QueueUtils::PeekHead(firstQueue);
			}
		}

		while (!QueueUtils::IsEmpty(secondQueue) && (secondPassanger.service_time <= (time - secondTimer)))
		{
			secondTimer += secondPassanger.service_time;
			QueueUtils::Pop(secondQueue);
			cout << 2 << "\t" << time << "\t" << secondPassanger.name << "\t" << secondPassanger.service_time << endl;
			if (!QueueUtils::IsEmpty(secondQueue))
			{
				secondPassanger = QueueUtils::PeekHead(secondQueue);
			}
		}

		time++;
	}
}

void PrintQueue(Queue<Passanger>* queue)
{
	Node<Passanger>* nextPtrQueue = queue->head;
	while (nextPtrQueue != nullptr)
	{
		cout << nextPtrQueue->key.name << "\t" << nextPtrQueue->key.service_time << endl;
		nextPtrQueue = nextPtrQueue->next;
	}
	cout << endl;
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		cout << "Invalid arguments count\n";
		cout << "Usage queue-of-stacks.exe <inputf file name>\n";
		return 1;
	}

	string inputFileName = argv[1];

	std::ifstream input;
	input.open(inputFileName);
	if (!input.is_open())
	{
		std::cout << "Failed to open '" << inputFileName << "' for reading\n";
		return 1;
	}

	try
	{
		auto firstQueue = new Queue<Passanger>();
		auto secondQueue = new Queue<Passanger>();

		ReadQueues(input, firstQueue, secondQueue);

		bool end = false;

		while (!end)
		{
			int command;

			cout << "Enter command:" << endl;
			cout << "Print first queue - 1" << endl;
			cout << "Print second queue - 2" << endl;
			cout << "Checkout - 3" << endl;
			cout << "End work - 4" << endl;
			cin >> command;
			cout << endl;


			switch (command)
			{
			case 1:
				PrintQueue(firstQueue);
				break;
			case 2:
				PrintQueue(secondQueue);
				break;
			case 3:
				ProcessCheckouts(firstQueue, secondQueue);
				cout << "Successfuly" << endl;
				break;
			case 4:
				end = true;
				break;
			default:
				cout << "Unknown command" << endl;
				break;
			}
		}
	}
	catch (exception const& e)
	{
		cout << e.what() << endl; // печать ошибки при нахождении
	}

	if (input.bad())
	{
		cout << "Failed to read data form input file\n";
		return 1;
	}

	return 0;
}