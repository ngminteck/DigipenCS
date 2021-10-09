#pragma once
#include "BehaviorNode.h"

class D_Fire : public BaseNode<D_Fire>
{
public:

protected:
	virtual void on_enter() override;
	virtual void on_update(float dt) override;
};