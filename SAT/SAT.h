#pragma once
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
	void Launch();
	void CreateExpression();
	Evaluable* GetSymbole(string _symbole);
	bool ContainsSymbole(string _symbole);
	size_t GetCharParse(string _expression);
	Evaluable* Parse(string _expression);
	int CalculateStatements();
	void SetVariablesValue(const unsigned short _iteration);
	void CheckCaractExpr(const unsigned short _nbTrueStatement);
};

