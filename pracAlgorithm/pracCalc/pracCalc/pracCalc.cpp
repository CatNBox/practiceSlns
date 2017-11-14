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

	cout << "�Էµ� �� : " << endl << infixExp << endl;

	postfixExp = getPostfixExp(infixExp);
	
	cout << "��ȯ�� �� : " << endl << postfixExp << endl;

	cin >> infixExp;
	return 0;
}

string getPostfixExp(string infixExp)
{
	string tempStr = "";
	stack<char> tempStack;

	for (int i = 0; i < infixExp.length(); i++)
	{
		//����, (, ), +, -, *, / ����
		char tempChar;
		tempChar = infixExp.at(i);

		if (tempChar == '(')
		{
			//'('�� ���������ʰ� ���ÿ� �ױ�� �Ѵ�
			tempStack.push(tempChar);
		}
		else if (tempChar == ')')
		{
			//')' �� ¦�� ã�������� ������ pop�ϰ�
			//ã�� ���ϰ� empty�� �Ǹ� invalid)�� ������

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
			//������ ��� push_back
			tempStr.push_back(tempChar);
		}
		else if (tempChar == 46)
		{
			//'.'�� ��� tempStr�� ������ ���ڰ� ���ڹ��� �� ��� push_back
			//������ ���ڰ� ���ڹ����� �ƴ� ��� invalid.�� ������
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
			//','�� ��� invalid, ����
			//'.'�� ���� ���� else if ������ ���͸���
			//������ ��Ģ���� ��ȣ�� ó��
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

