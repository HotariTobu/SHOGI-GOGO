#pragma once

#ifndef __TATAMI_H_INCLUDED__
#define __TATAMI_H_INCLUDED__

#include "IObject.h"

class Tatami : public IObject {
public:
	Tatami(ISceneNode* parent, ISceneManager* mgr, s32 id, ScaleDefiner* sd, bool v);
	~Tatami();

	Tatami* clone(ISceneNode* newParent = 0, ISceneManager * newManager = 0) override;

private:
	Tatami(ISceneNode* parent, ISceneManager* mgr, s32 id, ScaleDefiner* sd) : IObject(parent, mgr, id, sd) {}
};

#endif // !__TATAMI_H_INCLUDED__