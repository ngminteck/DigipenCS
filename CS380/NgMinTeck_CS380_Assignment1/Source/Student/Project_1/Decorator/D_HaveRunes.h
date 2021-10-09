#pragma once
#include "BehaviorNode.h"

class D_HaveRunes : public BaseNode<D_HaveRunes>
{
public:

protected:
	virtual void on_enter() override;
	virtual void on_update(float dt) override;
};