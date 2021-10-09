#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"

class L_RandomRespawn : public BaseNode<L_RandomRespawn>
{
protected:
	virtual void on_enter() override;
	virtual void on_update(float dt) override;

private:
	Vec3 pos;
};