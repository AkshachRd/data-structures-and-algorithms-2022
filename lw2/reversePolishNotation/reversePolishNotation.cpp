#include <iostream>
#include <string>
#include <iterator>
#include <vector>
#include <stack>
#include <fstream>
using namespace std;

bool tryParse(const string& symbol);
int priority(const string& c);
bool isOperator(const string& c);

bool tryParse(const string& symbol)
{
    bool isNumber = false;
    for (unsigned int i = 0; i < symbol.size(); i++)
    {
        if (isdigit(symbol[i]))
        {
            isNumber = true;
        }
    }
    return isNumber;
}

int priority(const string& c)
{
    if (c == "^")
    {
        return 3;
    }
    if (c == "*" || c == "/")
    {
        return 2;
    }
    if (c == "+" || c == "-")
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

bool isOperator(const string& c)
{
    return (c == "+" || c == "-" || c == "*" || c == "/" || c == "^");
}

int main(int argc, char* argv[])
{
    ifstream input(argv[1]);
    vector<string> tokens;//store the tokens here
    while (input.is_open())
    {
        string temp;
        input >> temp;
        tokens.push_back(temp);
    }
    vector<string> outputList; //output vector
    stack<string> s; //main stack

    for (unsigned int i = 0; i < tokens.size(); i++)
    {
        if (tryParse(tokens[i]))
        {
            outputList.push_back(tokens[i]);
        }
        if (tokens[i] == "(")
        {
            s.push(tokens[i]);
            if (tokens[i] == ")")
            {
                while (!s.empty() && s.top() != "(")
                {
                    outputList.push_back(s.top());

                }
            }
        }
    }
}