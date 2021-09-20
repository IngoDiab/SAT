#include "And.h"

And::And(Evaluable* _left, Evaluable* _right)
{
	m_LeftLitt = _left;
	m_RightLitt = _right;
}

Evaluable* And::GetLeftLitt() const
{
	return m_LeftLitt;
}

Evaluable* And::GetRightLitt() const
{
	return m_RightLitt;
}

bool And::Evaluate() const
{
	if (!m_LeftLitt || !m_RightLitt) return false;
	return m_LeftLitt->Evaluate() && m_RightLitt->Evaluate();
}
