#include "IObject.h"

IObject::~IObject() {
	ISceneNode::~ISceneNode();

	if (mesh) {
		mesh->drop();
	}
}

void IObject::OnRegisterSceneNode() {
	if (IsVisible) {
		SceneManager->registerNodeForRendering(this);
	}

	ISceneNode::OnRegisterSceneNode();
}

void IObject::render() {
	IVideoDriver* drv = SceneManager->getVideoDriver();

	drv->setTransform(ETS_WORLD, AbsoluteTransformation);

	u32 meshBufferCount = mesh->getMeshBufferCount();
	for (u32 i = 0; i < meshBufferCount; i++) {
		IMeshBuffer* meshBuffer = mesh->getMeshBuffer(i);
		drv->setMaterial(meshBuffer->getMaterial());
		drv->drawMeshBuffer(meshBuffer);
	}
}

const aabbox3d<f32>& IObject::getBoundingBox() const {
	return mesh->getBoundingBox();
}

u32 IObject::getMaterialCount() const {
	return mesh->getMeshBufferCount();
}

SMaterial& IObject::getMaterial(u32 i) {
	return mesh->getMeshBuffer(i)->getMaterial();
}