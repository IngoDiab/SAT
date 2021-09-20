#include "SymboleAtomique.h"

SymboleAtomique::SymboleAtomique()
{
    m_Value = false;
    m_Symbole = "";
}

SymboleAtomique::SymboleAtomique(string _symbole)
{
    m_Symbole = _symbole;
}

SymboleAtomique::SymboleAtomique(const bool _value)
{
    m_Value = _value;
}

SymboleAtomique::SymboleAtomique(const string _symbole, const bool _value)
{
    m_Symbole = _symbole;
    m_Value = _value;
}

void SymboleAtomique::SetValue(const bool _value)
{
    m_Value = _value;
}

string SymboleAtomique::GetSymbole() const
{
    return m_Symbole;
}

bool SymboleAtomique::Evaluate() const
{
    return m_Value;
}
