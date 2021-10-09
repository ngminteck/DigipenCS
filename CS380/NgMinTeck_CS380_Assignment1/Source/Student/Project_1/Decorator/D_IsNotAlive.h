#pragma once
#include "BehaviorNode.h"

class D_IsNotAlive : public BaseNode<D_IsNotAlive>
{
public:

protected:
	virtual void on_enter() override;
	virtual void on_update(float dt) override;
};
