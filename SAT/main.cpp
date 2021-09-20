#include <vector>
#include <iostream>
using namespace std;

#include "SAT.h"

int main()
{
	unsigned short _nbVariable = 0;
	string _expression;

	cout << "Nombre de variables : ";
	cin >> _nbVariable;

	cout << "Expression : ";
	cin >> _expression;
	cout << "\n";

	SAT* _SAT = new SAT(string(_expression), _nbVariable);
	_SAT->Launch();

	delete _SAT;
	return 0;
}
