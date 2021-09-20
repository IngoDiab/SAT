#pragma once
#include "Evaluable.h"
class And : public Evaluable
{
private:
	Evaluable* m_LeftLitt;
	Evaluable* m_RightLitt;

public:
	And(Evaluable*, Evaluable*);

public:
	Evaluable* GetLeftLitt() const;
	Evaluable* GetRightLitt() const;

public:
	bool Evaluate() const override;
};

