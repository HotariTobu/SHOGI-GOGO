#include "Tatami.h"

Tatami::Tatami(ISceneNode* parent, ISceneManager* mgr, s32 id, ScaleDefiner* sd, bool v) : IObject(parent, mgr, id, sd) {
	SMeshBuffer* meshBuffer = new SMeshBuffer();

	SColor color = SColor(255, 255, 255, 255);

	f32 height = sd->getScale(ESN_TATAMI_HEIGHT);

	meshBuffer->Vertices.set_used(4);
	if (v) {
		meshBuffer->Vertices[0] = S3DVertex(0, 0, 0, 0, 1, 0, color, 0, 1);
		meshBuffer->Vertices[1] = S3DVertex(height, 0, 0, 0, 1, 0, color, 0, 0);
		meshBuffer->Vertices[2] = S3DVertex(height, 0, -height * 2, 0, 1, 0, color, 1, 0);
		meshBuffer->Vertices[3] = S3DVertex(0, 0, -height * 2, 0, 1, 0, color, 1, 1);
	}
	else {
		meshBuffer->Vertices[0] = S3DVertex(0, 0, 0, 0, 1, 0, color, 0, 0);
		meshBuffer->Vertices[1] = S3DVertex(height * 2, 0, 0, 0, 1, 0, color, 1, 0);
		meshBuffer->Vertices[2] = S3DVertex(height * 2, 0, -height, 0, 1, 0, color, 1, 1);
		meshBuffer->Vertices[3] = S3DVertex(0, 0, -height, 0, 1, 0, color, 0, 1);
	}

	meshBuffer->Indices.set_used(6);
	u16 indices[] = { 0,1,2, 0,2,3 };
	for (s32 i = 0; i < 6; i++) {
		meshBuffer->Indices[i] = indices[i];
	}

	meshBuffer->Material.Wireframe = false;
	meshBuffer->Material.Lighting = false;

	SMesh* originalMesh = new SMesh();
	originalMesh->addMeshBuffer(meshBuffer);
	mesh = originalMesh;

	aabbox3df boundingBox;
	boundingBox.reset(meshBuffer->Vertices[0].Pos);
	for (s32 i = 1; i < 4; ++i) {
		boundingBox.addInternalPoint(meshBuffer->Vertices[i].Pos);
	}
	mesh->setBoundingBox(boundingBox);
}

Tatami::~Tatami() {
	IObject::~IObject();
}

Tatami* Tatami::clone(ISceneNode* newParent, ISceneManager* newManager) {
	if (!newParent) {
		newParent = Parent;
	}
	if (!newManager) {
		newManager = SceneManager;
	}

	Tatami* clone = new Tatami(newParent, newManager, ID, sd);
	clone->mesh = SceneManager->getMeshManipulator()->createMeshCopy(mesh);
	return clone;
}