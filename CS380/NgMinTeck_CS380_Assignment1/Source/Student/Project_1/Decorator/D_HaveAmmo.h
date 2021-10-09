#pragma once
#include "BehaviorNode.h"

class D_HaveAmmo : public BaseNode<D_HaveAmmo>
{
public:

protected:
	virtual void on_enter() override;
	virtual void on_update(float dt) override;
};