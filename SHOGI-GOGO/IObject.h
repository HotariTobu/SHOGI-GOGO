#pragma once

#ifndef __I_OBJECT_H_INCLUDED__
#define __I_OBJECT_H_INCLUDED__

#include "MyHead.h"
#include "ScaleDefiner.h"

class IObject : public ISceneNode {
public:
	IObject(ISceneNode* parent, ISceneManager* mgr, s32 id, ScaleDefiner* sd) : ISceneNode(parent, mgr, id), sd(sd), mesh(0) {}
	virtual ~IObject();

	virtual void OnRegisterSceneNode() override;
	virtual void render() override;
	virtual const aabbox3d<f32>& getBoundingBox() const override;
	virtual u32 getMaterialCount() const override;
	virtual SMaterial& getMaterial(u32 i) override;

protected:
	ScaleDefiner* sd;
	IMesh* mesh;

};

#endif // !__I_OBJECT_H_INCLUDED__