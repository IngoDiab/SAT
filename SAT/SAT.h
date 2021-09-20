#pragma once

#define SYMBOL_OR '|'
#define SYMBOL_AND '&'
#define SYMBOL_NOT '!'
#define SYMBOL_PARANTHESE_OPEN '('
#define SYMBOL_PARANTHESE_CLOSE ')'

class Evaluable;
class SymboleAtomique;

class SAT
{
private:
	string				m_ExpressionString;
	Evaluable*			m_Expression;
	SymboleAtomique**	m_Variables;
	unsigned short		m_NbVariables;
	unsigned short		m_NbCurrentVariables;

public:
	SAT(string _expression, unsigned short _nbVariable);
	~SAT();

public:
	void GenerateSolution();
	Evaluable* GetSymbole(string _symbole);
	bool ContainsSymbole(string _symbole);
	string RemoveParanthese(string _expression);
	bool IsLessImportant(char _char1, char _char2);
	size_t GetCharParse(string _expression);
	Evaluable* Parse(string _expression);
	int CalculateStatements();
	void SetVariablesValue(const unsigned short _iteration);
	void CheckCaractExpr(const unsigned short _nbTrueStatement);
};

