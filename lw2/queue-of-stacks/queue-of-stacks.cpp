//19. Организовать  в основной  памяти  с помощью указателей
//очередь  из  стеков.Обеспечить  операции  ведения  стека  из
//начала  очереди, дополнения  и  продвижения  очереди, выдачи
//содержимого очереди(9).
// <фамилия имя> ПС-21
// Visual Studio 2022, C++ 20
#include <iostream>
#include <optional>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

template <typename T>
struct Stack
{
	Stack(T element)
		: key(element), next(nullptr)
	{}

	T key;
	Stack<T>* next;
};

namespace StackUtils
{
	template <typename T>
	bool IsEmpty(Stack<T>* stack)
	{
		return stack == nullptr;
	}

	template <typename T>
	void Push(Stack<T>** root, T data)
	{
		Stack<T>* stackNode = new Stack<T>(data);
		stackNode->next = *root;
		*root = stackNode;
	}

	template <typename T>
	T Pop(Stack<T>** root)
	{
		if (IsEmpty(*root))
			throw exception("failed to pop element from empty stack");
		Stack<T>* temp = *root;
		*root = (*root)->next;
		T popped = temp->key;

		delete temp;
		return popped;
	}

	template <typename T>
	T Peek(Stack<T>* root)
	{
		if (IsEmpty(root))
			throw exception("failed to peek element from empty stack");
		return root->key;
	}
}

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

struct Args
{
	std::string inputFileName;
};

std::optional<Args> ParseArgs(int argc, char* argv[])
{
	if (argc != 2)
	{
		cout << "Invalid arguments count\n";
		cout << "Usage queue-of-stacks.exe <inputf file name>\n";
		return nullopt;
	}
	Args args;
	args.inputFileName = argv[1];
	return args;
}

template <typename T>
void PrintStack(Stack<T>* stack)
{
	Stack<T>* nextPtr = stack;
	while (!StackUtils::IsEmpty(nextPtr))
	{
		cout << StackUtils::Peek(nextPtr) << " ";
		nextPtr = nextPtr->next;
	}
	cout << endl;
}


template <typename T>
void PrintQueueOfStacks(Queue<T>* queue)
{
	cout << "Queue head: ";
	T nextPtr = QueueUtils::PeekHead(queue); // смотрим в начало очереди
	while (!StackUtils::IsEmpty(nextPtr))
	{
		cout << StackUtils::Peek(nextPtr) << " "; 
		nextPtr = nextPtr->next; // следующий элемент стека
	}
	cout << endl;

	cout << "Queue tail: ";
	nextPtr = QueueUtils::PeekTail(queue);
	while (!StackUtils::IsEmpty(nextPtr))
	{
		cout << StackUtils::Peek(nextPtr) << " ";
		nextPtr = nextPtr->next;
	}
	cout << endl;

	cout << "Queue:" << endl;
	Node<T>* nextPtrQueue = queue->head;
	while (nextPtrQueue != nullptr)
	{
		PrintStack(nextPtrQueue->key);
		nextPtrQueue = nextPtrQueue->next;
	}
	cout << endl;
}


template <typename T>
Stack<T>* AddStack(Stack<T>* stack  = nullptr)
{
	Stack<T>* st = stack;
	bool end = false;

	while (!end)
	{
		int command;

		cout << "Add to the stack - 1" << endl;
		cout << "Delete from the stack - 2" << endl;
		cout << "Print the stack - 3" << endl;
		cout << "Back to main menu - 4" << endl;
		cout << "Enter command:" << endl;
		cin >> command;

		switch (command)
		{
		case 1:
			cout << "Enter data: ";
			T data;
			cin >> data;
			StackUtils::Push(&st, data);
			cout << "Successfuly" << endl;
			break;
		case 2:
			StackUtils::Pop(&st);
			cout << "Successfuly" << endl;
			break;
		case 3:
			PrintStack(st);
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

	return st;
}

int main(int argc, char* argv[])
{
	auto args = ParseArgs(argc, argv);
	// Проверка правильности аргументов командной строки
	if (!args)
	{
		return 1;
	}

	// Открываем входной файл
	std::ifstream input;
	input.open(args->inputFileName);
	if (!input.is_open())
	{
		std::cout << "Failed to open '" << args->inputFileName << "' for reading\n";
		return 1;
	}

	try
	{
		Queue<Stack<int>*>* queue = new Queue<Stack<int>*>(); // инициализация очереди стеков

		// чтение файла построчно
		for (string line; getline(input, line); )
		{
			Stack<int>* stack = nullptr; // инициализация стека чисел

			istringstream iss(line); // преобразования строки в поток для удобного считывания чисел

			int data;
			// чтение чисел из строки
			while (iss >> data)
			{
				StackUtils::Push(&stack, data); // добавление числа в стек
			}

			QueueUtils::Push(queue, stack); // добавление стека в очередь стеков
		}

		// печать 
		PrintQueueOfStacks(queue);

		bool end = false;

		while (!end)
		{
			int command;

			cout << "Enter command:" << endl;
			cout << "Add to the queue of stacks - 1" << endl;
			cout << "Push the queue of stacks forward - 2" << endl;
			cout << "Edit stack in the beginning of the queue - 3" << endl;
			cout << "Print the queue of stacks - 4" << endl;
			cout << "End work - 5" << endl;
			cin >> command;

			Stack<int>* addStack = nullptr;

			switch (command)
			{
			case 1:
				addStack = AddStack<int>();
				QueueUtils::Push(queue, addStack);
				cout << "Successfuly" << endl;
				break;
			case 2:
				QueueUtils::Pop(queue);
				cout << "Successfuly" << endl;
				break;
			case 3: 
				addStack = AddStack<int>(QueueUtils::PeekHead(queue));
				QueueUtils::Pop(queue);
				QueueUtils::PushToHead(queue, addStack);
				cout << "Successfuly" << endl;
				break;
			case 4:
				PrintQueueOfStacks(queue);
				cout << "Successfuly" << endl;
				break;
			case 5:
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
		std::cout << "Failed to read data form input file\n";
		return 1;
	}

	return 0;

}