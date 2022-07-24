#include "Board.h"
#include "Filter.h"
#include "Converter.h"

Board::Board(ISceneNode* parent, ISceneManager* mgr, s32 id, ScaleDefiner* sd) : IObject(parent, mgr, id, sd), boardHeight(0), rectangleHeight(0), rectangleWidth(0), mz(0), mx(0) {
	IVideoDriver* drv = mgr->getVideoDriver();
	IFileSystem* fsy = mgr->getFileSystem();

	f32 boardVerticalHalf = sd->getScale(ESN_BOARD_VERTICAL) / 2.0f;
	f32 boardHorizonHalf = sd->getScale(ESN_BOARD_HORIZON) / 2.0f;
	f32 boardThickness = sd->getScale(ESN_BOARD_THICKNESS);
	f32 boardFootHeight = sd->getScale(ESN_BOARD_FOOT_HEIGHT);
	f32 boardFootMargin = sd->getScale(ESN_BOARD_FOOT_MARGIN);
	boardHeight = boardThickness + boardFootHeight;
	f32 boardPoolHeight = sd->getScale(ESN_BOARD_POOL_HEIGHT);
	f32 boardPoolOutHeightHalf = sd->getScale(ESN_BOARD_POOL_OUT_HEIGHT) / 2.0f;
	f32 boardPoolOutWidthHalf = sd->getScale(ESN_BOARD_POOL_OUT_WIDTH) / 2.0f;
	f32 boardPoolInHeightHalf = sd->getScale(ESN_BOARD_POOL_IN_HEIGHT) / 2.0f;
	f32 boardPoolInWidthHalf = sd->getScale(ESN_BOARD_POOL_IN_WIDTH) / 2.0f;
	f32 standMargin = sd->getScale(ESN_STAND_MARGIN);

	f32 rectangleRate = 10 * 1.0f / sd->getScale(ESN_RECTANGLE_LINE_THICKNESS);
	rectangleHeight = sd->getScale(ESN_RECTANGLE_HEIGHT) * rectangleRate;
	rectangleWidth = sd->getScale(ESN_RECTANGLE_WIDTH) * rectangleRate;

	f32 UVRate = 1.0f / (boardThickness * 2 + boardVerticalHalf * 4) * 2;
	f32 boardVerticalUV = boardVerticalHalf * UVRate;
	f32 boardHorizonUV = boardHorizonHalf * UVRate;
	f32 boardThicknessUV = boardThickness * UVRate / 2.0f;
	f32 boardPoolOutHeightUV = boardPoolOutHeightHalf * UVRate;
	f32 boardPoolOutWidthUV = boardPoolOutWidthHalf * UVRate;
	f32 boardPoolInHeightUV = boardPoolInHeightHalf * UVRate;
	f32 boardPoolInWidthUV = boardPoolInWidthHalf * UVRate;
	f32 rectangleHeightUV = sd->getScale(ESN_RECTANGLE_HEIGHT) * UVRate / 2;
	f32 rectangleWidthUV = sd->getScale(ESN_RECTANGLE_WIDTH) * UVRate / 2;

	SMesh* originalMesh = new SMesh();

	SMaterial material;
	material.Wireframe = false;
	material.Lighting = false;

	SColor color = SColor(255, 255, 255, 255);

	SMeshBuffer* meshBuffer = new SMeshBuffer();

	vector3df Vectors[17];
	{
		Vectors[0] = vector3df(-boardHorizonHalf, boardHeight, boardVerticalHalf);
		Vectors[1] = vector3df(boardHorizonHalf, boardHeight, boardVerticalHalf);
		Vectors[2] = vector3df(-boardHorizonHalf, boardHeight, -boardVerticalHalf);
		Vectors[3] = vector3df(boardHorizonHalf, boardHeight, -boardVerticalHalf);

		Vectors[4] = vector3df(-boardHorizonHalf, boardFootHeight, boardVerticalHalf);
		Vectors[5] = vector3df(boardHorizonHalf, boardFootHeight, boardVerticalHalf);
		Vectors[6] = vector3df(-boardHorizonHalf, boardFootHeight, -boardVerticalHalf);
		Vectors[7] = vector3df(boardHorizonHalf, boardFootHeight, -boardVerticalHalf);

		Vectors[8] = vector3df(-boardPoolOutWidthHalf, boardFootHeight, boardPoolOutHeightHalf);
		Vectors[9] = vector3df(boardPoolOutWidthHalf, boardFootHeight, boardPoolOutHeightHalf);
		Vectors[10] = vector3df(-boardPoolOutWidthHalf, boardFootHeight, -boardPoolOutHeightHalf);
		Vectors[11] = vector3df(boardPoolOutWidthHalf, boardFootHeight, -boardPoolOutHeightHalf);

		Vectors[12] = vector3df(-boardPoolInWidthHalf, boardFootHeight + boardPoolHeight, boardPoolInHeightHalf);
		Vectors[13] = vector3df(boardPoolInWidthHalf, boardFootHeight + boardPoolHeight, boardPoolInHeightHalf);
		Vectors[14] = vector3df(-boardPoolInWidthHalf, boardFootHeight + boardPoolHeight, -boardPoolInHeightHalf);
		Vectors[15] = vector3df(boardPoolInWidthHalf, boardFootHeight + boardPoolHeight, -boardPoolInHeightHalf);

		Vectors[16] = vector3df(0, boardFootHeight, 0);
	}

	vector2df Coordinates[29];
	{
		Coordinates[0] = vector2df(boardThicknessUV, 0);
		Coordinates[1] = vector2df(Coordinates[0].X + boardHorizonUV, 0);

		Coordinates[2] = vector2df(0, boardThicknessUV);
		Coordinates[3] = vector2df(Coordinates[0].X, Coordinates[2].Y);
		Coordinates[4] = vector2df(Coordinates[1].X, Coordinates[2].Y);
		Coordinates[5] = vector2df(Coordinates[1].X + boardThicknessUV, Coordinates[2].Y);

		Coordinates[6] = vector2df(0, Coordinates[2].Y + boardVerticalUV);
		Coordinates[7] = vector2df(Coordinates[3].X, Coordinates[6].Y);
		Coordinates[8] = vector2df(Coordinates[4].X, Coordinates[6].Y);
		Coordinates[9] = vector2df(Coordinates[5].X, Coordinates[6].Y);

		Coordinates[10] = vector2df(Coordinates[0].X, Coordinates[6].Y + boardThicknessUV);
		Coordinates[11] = vector2df(Coordinates[1].X, Coordinates[10].Y);

		Coordinates[12] = vector2df(Coordinates[0].X, 1);
		Coordinates[13] = vector2df(Coordinates[1].X, 1);

		Coordinates[14] = vector2df(Coordinates[0].X + (boardHorizonUV - boardPoolOutWidthUV) / 2.0f, Coordinates[10].Y + (boardVerticalUV - boardPoolOutHeightUV) / 2.0f);
		Coordinates[15] = vector2df(Coordinates[14].X + boardPoolOutWidthUV, Coordinates[14].Y);

		Coordinates[16] = vector2df(Coordinates[14].X, Coordinates[14].Y + boardPoolOutHeightUV);
		Coordinates[17] = vector2df(Coordinates[15].X, Coordinates[16].Y);

		Coordinates[18] = vector2df(1 - boardPoolOutWidthUV, 0);
		Coordinates[19] = vector2df(1, 0);

		Coordinates[20] = vector2df(1 - boardPoolInWidthUV, (boardPoolOutHeightUV - boardPoolInHeightUV) / 2.0f);
		Coordinates[21] = vector2df(1, Coordinates[20].Y);

		Coordinates[22] = vector2df(Coordinates[18].X, boardPoolOutHeightUV / 2.0f);
		Coordinates[23] = vector2df(Coordinates[20].X, Coordinates[22].Y);
		Coordinates[24] = vector2df(1, Coordinates[22].Y);

		Coordinates[27] = vector2df(Coordinates[18].X, boardPoolOutHeightUV);
		Coordinates[28] = vector2df(1, Coordinates[27].Y);

		Coordinates[25] = vector2df(Coordinates[20].X, Coordinates[27].Y - Coordinates[20].Y);
		Coordinates[26] = vector2df(1, Coordinates[25].Y);
	}

	meshBuffer->Vertices.set_used(29);
	{
		meshBuffer->Vertices[0] = S3DVertex(Vectors[0], vector3df(0, 1, 0), color, Coordinates[3]);
		meshBuffer->Vertices[1] = S3DVertex(Vectors[1], vector3df(0, 1, 0), color, Coordinates[4]);
		meshBuffer->Vertices[2] = S3DVertex(Vectors[2], vector3df(0, 1, 0), color, Coordinates[7]);
		meshBuffer->Vertices[3] = S3DVertex(Vectors[3], vector3df(0, 1, 0), color, Coordinates[8]);
		meshBuffer->Vertices[4] = S3DVertex(Vectors[4], vector3df(0, 1, 0), color, Coordinates[0]);
		meshBuffer->Vertices[5] = S3DVertex(Vectors[4], vector3df(0, 1, 0), color, Coordinates[2]);
		meshBuffer->Vertices[6] = S3DVertex(Vectors[4], vector3df(0, 1, 0), color, Coordinates[12]);
		meshBuffer->Vertices[7] = S3DVertex(Vectors[5], vector3df(0, 1, 0), color, Coordinates[1]);
		meshBuffer->Vertices[8] = S3DVertex(Vectors[5], vector3df(0, 1, 0), color, Coordinates[5]);
		meshBuffer->Vertices[9] = S3DVertex(Vectors[5], vector3df(0, 1, 0), color, Coordinates[13]);
		meshBuffer->Vertices[10] = S3DVertex(Vectors[6], vector3df(0, -1, 0), color, Coordinates[6]);
		meshBuffer->Vertices[11] = S3DVertex(Vectors[6], vector3df(0, -1, 0), color, Coordinates[10]);
		meshBuffer->Vertices[12] = S3DVertex(Vectors[7], vector3df(0, -1, 0), color, Coordinates[9]);
		meshBuffer->Vertices[13] = S3DVertex(Vectors[7], vector3df(0, -1, 0), color, Coordinates[11]);
		meshBuffer->Vertices[14] = S3DVertex(Vectors[8], vector3df(0, -1, 0), color, Coordinates[16]);
		meshBuffer->Vertices[15] = S3DVertex(Vectors[8], vector3df(0, -1, 0), color, Coordinates[22]);
		meshBuffer->Vertices[16] = S3DVertex(Vectors[9], vector3df(0, -1, 0), color, Coordinates[17]);
		meshBuffer->Vertices[17] = S3DVertex(Vectors[9], vector3df(0, -1, 0), color, Coordinates[27]);
		meshBuffer->Vertices[18] = S3DVertex(Vectors[10], vector3df(0, -1, 0), color, Coordinates[18]);
		meshBuffer->Vertices[19] = S3DVertex(Vectors[10], vector3df(0, -1, 0), color, Coordinates[14]);
		meshBuffer->Vertices[20] = S3DVertex(Vectors[11], vector3df(0, -1, 0), color, Coordinates[15]);
		meshBuffer->Vertices[21] = S3DVertex(Vectors[11], vector3df(0, -1, 0), color, Coordinates[19]);
		meshBuffer->Vertices[22] = S3DVertex(Vectors[11], vector3df(0, -1, 0), color, Coordinates[28]);
		meshBuffer->Vertices[23] = S3DVertex(Vectors[12], vector3df(0, -1, 0), color, Coordinates[23]);
		meshBuffer->Vertices[24] = S3DVertex(Vectors[13], vector3df(0, -1, 0), color, Coordinates[25]);
		meshBuffer->Vertices[25] = S3DVertex(Vectors[14], vector3df(0, -1, 0), color, Coordinates[20]);
		meshBuffer->Vertices[26] = S3DVertex(Vectors[15], vector3df(0, -1, 0), color, Coordinates[21]);
		meshBuffer->Vertices[27] = S3DVertex(Vectors[15], vector3df(0, -1, 0), color, Coordinates[26]);
		meshBuffer->Vertices[28] = S3DVertex(Vectors[16], vector3df(0, -1, 0), color, Coordinates[24]);
	}

	u16 indices[] = { 4,7,0, 7,1,0, 5,0,2, 5,2,10, 0,1,3, 0,3,2, 1,8,12, 1,12,3, 2,3,11, 3,13,11, 11,13,19, 13,20,19, 13,9,20, 9,16,20, 11,14,6, 11,19,14, 14,16,6, 16,9,6, 18,21,25, 21,26,25, 18,25,15, 25,23,15, 25,26,28, 25,28,23, 15,24,17, 15,23,24, 23,28,24, 28,27,24, 24,22,17, 24,27,22 };
	meshBuffer->Indices.set_pointer(indices, 90);

	meshBuffer->Material = material;
	ITexture* texture = drv->getTexture("resources/images/board.png");
	if (texture) {
		dimension2du targetSize(f32_to_s32(rectangleWidth * 9) + 10, f32_to_s32(rectangleHeight * 9) + 10);
		ITexture* target = drv->addRenderTargetTexture(targetSize, "rt", ECF_A1R5G5B5);
		if (target) {
			drv->setRenderTarget(target);

			f32 v = 0;
			for (u32 i = 0; i < 10; i++) {
				drv->draw2DLine(vector2di(0, f32_to_s32(v)), vector2di(targetSize.Width - 1, f32_to_s32(v)), SColor(255, 0, 0, 0));
				v += rectangleHeight + 1;
			}

			f32 u = 0;
			for (u32 i = 0; i < 10; i++) {
				drv->draw2DLine(vector2di(f32_to_s32(u), 0), vector2di(f32_to_s32(u), targetSize.Height - 1), SColor(255, 0, 0, 0));
				u += rectangleWidth + 1;
			}

			drv->endScene();
			drv->setRenderTarget(0);

			dimension2du textureSize = texture->getOriginalSize();
			target = scaling(drv, target, dimension2du(f32_to_s32(textureSize.Width * rectangleWidthUV * 9), f32_to_s32(textureSize.Height * rectangleHeightUV * 9)));
			texture = blend(drv, texture, target, vector2di(f32_to_s32(textureSize.Width * (boardThicknessUV + boardHorizonUV / 2.0f - rectangleWidthUV / 2.0f * 9)), f32_to_s32(textureSize.Height * (boardThicknessUV + boardVerticalUV / 2.0f - rectangleHeightUV / 2.0f * 9))));

		}

		meshBuffer->Material.setTexture(0, texture);
	}

	originalMesh->addMeshBuffer(meshBuffer);
	convertMesh(originalMesh);

	IMesh* foot = mgr->getMesh("resources/models/foot.obj");
	if (foot) {
		IMeshBuffer* source = foot->getMeshBuffer(0);
		u32 vertexCount = source->getVertexCount();
		u32 indexCount = source->getIndexCount();

		f32 px = boardHorizonHalf - boardFootMargin;
		f32 nx = boardFootMargin - boardHorizonHalf;
		f32 pz = boardVerticalHalf - boardFootMargin;
		f32 nz = boardFootMargin - boardVerticalHalf;

		ITexture* texture = drv->getTexture("resources/images/foot.png");

		for (s32 i = 0; i < 4; i++) {
			SMeshBuffer* aFoot = new SMeshBuffer();
			aFoot->Vertices.set_used(vertexCount);
			for (u32 j = 0; j < vertexCount; j++) {
				aFoot->Vertices[j] = ((S3DVertex*)(source->getVertices()))[j];
			}

			aFoot->Indices.set_pointer(source->getIndices(), source->getIndexCount());

			vector3df addition(i % 2 ? nx : px, 0, i / 2 ? nz : pz);

			for (u32 j = 0; j < vertexCount; j++) {
				aFoot->Vertices[j].Pos += addition;
			}

			if (texture) {
				aFoot->Material = material;
				aFoot->Material.setTexture(0, texture);
			}

			originalMesh->addMeshBuffer(aFoot);
		}
	}

	IMesh* stand = mgr->getMesh("resources/models/stand.obj");
	if (stand) {
		IMeshBuffer* source = stand->getMeshBuffer(0);
		u32 vertexCount = source->getVertexCount();
		u32 indexCount = source->getIndexCount();

		ITexture* texture = drv->getTexture("resources/images/stand.png");

		for (s32 i = 0; i < 2; i++) {
			SMeshBuffer* aStand = new SMeshBuffer();
			aStand->Vertices.set_used(vertexCount);
			for (u32 j = 0; j < vertexCount; j++) {
				aStand->Vertices[j] = ((S3DVertex*)(source->getVertices()))[j];
			}

			aStand->Indices.set_pointer(source->getIndices(), source->getIndexCount());

			vector3df addition(boardHorizonHalf + standMargin, 0, standMargin - boardVerticalHalf);
			if (i == 0) {
				addition = -addition;
			}

			for (u32 j = 0; j < vertexCount; j++) {
				aStand->Vertices[j].Pos += addition;
			}

			if (texture) {
				aStand->Material = material;
				aStand->Material.setTexture(0, texture);
			}

			originalMesh->addMeshBuffer(aStand);
		}
	}

	mesh = SceneManager->getMeshManipulator()->createMeshWithTangents(originalMesh);
	originalMesh->drop();

	aabbox3df boundingBox;
	boundingBox.reset(vector3df());
	for (s32 i = 0; i < 17; ++i) {
		boundingBox.addInternalPoint(Vectors[i]);
	}
	mesh->setBoundingBox(boundingBox);
}

Board::~Board() {
	IObject::~IObject();
}

void Board::render() {
	IObject::render();

	IVideoDriver* drv = SceneManager->getVideoDriver();

	drv->setTransform(ETS_WORLD, AbsoluteTransformation);
	drv->setMaterial(lineMaterial);

	
}