#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"

class L_MoveToPosition : public BaseNode<L_MoveToPosition>
{
protected:
	virtual void on_enter() override;
	virtual void on_update(float dt) override;

private:

};