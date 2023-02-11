// 23. В строке текстового файла  задано  выражение  из  целых
//чисел и операций '+', '-', '*', '/', '^'.Порядок  вычислений
//определяется приоритетом операций и круглыми скобками.Возможен
//одноместный минус в начале  выражения  или  после  открывающей
//скобки.Преобразовать выражение в постфиксную  форму(алгоритм
//Дейкстры) и вычислить его значение.Показать этапы  выполнения
//(11).
// Худяков Даниил ПС-23
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
	std::string inputFileName;
	std::string outputFileName;
};

std::optional<Args> ParseArgs(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cout << "Invalid arguments count\n";
		std::cout << "Usage copyfile.exe <inputf file name> <output file name>\n";
		return std::nullopt;
	}
	Args args;
	args.inputFileName = argv[1];
	args.outputFileName = argv[2];
	return args;
}

void removeSpaces(string& s)
{
	s.erase(remove_if(s.begin(), s.end(), isspace), s.end());
}

bool isOperand(char ch)
{
	return ch >= '0' && ch <= '9';
}

bool isOperator(char op)
{
	return op == '+' || op == '-' || op == '*' || op == '/' || op == '^' || op == '(' || op == ')' || op == '~';
}

bool isUnaryMinus(string const& infixForm, char op, unsigned index)
{
	return op == '-' && (index == 0 || (index > 1 && (isOperator(infixForm[index - 1]) || (infixForm[index - 1] == '('))));
}

bool isLeftAssociationOperator(char op)
{
	switch (op)
	{
	case '+':
	case '-':
	case '~':
	case '*':
	case '/':
		return true;
	case '^':
		return false;
	default:
		throw exception("Unknown operator");
	}
}

int getPriority(char op)
{
	switch (op)
	{
	case '(':
		return 0;
	case '+':
	case '-':
		return 1;
	case '*':
	case '/':
		return 2;
	case '^':
		return 4;
	case '~':
		return 3;
	default:
		throw exception("Unknown operator " + op);
	}
}

string infixToPostfix(const string& input)
{
	Stack<char>* st = nullptr;
	string result = "";
	unsigned i = 0;

	for (auto ch : input)
	{
		if (isOperand(ch))
		{
			if (i > 0 && !isdigit(input[i - 1]))
			{
				result += ' ';
			}
			result += ch;
		}
		else if (ch == '(')
		{
			StackUtils::Push(&st, ch);
		}
		else if (ch == ')')
		{
			while (StackUtils::Peek(st) != '(')
			{
				result += ' ';
				result += StackUtils::Pop(&st);
			}
			StackUtils::Pop(&st);
		}
		else
		{
			if (isUnaryMinus(input, ch, i))
			{
				ch = '~';
			}

			if (isLeftAssociationOperator(ch))
			{
				while (!StackUtils::IsEmpty(st) && getPriority(ch) <= getPriority(StackUtils::Peek(st)))
				{
					result += ' ';
					result += StackUtils::Pop(&st);
				}
			}
			else
			{
				while (!StackUtils::IsEmpty(st) && getPriority(ch) < getPriority(StackUtils::Peek(st)))
				{
					result += ' ';
					result += StackUtils::Pop(&st);
				}
			}
			

			StackUtils::Push(&st, ch);
		}

		cout << result << endl;
		i++;
	}

	while (!StackUtils::IsEmpty(st))
	{
		result += ' ';
		result += StackUtils::Pop(&st);
		cout << result << endl;
	}

	return result;
}

double calcOperation(double a, double b, char op)
{
	switch (op)
	{
	case '+':
		return b + a;
	case '-':
		return b - a;
	case '*':
		return b * a;
	case '/':
		return b / a;
	case '^':
		return pow(b, a);
	default:
		throw exception("Unknown operator" + op);
	}
}

double charToDouble(char ch)
{
	switch (ch)
	{
		case '1':
			return 1;
		case '2':
			return 2;
		case '3':
			return 3;
		case '4':
			return 4;
		case '5':
			return 5;
		case '6':
			return 6;
		case '7':
			return 7;
		case '8':
			return 8;
		case '9':
			return 9;
		default:
			throw exception("Unknown char" + ch);
	}
}

double postfixEval(string postfix)
{
	double a, b;
	Stack<double>* st = nullptr;
	unsigned i = 0;
	unsigned l = postfix.length();

	while (i < l)
	{
		char ch = postfix[i];

		if (isOperator(ch))
		{
			if (ch == '~')
			{
				a = StackUtils::Pop(&st);
				a *= -1;
				StackUtils::Push(&st, a);
			}
			else
			{
				a = StackUtils::Pop(&st);
				b = StackUtils::Pop(&st);

				StackUtils::Push(&st, calcOperation(a, b, ch));
			}
		}
		else if (isOperand(ch))
		{
			StackUtils::Push(&st, charToDouble(ch));
		}
		else
		{
			throw exception("Unknown symbol in postfix string");
		}

		i++;
	}
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

	string str;
	getline(input, str);
	try
	{
		removeSpaces(str);
		string result = infixToPostfix(str);
		cout << "Result: " << result << endl;
		removeSpaces(result);
		cout << "Evalutation: " << postfixEval(result) << endl;
	}
	catch (exception const& e)
	{
		cout << e.what() << endl;
	}
	

	if (input.bad())	
	{
		std::cout << "Failed to read data form input file\n";
		return 1;
	}

	return 0;

}