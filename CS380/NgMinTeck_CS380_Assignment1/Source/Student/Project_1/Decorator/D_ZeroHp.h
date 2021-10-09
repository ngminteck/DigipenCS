#pragma once
#include "BehaviorNode.h"

class D_ZeroHp : public BaseNode<D_ZeroHp>
{
public:

protected:
	virtual void on_enter() override;
	virtual void on_update(float dt) override;
};
