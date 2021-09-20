#include "Not.h"

Not::Not(Evaluable* _litt)
{
    m_Litt = _litt;
}

Evaluable* Not::GetLitt() const
{
    return m_Litt;
}

bool Not::Evaluate() const
{
    if (!m_Litt) return false;
    return !m_Litt->Evaluate();
}
