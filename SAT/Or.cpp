#include "Or.h"

Or::Or(Evaluable* _left, Evaluable* _right)
{
	m_LeftLitt = _left;
	m_RightLitt = _right;
}

Evaluable* Or::GetLeftLitt() const
{
	return m_LeftLitt;
}

Evaluable* Or::GetRightLitt() const
{
	return m_RightLitt;
}

bool Or::Evaluate() const
{
	if (!m_LeftLitt || !m_RightLitt) return false;
	return m_LeftLitt->Evaluate() || m_RightLitt->Evaluate();
}
