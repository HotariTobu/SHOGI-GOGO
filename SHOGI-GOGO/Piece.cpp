#include "Piece.h"

Piece::Piece(ISceneNode* parent, ISceneManager* mgr, s32 id, ScaleDefiner* sd, f32 rate) : IObject(parent, mgr, id, sd), rate(rate), promotion(false) {
	//h
	f32 height = sd->getScale(ESN_PIECE_HEIGHT);

	f32 backHeight = height * 0.3f;

	vector2df A, B;
	//a
	f32 ratio = height / (sin18 + rate * sin81);
	//ab
	f32 ratioRate = ratio * rate;

	A.X = ratio * cos18;
	A.Y = ratio * -sin18;
	f32 ABX = ratioRate * cos81;
	B.X = ABX + A.X;
	B.Y = -height;

	//e
	f32 radiusLong = B.X * proE;
	//d
	f32 radiusOut = radiusLong - ratioRate;

	radius = ratio * cos108 + sqrtf(ratio * ratio * cos2p108s1 + radiusOut * radiusOut);

	//g
	f32 radiusIn = ratio * cos63;
	//j
	f32 radiusShort = radiusIn + ratioRate;
	//f
	f32 frontHeight = backHeight - radiusShort * cos85 * 2;
	//i
	f32 middleHeight = (radiusIn * backHeight + ratioRate * frontHeight) / radiusShort;

	vector2df ADif, BDif;
	ADif.X = middleHeight * cos85;
	ADif.Y = middleHeight * sin85;
	BDif.X = backHeight * cos85;
	BDif.Y = backHeight * sin85;

	Vectors[0] = vector3df(0, 0, 0);
	Vectors[1] = vector3df(A.X, 0, A.Y);
	Vectors[2] = vector3df(B.X, 0, B.Y);
	Vectors[3] = vector3df(-A.X, 0, A.Y);
	Vectors[4] = vector3df(-B.X, 0, B.Y);
	Vectors[5] = vector3df(0, frontHeight * sin85, frontHeight * cos85);
	Vectors[6] = vector3df(A.X, ADif.Y, A.Y + ADif.X);
	Vectors[7] = vector3df(B.X, BDif.Y, B.Y + BDif.X);
	Vectors[8] = vector3df(-A.X, ADif.Y, A.Y + ADif.X);
	Vectors[9] = vector3df(-B.X, BDif.Y, B.Y + BDif.X);

	f32 URate = 0.5f / (ABX + backHeight + B.X * 2);
	f32 VRate = 1.0f / (height + backHeight);

	Coordinates[0] = vector2df(URate * (ABX + backHeight + B.X), 0);
	Coordinates[1] = vector2df(0.5f - URate * ABX, VRate * -A.Y);
	Coordinates[2] = vector2df(0.5f, VRate * height);
	Coordinates[3] = vector2df(URate * (ABX * 2 + backHeight), Coordinates[1].Y);
	Coordinates[4] = vector2df(URate * (ABX + backHeight), Coordinates[2].Y);

	f32 addition = 0.5f - URate * ABX;
	for (u32 i = 5; i < 10; i++) {
		Coordinates[i] = Coordinates[i - 5];
		Coordinates[i].X += addition;
	}

	Coordinates[10] = vector2df(0, Coordinates[1].Y);
	Coordinates[11] = vector2df(URate * ABX, Coordinates[2].Y);
	Coordinates[12] = vector2df(Coordinates[2].X, 1);
	Coordinates[13] = vector2df(Coordinates[4].X, 1);
	Coordinates[14] = vector2df(0, Coordinates[0].Y);
	Coordinates[15] = vector2df(Coordinates[1].X, Coordinates[0].Y);

	setPromotion(false);

	aabbox3df boundingBox;
	boundingBox.reset(Vectors[0]);
	for (s32 i = 1; i < 10; ++i) {
		boundingBox.addInternalPoint(Vectors[i]);
	}
	mesh->setBoundingBox(boundingBox);
}

Piece::~Piece() {
	IObject::~IObject();
}

Piece* Piece::clone(ISceneNode* newParent, ISceneManager* newManager) {
	if (!newParent) {
		newParent = Parent;
	}
	if (!newManager) {
		newManager = SceneManager;
	}

	Piece* clone = new Piece(newParent, newManager, ID, sd);
	clone->mesh = SceneManager->getMeshManipulator()->createMeshCopy(mesh);

	for (u32 i = 0; i < 10; i++) {
		clone->Vectors[i] = Vectors[i];
	}
	for (u32 i = 0; i < 16; i++) {
		clone->Coordinates[i] = Coordinates[i];
	}
	clone->rate = rate;
	clone->radius = radius;
	clone->promotion = promotion;

	return clone;
}

f32 Piece::getRadius() const {
	return radius;
}

void Piece::setPromotion(bool promotion) {
	this->promotion = promotion;

	SMeshBuffer* meshBuffer = new SMeshBuffer();
	meshBuffer->Vertices.set_used(16);

	SColor color(255, 255, 255, 255);

	if (promotion) {
		meshBuffer->Vertices[0] = S3DVertex(Vectors[0], vector3df(0, -1, -1), color, Coordinates[5]);
		meshBuffer->Vertices[1] = S3DVertex(Vectors[1], vector3df(1, -1, -1), color, Coordinates[8]);
		meshBuffer->Vertices[2] = S3DVertex(Vectors[2], vector3df(1, -1, 1), color, Coordinates[9]);
		meshBuffer->Vertices[3] = S3DVertex(Vectors[3], vector3df(-1, -1, -1), color, Coordinates[6]);
		meshBuffer->Vertices[4] = S3DVertex(Vectors[4], vector3df(-1, -1, 1), color, Coordinates[7]);

		meshBuffer->Vertices[5] = S3DVertex(Vectors[5], vector3df(0, 1, -1), color, Coordinates[0]);
		meshBuffer->Vertices[6] = S3DVertex(Vectors[6], vector3df(1, 1, -1), color, Coordinates[1]);
		meshBuffer->Vertices[7] = S3DVertex(Vectors[7], vector3df(1, 1, 1), color, Coordinates[2]);
		meshBuffer->Vertices[8] = S3DVertex(Vectors[8], vector3df(-1, 1, -1), color, Coordinates[3]);
		meshBuffer->Vertices[9] = S3DVertex(Vectors[9], vector3df(-1, 1, 1), color, Coordinates[4]);

		meshBuffer->Vertices[10] = S3DVertex(Vectors[3], vector3df(1, 1, -1), color, Coordinates[10]);
		meshBuffer->Vertices[11] = S3DVertex(Vectors[4], vector3df(1, 1, 1), color, Coordinates[11]);
		meshBuffer->Vertices[12] = S3DVertex(Vectors[2], vector3df(-1, 1, 1), color, Coordinates[12]);
		meshBuffer->Vertices[13] = S3DVertex(Vectors[4], vector3df(1, 1, 1), color, Coordinates[13]);
		meshBuffer->Vertices[14] = S3DVertex(Vectors[0], vector3df(0, 1, -1), color, Coordinates[14]);
		meshBuffer->Vertices[15] = S3DVertex(Vectors[5], vector3df(0, -1, -1), color, Coordinates[15]);

		u16 Indices[] = { 0,2,1, 0,4,2, 0,3,4, 0,1,6, 1,2,7, 12,13,9, 11,10,8, 10,14,5, 0,6,15, 1,7,6, 12,9,7, 11,8,9, 10,5,8, 5,9,8, 5,7,9, 5,6,7 };
		meshBuffer->Indices.set_pointer(Indices, 48);
	}
	else {
		meshBuffer->Vertices[0] = S3DVertex(Vectors[0], vector3df(0, -1, -1), color, Coordinates[0]);
		meshBuffer->Vertices[1] = S3DVertex(Vectors[1], vector3df(1, -1, -1), color, Coordinates[3]);
		meshBuffer->Vertices[2] = S3DVertex(Vectors[2], vector3df(1, -1, 1), color, Coordinates[4]);
		meshBuffer->Vertices[3] = S3DVertex(Vectors[3], vector3df(-1, -1, -1), color, Coordinates[1]);
		meshBuffer->Vertices[4] = S3DVertex(Vectors[4], vector3df(-1, -1, 1), color, Coordinates[2]);

		meshBuffer->Vertices[5] = S3DVertex(Vectors[5], vector3df(0, 1, -1), color, Coordinates[5]);
		meshBuffer->Vertices[6] = S3DVertex(Vectors[6], vector3df(1, 1, -1), color, Coordinates[6]);
		meshBuffer->Vertices[7] = S3DVertex(Vectors[7], vector3df(1, 1, 1), color, Coordinates[7]);
		meshBuffer->Vertices[8] = S3DVertex(Vectors[8], vector3df(-1, 1, -1), color, Coordinates[8]);
		meshBuffer->Vertices[9] = S3DVertex(Vectors[9], vector3df(-1, 1, 1), color, Coordinates[9]);

		meshBuffer->Vertices[10] = S3DVertex(Vectors[6], vector3df(1, 1, -1), color, Coordinates[10]);
		meshBuffer->Vertices[11] = S3DVertex(Vectors[7], vector3df(1, 1, 1), color, Coordinates[11]);
		meshBuffer->Vertices[12] = S3DVertex(Vectors[9], vector3df(-1, 1, 1), color, Coordinates[12]);
		meshBuffer->Vertices[13] = S3DVertex(Vectors[7], vector3df(1, 1, 1), color, Coordinates[13]);
		meshBuffer->Vertices[14] = S3DVertex(Vectors[5], vector3df(0, 1, -1), color, Coordinates[14]);
		meshBuffer->Vertices[15] = S3DVertex(Vectors[0], vector3df(0, -1, -1), color, Coordinates[15]);

		u16 Indices[] = { 0,2,1, 0,4,2, 0,3,4, 0,1,10, 1,2,11, 2,4,12, 4,3,8, 3,15,5, 0,10,14, 1,11,10, 2,12,13, 4,8,9, 3,5,8, 5,6,7, 5,7,9, 5,9,8 };
		meshBuffer->Indices.set_pointer(Indices, 48);
	}

	if (mesh) {
		meshBuffer->Material = getMaterial(0);
	}
	else {
		meshBuffer->Material.Wireframe = false;
		meshBuffer->Material.Lighting = false;
	}

	SMesh* originalMesh = new SMesh();
	originalMesh->addMeshBuffer(meshBuffer);
	if (mesh) {
		mesh->drop();
	}
	convertMesh(originalMesh);
	mesh = SceneManager->getMeshManipulator()->createMeshWithTangents(originalMesh);
	originalMesh->drop();
}

bool Piece::isPromoted() const {
	return promotion;
}

#include <IWriteFile.h>

void Piece::temp() {
	IVideoDriver* drv = SceneManager->getVideoDriver();

	f32 height = sd->getScale(ESN_PIECE_HEIGHT);
	f32 backHeight = height * 0.3f;
	u32 UVHeight = (u32)((height + backHeight) * 0.5f / (height / (sin18 + rate * sin81) * rate * cos81 + backHeight + (height / (sin18 + rate * sin81) * rate * cos81 + height / (sin18 + rate * sin81) * cos18) * 2) * 1024);

	vector2di points[16];
	for (s32 i = 0; i < 16; i++) {
		points[i].X = (s32)(Coordinates[i].X * 1024);
		points[i].Y = (s32)(Coordinates[i].Y * UVHeight);
	}

	ITexture* target = drv->addRenderTargetTexture(dimension2du(1024, UVHeight));
	drv->setRenderTarget(target);

	drv->draw2DLine(points[0], points[1]);
	drv->draw2DLine(points[1], points[2]);
	drv->draw2DLine(points[0], points[3]);
	drv->draw2DLine(points[3], points[4]);
	drv->draw2DLine(points[2], points[4]);
	drv->draw2DLine(points[5], points[6]);
	drv->draw2DLine(points[6], points[7]);
	drv->draw2DLine(points[5], points[8]);
	drv->draw2DLine(points[8], points[9]);
	drv->draw2DLine(points[7], points[9]);
	drv->draw2DLine(points[1], points[8]);
	drv->draw2DLine(points[2], points[9]);
	drv->draw2DLine(points[3], points[10]);
	drv->draw2DLine(points[4], points[11]);
	drv->draw2DLine(points[2], points[12]);
	drv->draw2DLine(points[4], points[13]);
	drv->draw2DLine(points[12] - 1, points[13] - 1);
	drv->draw2DLine(points[0], points[14]);
	drv->draw2DLine(points[5], points[15]);
	drv->draw2DLine(points[1], points[15]);
	drv->draw2DLine(points[10], points[14]);
	drv->draw2DLine(points[10], points[11]);

	drv->setRenderTarget(0);
	IImage* image = drv->createImage(target, vector2di(0, 0), target->getOriginalSize());
	drv->writeImageToFile(image, stringc(rate).append(".png"));
	image->drop();

	/*IMeshWriter* writer = SceneManager->createMeshWriter(EMWT_COLLADA);
	IWriteFile* file = SceneManager->getFileSystem()->createAndWriteFile("Piece.dae");
	writer->writeMesh(file, mesh);
	writer->drop();
	file->drop();*/
}