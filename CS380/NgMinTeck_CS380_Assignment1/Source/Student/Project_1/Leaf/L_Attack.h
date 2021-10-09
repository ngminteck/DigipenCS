#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"

class L_Attack : public BaseNode<L_Attack>
{
protected:
	virtual void on_enter() override;
	virtual void on_update(float dt) override;

private:

};