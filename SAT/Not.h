#pragma once
#include "Evaluable.h"
class Not : public Evaluable
{
private:
	Evaluable* m_Litt;

public:
	Not(Evaluable*);

public:
	Evaluable* GetLitt() const;

public:
	bool Evaluate() const override;
};

