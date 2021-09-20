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

/// <summary>
/// Launch the generation of the SAT
/// </summary>
void SAT::GenerateSolution()
{
	m_Expression = Parse(m_ExpressionString);
	const unsigned short _nbTrueStatement = CalculateStatements();
	CheckCaractExpr(_nbTrueStatement);
}

/// <summary>
/// Return the symbol if it's contains in the variables, nullptr otherwise
/// </summary>
/// <param name="_symbole">: Symbole searched</param>
/// <returns></returns>
Evaluable* SAT::GetSymbole(string _symbole) 
{
	if (m_NbCurrentVariables == 0) return nullptr;
	for (unsigned int i = 0; i < m_NbCurrentVariables; i++)
	{
		if (m_Variables[i]->GetSymbole() == _symbole) return m_Variables[i];
	}
	return nullptr;

}

/// <summary>
/// Return the expression without useless parentheses
/// </summary>
/// <param name="_expression">: Expression to clean</param>
/// <returns></returns>
string SAT::RemoveParanthese(string _expression)
{
	vector<size_t> _openParanthese;
	vector<size_t> _closeParanthese;
	for (int i = 0; i < _expression.size(); i++)
	{
		if (_expression.at(i) == SYMBOL_PARANTHESE_OPEN) _openParanthese.push_back(i);
		else if (_expression.at(i) == SYMBOL_PARANTHESE_CLOSE) _closeParanthese.push_back(i);
	}

	const bool _existsParanthese = !_openParanthese.size() == 0 && !_closeParanthese.size() == 0;
	if (!_existsParanthese) return _expression;
	const bool _onlyOnePair = _openParanthese.size() == 1 && _closeParanthese.size() == 1;
	bool _canRemoveParanthese = false;

	if (_onlyOnePair) _canRemoveParanthese = _openParanthese.at(0) == 0 && _closeParanthese.at(0) == _expression.size() - 1;
	else _canRemoveParanthese = _openParanthese.at(_openParanthese.size() - 1) < _closeParanthese.at(_closeParanthese.size() - 2);

	if (_canRemoveParanthese) return RemoveParanthese(_expression.substr(1, _expression.size() - 2));
	return _expression;
}

/// <summary>
/// Check if _char1 has less priority than _char2
/// </summary>
/// <param name="_char1">: First char</param>
/// <param name="_char2">: Second char</param>
/// <returns></returns>
bool SAT::IsLessImportant(char _char1, char _char2) 
{
	switch (_char1) 
	{
		case SYMBOL_OR:
			return false;
		case SYMBOL_AND:
			return _char2 == SYMBOL_OR;
		case SYMBOL_NOT:
			return _char2 == SYMBOL_OR || _char2 == SYMBOL_AND;
		default:
			return true;
	}
}

/// <summary>
/// Returns the index of the symbole which has the less priority in the expression
/// </summary>
/// <param name="_expression">: Expression to check</param>
/// <returns></returns>
size_t SAT::GetCharParse(string _expression)
{
	cout << _expression << endl;
	int _nbParanthese = 0;
	int _nbParantheseMin = _expression.size();
	int _charIndex = -1;
	char _currentChar = ' ';
	for (int i = 0; i < _expression.size(); i++)
	{
		if (_expression.at(i) == SYMBOL_PARANTHESE_OPEN)
		{
			_nbParanthese++;
			continue;
		}
		else if (_expression.at(i) == SYMBOL_PARANTHESE_CLOSE)
		{
			_nbParanthese--;
			continue;
		}

		if (_expression.at(i) == SYMBOL_AND || _expression.at(i) == SYMBOL_OR || _expression.at(i) == SYMBOL_NOT) {

			if (_nbParanthese < _nbParantheseMin)
			{
				_nbParantheseMin = _nbParanthese;
				_charIndex = i;
				_currentChar = _expression.at(i);
			}
			else if (_nbParanthese == _nbParantheseMin && IsLessImportant(_currentChar, _expression.at(i)))
			{
				_charIndex = i;
				_currentChar = _expression.at(i);
			}
		}
	}
	return _charIndex;
}

/// <summary>
/// Parse an expression with OR, AND or ! symbole
/// </summary>
/// <param name="_expression">: Expression to Parse</param>
/// <returns></returns>
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

	switch (_parantheseRemoved.at(_parsingCharPos))
	{
		case SYMBOL_AND:
			return new And(Parse(_leftMember), Parse(_rightMember));

		case SYMBOL_OR:
			return new Or(Parse(_leftMember), Parse(_rightMember));

		case SYMBOL_NOT:
			return new Not(Parse(_rightMember));
	}
}

/// <summary>
/// Returns the number of true statements for the SAT's expression
/// </summary>
/// <returns></returns>
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

/// <summary>
/// Generate true/false for each atomic symbole thanks to the iteration
/// </summary>
/// <param name="_iteration">Number of the current iteration</param>
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

/// <summary>
/// Check if the expression is tautologique,...
/// </summary>
/// <param name="_nbTrueStatement">: Number of true statements</param>
void SAT::CheckCaractExpr(const unsigned short _nbTrueStatement)
{
	if (_nbTrueStatement == pow(2, m_NbVariables)) cout << "\nL'expression est tautologique." << endl;
	else if (_nbTrueStatement == 0) cout << "\nL'expression est insatisfiable." << endl;
	else cout << "\nL'expression est satisfiable." << endl;
}
