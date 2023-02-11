//15. Трассировка  программы, не   содержащей   рекурсивных
//вызовов  и  повторяющихся  имен  процедур, распечатана в виде
//списка выполняемых процедур.Процедура попадает в список, если
//к ней произошло обращение из вызывающей процедуры либо возврат
//управления из  вызванной  ей  процедуры.Структура  программы
//такова, что  каждая вызываемая процедура вложена в вызывающую
//ее процедуру.Известен объем памяти, который  требуется  для
//загрузки каждой процедуры.При выходе из процедуры занимаемая
//ей   память   освобождается.Выяснить, соответствует    ли
//трассировка  правильной  работе  программы.Определить размер
//памяти, необходимый для работы программы, и цепочку вызовов,
//требующую максимальной памяти(8).
// <фамилия имя> ПС-21
// Visual Studio 2022 C++ 20
#include <iostream>
#include <optional>
#include <fstream>
#include <string>

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
	bool IsOnlyOneInStack(Stack<T>* stack)
	{
		return stack->next == nullptr;
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

struct Args
{
	string trassFileName;
	string volFileName;
};

std::optional<Args> ParseArgs(int argc, char* argv[])
{
	if (argc != 3)
	{
		cout << "Invalid arguments count\n";
		cout << "Usage program-trace.exe <inputf file name>\n";
		return nullopt;
	}
	Args args;
	args.trassFileName = argv[1];
	args.volFileName = argv[2];
	return args;
}

struct Procedure
{
	string name;
	int memory;
	optional<string> parent;
};

Procedure GetProcFromVolStack(Stack<Procedure>* volStack, const string& name)
{
	Stack<Procedure>* next = volStack;

	while (!StackUtils::IsEmpty(next))
	{
		Procedure currProc = StackUtils::Peek(next);
		if (name == currProc.name)
		{
			return currProc;
		}

		next = next->next;
	}

	throw exception("can't find memory in vol stack");
}

template <typename T>
bool IsProcedureInStack(Stack<T>* stack, const T& data)
{
	Stack<T>* next = stack;

	while (!StackUtils::IsEmpty(next))
	{
		if (data == StackUtils::Peek(next))
		{
			return true;
		}

		next = next->next;
	}

	return false;
}

void ChangeProcedureParent(Stack<Procedure>* memoryStack, const string& name, const string& parent)
{
	Stack<Procedure>* next = memoryStack;

	while (!StackUtils::IsEmpty(next))
	{
		Procedure currProc = StackUtils::Peek(next);
		if (name == currProc.name)
		{
			next->key.parent = parent;
			return;
		}

		next = next->next;
	}

	throw exception("can't find memory in memory stack");
}

int main(int argc, char* argv[])
{
	auto args = ParseArgs(argc, argv);
	// Проверка правильности аргументов командной строки
	if (!args)
	{
		return 1;
	}

	// Открываем файл величин
	std::ifstream inputVol;
	inputVol.open(args->volFileName);
	if (!inputVol.is_open())
	{
		std::cout << "Failed to open '" << args->volFileName << "' for reading\n";
		return 1;
	}

	Stack<Procedure>* volStack = nullptr; // инициализация стека величин

	try
	{
		string name;
		int memory;

		// чтение файла построчно
		while (inputVol >> name >> memory)
		{
			Procedure proc;
			proc.memory = memory;
			proc.name = name;

			StackUtils::Push(&volStack, proc); // добавление в стек данных о памяти занимаемой процедурой
		}
	}
	catch (exception const& e)
	{
		cout << e.what() << endl; // печать ошибки при нахождении
	}

	if (inputVol.bad())
	{
		std::cout << "Failed to read data form vol input file\n";
		return 1;
	}

	// Открываем трассировочный файл
	std::ifstream inputTrass;
	inputTrass.open(args->trassFileName);
	if (!inputTrass.is_open())
	{
		std::cout << "Failed to open '" << args->trassFileName << "' for reading\n";
		return 1;
	}

	try
	{
		Stack<string>* trassStack = nullptr; // инициализация стека трассировки

		string name;

		int maxMemory = 0;
		int currMemory = 0;
		string currMemoryProcChain = "";
		string maxMemoryProcChain = "";
		int maxMemoryPos = 0;
		int pos = 0;

		// чтение файла построчно
		while (inputTrass >> name)
		{
			pos++;
			// смотрим если ли в стеке вызовов процедура
			// чтобы понять она новая или закрывающая
			if (IsProcedureInStack(trassStack, name))
			{
				// смотрим есть ли в процедуре подпроцедуры
				string currTrassProcName = StackUtils::Peek(trassStack);

				// повторяем вынесение из стека трассеровки пока имя зыкрывающее не совпадет
				// с закрывающим
				while (currTrassProcName != name)
				{
					// удаление имени текущей процедуры из стека трассеровки
					StackUtils::Pop(&trassStack);
					// получение данных о процедуре
					Procedure currVolProc = GetProcFromVolStack(volStack, currTrassProcName);
					// если неправильный родитель, то ошибка
					if (currVolProc.parent && currVolProc.parent != name)
					{
						string mesg = "<" + currTrassProcName + "> must have parent <" + *currVolProc.parent + ">";
						throw exception(mesg.c_str());
					}
					// удаление из текущего состояния цепочки вызовов памяти соответствуещей текущей процедуре
					currMemory -= currVolProc.memory;
					currMemoryProcChain.erase(currMemoryProcChain.end() - currTrassProcName.length() - 1, currMemoryProcChain.end());
					// задание родителя процедуры
					ChangeProcedureParent(volStack, currTrassProcName, name);
					// получение имени текущей процедуры из стека трассеровки
					currTrassProcName = StackUtils::Peek(trassStack);
				}
			}
			else
			{
				//добавление новой процедуры в стек вызовов
				StackUtils::Push(&trassStack, name);
				// прибавление в текущую строку вызовов новой процедуры
				Procedure currVolProc = GetProcFromVolStack(volStack, name);
				currMemory += currVolProc.memory;
				currMemoryProcChain += name;
				currMemoryProcChain += " ";
			}

			// смотрим если текущая цепочка занимает больше памяти
			// чем максимальная то она становится максимальной 
			if (maxMemory < currMemory)
			{
				maxMemory = currMemory;
				maxMemoryProcChain = currMemoryProcChain;
				maxMemoryPos = pos;
			}
		}

		// печать 
		cout << "Max memory: " << maxMemory << endl;
		// разворот всей итоговой строки для красивого вывода
		cout << "Max memory procedure chain: " << maxMemoryProcChain << endl;
	}
	catch (exception const& e)
	{
		cout << e.what() << endl; // печать ошибки при нахождении
	}


	if (inputTrass.bad())
	{
		std::cout << "Failed to read data form trass input file\n";
		return 1;
	}

	return 0;
}