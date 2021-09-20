#pragma once
#include "Evaluable.h"

class Or : public Evaluable
{
private:
	Evaluable* m_LeftLitt;
	Evaluable* m_RightLitt;

public:
	Or(Evaluable*, Evaluable*);

public:
	Evaluable* GetLeftLitt() const;
	Evaluable* GetRightLitt() const;

public:
	bool Evaluate() const override;
};

