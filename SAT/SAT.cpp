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

void SAT::CreateExpression()
{
	/*SymboleAtomique* a = new SymboleAtomique(string("a"));
	m_Variables[0] = a;
	SymboleAtomique* b = new SymboleAtomique(string("b"));
	m_Variables[1] = b;
	SymboleAtomique* c = new SymboleAtomique(string("c"));
	m_Variables[2] = c;

	Not* notA = new Not(*a);
	And* notAandB = new And(*notA, *b);
	m_Expression = new Or(*notAandB, *c);*/
	//m_Expression = Parse(m_ExpressionString);
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

size_t SAT::GetCharParse(string _expression)
{
	return _expression.find('&');
}

Evaluable* SAT::Parse(string _expression) 
{
	size_t _parsingCharPos = GetCharParse(_expression);
	string _leftMember = _expression.substr(0, _parsingCharPos);
	string _rightMember = _expression.substr(_parsingCharPos+1);
	if (_parsingCharPos >= _expression.size()) {
		Evaluable* _symbole = GetSymbole(_expression);
		if (_symbole) return _symbole;
		SymboleAtomique* _atome = new SymboleAtomique(_expression);
		m_Variables[m_NbCurrentVariables] = _atome;
		m_NbCurrentVariables++;
		return _atome;
	}

	switch (_expression.at(_parsingCharPos))
	{
		cout << _parsingCharPos << endl;
		case '&':
			return new And(Parse(_leftMember), Parse(_rightMember));

		case '|':
			return new Or(Parse(_leftMember), Parse(_rightMember));

		case '!':
			return new Not(Parse(_rightMember));

		default:
			Evaluable* _symbole = GetSymbole(_expression);
			if (_symbole) return _symbole;
			SymboleAtomique* _atome = new SymboleAtomique(_expression);
			m_Variables[m_NbCurrentVariables] = _atome;
			m_NbCurrentVariables++;
			return _atome;
	}
}

int SAT::CalculateStatements()
{
	unsigned short _nbTrueStatement = 0;
	cout << m_NbVariables << endl;
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
