#pragma once
#include <string>
using namespace std;

#include "Evaluable.h"

class SymboleAtomique : public Evaluable
{
private:
	bool	m_Value;
	string	m_Symbole;

public:
	SymboleAtomique();
	SymboleAtomique(const string);
	SymboleAtomique(const bool);
	SymboleAtomique(const string, const bool);

public:
	void SetValue(const bool);
	string GetSymbole() const;

public:
	bool Evaluate() const override;
};

