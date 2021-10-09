#pragma once
#include "BehaviorNode.h"

class D_HaveVaildTarget : public BaseNode<D_HaveVaildTarget>
{
public:

protected:
	virtual void on_enter() override;
	virtual void on_update(float dt) override;
};