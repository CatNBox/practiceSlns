#include <iostream>
#include <string>
#include <stack>

/*
using namespace std;

string getPostfixExp(string infixExp);

int main()
{
	string infixExp;
	string postfixExp;

	cin >> infixExp;

	cout << "입력된 식 : " << endl << infixExp << endl;

	postfixExp = getPostfixExp(infixExp);
	
	cout << "변환된 식 : " << endl << postfixExp << endl;

	cin >> infixExp;
	return 0;
}

string getPostfixExp(string infixExp)
{
	string tempStr = "";
	stack<char> tempStack;

	for (int i = 0; i < infixExp.length(); i++)
	{
		//숫자, (, ), +, -, *, / 구분
		char tempChar;
		tempChar = infixExp.at(i);

		if (tempChar == '(')
		{
			//'('를 무시하지않고 스택에 쌓기로 한다
			tempStack.push(tempChar);
		}
		else if (tempChar == ')')
		{
			//')' 와 짝을 찾을때까지 스택을 pop하고
			//찾지 못하고 empty가 되면 invalid)를 리턴함

			while (tempStack.top() != '(' && !tempStack.empty())
			{
				tempStr.push_back(' ');
				tempStr.push_back(tempStack.top());
				tempStack.pop();

				if (tempStack.empty())
				{
					return "invalid)";
				}
			}

			if (tempStack.top() == '(')
			{
				tempStack.pop();
			}
		}
		else if (tempChar > 47 && tempChar < 58)
		{
			//숫자일 경우 push_back
			tempStr.push_back(tempChar);
		}
		else if (tempChar == 46)
		{
			//'.'일 경우 tempStr의 마지막 문자가 숫자범위 일 경우 push_back
			//마지막 문자가 숫자범위가 아닐 경우 invalid.을 리턴함
			if (tempStr.at(tempStr.length()-1) > 47 && 
				tempStr.at(tempStr.length()-1) < 58)
			{
				tempStr.push_back(tempChar);
			}
			else
			{
				return "invalid.";
			}
		}
		else if (tempChar > 41 && tempChar < 48)
		{
			//','일 경우 invalid, 리턴
			//'.'일 경우는 위의 else if 절에서 필터링됨
			//나머지 사칙연산 기호만 처리
			if (tempChar == 44)
			{
				return "invalid,";
			}
			else if(tempChar == '+' || tempChar == '-')
			{
				tempStr.push_back(' ');

				while (!tempStack.empty() && tempStack.top() != '(')
				{
					tempStr.push_back(tempStack.top());
					tempStr.push_back(' ');
					tempStack.pop();
				}

				tempStack.push(tempChar);
			}
			else if (tempChar == '*' || tempChar == '/')
			{
				tempStr.push_back(' ');

				if (!tempStack.empty() && 
					tempStack.top() != '(' &&
					(tempStack.top() == '*' || tempStack.top() == '/'))
				{
					tempStr.push_back(tempStack.top());
					tempStr.push_back(' ');
					tempStack.pop();
				}

				tempStack.push(tempChar);
			}
		}
	}

	while (!tempStack.empty())
	{
		tempStr.push_back(' ');
		tempStr.push_back(tempStack.top());
		tempStack.pop();
	}

	return tempStr;
}
*/

