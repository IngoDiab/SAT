#include <vector>
#include <iostream>
using namespace std;

#include "SAT.h"
#include "SymboleAtomique.h"
#include "Not.h"
#include "And.h"
#include "Or.h"


SAT::SAT(string _expression, unsigned short _nbVariable) : m_ExpressionString(_expression), m_NbVariables(_nbVariable), m_NbCurrentVariables(0)
{
	m_Variables = new SymboleAtomique*[_nbVariable];
}

SAT::~SAT()
{
	delete m_Expression;
	delete[] m_Variables;
}

void SAT::Launch()
{
	m_Expression = Parse(m_ExpressionString);
	unsigned short _nbTrueStatement = CalculateStatements();
	CheckCaractExpr(_nbTrueStatement);
}

Evaluable* SAT::GetSymbole(string _symbole) 
{
	if (m_NbCurrentVariables == 0) return nullptr;
	for (size_t i = 0; i < m_NbCurrentVariables; i++)
	{
		if (m_Variables[i]->GetSymbole() == _symbole) return m_Variables[i];
	}
	return nullptr;

}

bool SAT::ContainsSymbole(string _symbole)
{
	if (m_NbCurrentVariables == 0 || m_NbCurrentVariables >= m_NbVariables)return true;
	for (size_t i = 0; i < m_NbCurrentVariables; i++)
	{
		if (m_Variables[i]->GetSymbole() == _symbole) return true;
	}
	return false;
}

string SAT::RemoveParanthese(string _expression)
{
	vector<size_t> _openParanthese;
	vector<size_t> _closeParanthese;
	for (int i = 0; i < _expression.size(); i++)
	{
		if (_expression.at(i) == '(') _openParanthese.push_back(i);
		else if (_expression.at(i) == ')') _closeParanthese.push_back(i);
	}

	const bool _existsParanthese = !_openParanthese.size() == 0 && !_closeParanthese.size() == 0;
	if (!_existsParanthese) return _expression;
	const bool _onlyOnePair = _openParanthese.size() == 1 && _closeParanthese.size() == 1;
	bool _canRemoveParanthese = false;

	if (_onlyOnePair) _canRemoveParanthese = _openParanthese.at(0) == 0 && _closeParanthese.at(0) == _expression.size() - 1;
	else _canRemoveParanthese = _openParanthese.at(_openParanthese.size() - 1) < _closeParanthese.at(_closeParanthese.size() - 2);

	if (_canRemoveParanthese) return _expression.substr(1, _expression.size() - 2);
	return _expression;
}


size_t SAT::GetCharParse(string _expression)
{
	const unsigned int _firstOr = _expression.find('|');
	if (_firstOr < _expression.size()) return _firstOr;
	const unsigned int _firstAnd = _expression.find('&');
	if (_firstAnd < _expression.size()) return _firstAnd;
	const unsigned int _firstNot = _expression.find('!');
	if (_firstNot < _expression.size()) return _firstNot;
	return -1;
}

Evaluable* SAT::Parse(string _expression) 
{
	string _parantheseRemoved = RemoveParanthese(_expression);
	size_t _parsingCharPos = GetCharParse(_parantheseRemoved);
	if (_parsingCharPos == -1) 
	{
		Evaluable* _symbole = GetSymbole(_parantheseRemoved);
		if (_symbole) return _symbole;
		SymboleAtomique* _atome = new SymboleAtomique(_parantheseRemoved);
		m_Variables[m_NbCurrentVariables] = _atome;
		m_NbCurrentVariables++;
		return _atome;
	}

	string _leftMember = _parantheseRemoved.substr(0, _parsingCharPos);
	string _rightMember = _parantheseRemoved.substr(_parsingCharPos+1);
	//cout << _leftMember << endl;
	//cout << _rightMember << endl;

	switch (_parantheseRemoved.at(_parsingCharPos))
	{
		case '&':
			return new And(Parse(_leftMember), Parse(_rightMember));

		case '|':
			return new Or(Parse(_leftMember), Parse(_rightMember));

		case '!':
			return new Not(Parse(_rightMember));
	}
}

int SAT::CalculateStatements()
{
	unsigned short _nbTrueStatement = 0;
	for (unsigned short i = 0; i < pow(2, m_NbVariables); i++)
	{
		SetVariablesValue(i);
		const bool _statementIsTrue = m_Expression->Evaluate();
		cout << boolalpha << "Expression : " << _statementIsTrue << endl;
		if (_statementIsTrue) _nbTrueStatement++;
	}
	return _nbTrueStatement;
}

void SAT::SetVariablesValue(const unsigned short _iteration)
{
	for (unsigned short j = 0; j < m_NbVariables; j++)
	{
		SymboleAtomique* _var = m_Variables[j];
		if (!_var)continue;
		_var->SetValue((_iteration & (int)pow(2, j)) == 0);
		cout << boolalpha << _var->GetSymbole() << " : " << _var->Evaluate() << "  ";
	}
}

void SAT::CheckCaractExpr(const unsigned short _nbTrueStatement)
{
	if (_nbTrueStatement == pow(2, m_NbVariables)) cout << "\nL'expression est tautologique." << endl;
	else if (_nbTrueStatement == 0) cout << "\nL'expression est insatisfiable." << endl;
	else cout << "\nL'expression est satisfiable." << endl;
}
