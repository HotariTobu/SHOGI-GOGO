#include "Main.h"

#ifdef _DEBUG
CGUIDebugBoard* debugBoard;
#endif // _DEBUG

//s32 grayscaleMaterialType;

int main() {
	IrrlichtDevice* device = getDevice();
	if (device == 0) {
		return 1;
	}
	
	MainEventReceiver* queue = new MainEventReceiver(device);

	device->setWindowCaption(L"«ŠûGOGO");
	device->setEventReceiver((IEventReceiver*)queue);

	IVideoDriver* drv = device->getVideoDriver();
	ISceneManager* mgr = device->getSceneManager();
	IGUIEnvironment* env = device->getGUIEnvironment();
	IFileSystem* fsy = device->getFileSystem();
	ITimer* tim = device->getTimer();

	video::ITexture* logo = drv->getTexture("resources/images/logo.png");
	if (logo) {
		dimension2du screenSize = drv->getScreenSize();
		bool slim = screenSize.Width < screenSize.Height;
		u32 length = slim ? screenSize.Width : screenSize.Height;
		logo = scaling(drv, logo, dimension2du(length, length));

		ITexture* back = singleColor(drv, SColor(255, 255, 255, 255), screenSize);
		if (back) {
			if (slim) {
				logo = blend(drv, back, logo, vector2di(0, (screenSize.Height - length) / 2));
			}
			else {
				logo = blend(drv, back, logo, vector2di((screenSize.Width - length) / 2, 0));
			}
		}

		drv->beginScene();
		drv->draw2DImage(logo, vector2di());
		drv->endScene();
	}

	ScaleDefiner* sd = new ScaleDefiner(device);

	EffectHandler* effect;
	if (hardwareCheck(drv,device->getLogger())) {
		effect = getEffectHandler(device);
	}
	else {
		effect = new EffectHandler(device, drv->getScreenSize());
	}

	/////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
	debugBoard = new CGUIDebugBoard(env, env->getRootGUIElement(), -1, recti(0, 0, 640, 480));
	debugBoard->setVisible(false);
	IGUIFont* font = env->getFont("../../Debug_Font/Debug_Font.xml");
	if (font) {
		debugBoard->setFont(font);
		debugBoard->setColor(SColor(128, 255, 255, 255));
	}
#endif // _DEBUG
	/////////////////////////////////////////////////////////////////////

	effect->addShadowLight(SShadowLight(shadowMapDimension, vector3df(10,10,10), vector3df(), SColor(0, 128, 128, 160), 1, 300, 90 * DEGTORAD));
	effect->addShadowLight(SShadowLight(shadowMapDimension, vector3df(0,100,0), vector3df(), SColor(0, 128, 128, 160), 1, 110, 90 * DEGTORAD));

	Pieces pieces = Pieces(device, mgr->getRootSceneNode(), sd);
	f32 pieceHeight = sd->getScale(ESN_PIECE_HEIGHT);
	f32 boardHeight = sd->getScale(ESN_BOARD_FOOT_HEIGHT) + sd->getScale(ESN_BOARD_THICKNESS);
	for (s32 i = 0; i < 8; i++) {
		pieces.getPiece3d((E_PIECE_NAME)i)->setPosition(vector3df((f32)(pieceHeight * i), boardHeight, 0));
		effect->addShadowToNode(pieces.getPiece3d((E_PIECE_NAME)i), (E_FILTER_TYPE)XEffectFilterType);
	}

	Board* board = new Board(mgr->getRootSceneNode(), mgr, 8, sd);
	effect->addShadowToNode(board, (E_FILTER_TYPE)XEffectFilterType);

	Tatami* tatamis[8];
	f32 tatamiHeight = sd->getScale(ESN_TATAMI_HEIGHT);
	tatamis[0] = new Tatami(mgr->getRootSceneNode(), mgr, -1, sd, false);
	ITexture* texture = drv->getTexture("resources/images/tatami.png");
	if (texture) {
		tatamis[0]->setMaterialTexture(0, texture);
	}
	tatamis[0]->setPosition(vector3df(-tatamiHeight * 2, 0, tatamiHeight * 2));
	tatamis[1] = tatamis[0]->clone(); tatamis[1]->setPosition(vector3df(0, 0, tatamiHeight * 2));
	tatamis[2] = tatamis[0]->clone(); tatamis[2]->setPosition(vector3df(-tatamiHeight, 0, tatamiHeight)); tatamis[2]->setRotation(vector3df(0, 90, 0));
	tatamis[3] = tatamis[0]->clone(); tatamis[3]->setPosition(vector3df(-tatamiHeight, 0, tatamiHeight));
	tatamis[4] = tatamis[0]->clone(); tatamis[4]->setPosition(vector3df(-tatamiHeight, 0, 0));
	tatamis[5] = tatamis[0]->clone(); tatamis[5]->setPosition(vector3df(tatamiHeight * 2, 0, tatamiHeight)); tatamis[5]->setRotation(vector3df(0, 90, 0));
	tatamis[6] = tatamis[0]->clone(); tatamis[6]->setPosition(vector3df(-tatamiHeight * 2, 0, -tatamiHeight));
	tatamis[7] = tatamis[0]->clone(); tatamis[7]->setPosition(vector3df(0, 0, -tatamiHeight));
	for (s32 i = 0; i < 8; i++) {
		effect->addShadowToNode(tatamis[i], (E_FILTER_TYPE)XEffectFilterType);
	}

	/////////////////////////////////////////////////////////////////////
	ISceneNode* cube = mgr->addCubeSceneNode(1.0f);
	cube->addAnimator((ISceneNodeAnimator*)mgr->createFlyCircleAnimator(vector3df(0, 100.0f, 0), 50.0f, 0.0005f));
	SMeshBuffer* smeshbuffer = new SMeshBuffer();
	{
		//0
		smeshbuffer->Vertices.push_back(S3DVertex(vector3df(1, 1, 1), vector3df(1, 0, 0), SColor(255, 255, 255, 255), vector2df()));
		smeshbuffer->Vertices.push_back(S3DVertex(vector3df(1, 1, 1), vector3df(0, 1, 0), SColor(255, 255, 255, 255), vector2df()));
		smeshbuffer->Vertices.push_back(S3DVertex(vector3df(1, 1, 1), vector3df(0, 0, 1), SColor(255, 255, 255, 255), vector2df()));

		//1
		smeshbuffer->Vertices.push_back(S3DVertex(vector3df(-1, 1, 1), vector3df(-1, 0, 0), SColor(255, 255, 255, 255), vector2df()));
		smeshbuffer->Vertices.push_back(S3DVertex(vector3df(-1, 1, 1), vector3df(0, 1, 0), SColor(255, 255, 255, 255), vector2df()));
		smeshbuffer->Vertices.push_back(S3DVertex(vector3df(-1, 1, 1), vector3df(0, 0, 1), SColor(255, 255, 255, 255), vector2df()));

		//2
		smeshbuffer->Vertices.push_back(S3DVertex(vector3df(1, -1, 1), vector3df(1, 0, 0), SColor(255, 255, 255, 255), vector2df()));
		smeshbuffer->Vertices.push_back(S3DVertex(vector3df(1, -1, 1), vector3df(0, -1, 0), SColor(255, 255, 255, 255), vector2df()));
		smeshbuffer->Vertices.push_back(S3DVertex(vector3df(1, -1, 1), vector3df(0, 0, 1), SColor(255, 255, 255, 255), vector2df()));

		//3
		smeshbuffer->Vertices.push_back(S3DVertex(vector3df(1, 1, -1), vector3df(1, 0, 0), SColor(255, 255, 255, 255), vector2df()));
		smeshbuffer->Vertices.push_back(S3DVertex(vector3df(1, 1, -1), vector3df(0, 1, 0), SColor(255, 255, 255, 255), vector2df()));
		smeshbuffer->Vertices.push_back(S3DVertex(vector3df(1, 1, -1), vector3df(0, 0, -1), SColor(255, 255, 255, 255), vector2df()));

		//4
		smeshbuffer->Vertices.push_back(S3DVertex(vector3df(-1, -1, 1), vector3df(-1, 0, 0), SColor(255, 255, 255, 255), vector2df()));
		smeshbuffer->Vertices.push_back(S3DVertex(vector3df(-1, -1, 1), vector3df(0, -1, 0), SColor(255, 255, 255, 255), vector2df()));
		smeshbuffer->Vertices.push_back(S3DVertex(vector3df(-1, -1, 1), vector3df(0, 0, 1), SColor(255, 255, 255, 255), vector2df()));

		//5
		smeshbuffer->Vertices.push_back(S3DVertex(vector3df(1, -1, -1), vector3df(1, 0, 0), SColor(255, 255, 255, 255), vector2df()));
		smeshbuffer->Vertices.push_back(S3DVertex(vector3df(1, -1, -1), vector3df(0, -1, 0), SColor(255, 255, 255, 255), vector2df()));
		smeshbuffer->Vertices.push_back(S3DVertex(vector3df(1, -1, -1), vector3df(0, 0, -1), SColor(255, 255, 255, 255), vector2df()));

		//6
		smeshbuffer->Vertices.push_back(S3DVertex(vector3df(-1, 1, -1), vector3df(-1, 0, 0), SColor(255, 255, 255, 255), vector2df()));
		smeshbuffer->Vertices.push_back(S3DVertex(vector3df(-1, 1, -1), vector3df(0, 1, 0), SColor(255, 255, 255, 255), vector2df()));
		smeshbuffer->Vertices.push_back(S3DVertex(vector3df(-1, 1, -1), vector3df(0, 0, -1), SColor(255, 255, 255, 255), vector2df()));

		//7
		smeshbuffer->Vertices.push_back(S3DVertex(vector3df(-1, -1, -1), vector3df(-1, 0, 0), SColor(255, 255, 255, 255), vector2df()));
		smeshbuffer->Vertices.push_back(S3DVertex(vector3df(-1, -1, -1), vector3df(0, -1, 0), SColor(255, 255, 255, 255), vector2df()));
		smeshbuffer->Vertices.push_back(S3DVertex(vector3df(-1, -1, -1), vector3df(0, 0, -1), SColor(255, 255, 255, 255), vector2df()));
	}
	{
		smeshbuffer->Indices.push_back(0 * 3 + 0);
		smeshbuffer->Indices.push_back(2 * 3 + 0);
		smeshbuffer->Indices.push_back(3 * 3 + 0);
		smeshbuffer->Indices.push_back(5 * 3 + 0);
		smeshbuffer->Indices.push_back(3 * 3 + 0);
		smeshbuffer->Indices.push_back(2 * 3 + 0);

		smeshbuffer->Indices.push_back(6 * 3 + 0);
		smeshbuffer->Indices.push_back(7 * 3 + 0);
		smeshbuffer->Indices.push_back(1 * 3 + 0);
		smeshbuffer->Indices.push_back(4 * 3 + 0);
		smeshbuffer->Indices.push_back(1 * 3 + 0);
		smeshbuffer->Indices.push_back(7 * 3 + 0);

		smeshbuffer->Indices.push_back(0 * 3 + 1);
		smeshbuffer->Indices.push_back(3 * 3 + 1);
		smeshbuffer->Indices.push_back(1 * 3 + 1);
		smeshbuffer->Indices.push_back(6 * 3 + 1);
		smeshbuffer->Indices.push_back(1 * 3 + 1);
		smeshbuffer->Indices.push_back(3 * 3 + 1);

		smeshbuffer->Indices.push_back(7 * 3 + 1);
		smeshbuffer->Indices.push_back(5 * 3 + 1);
		smeshbuffer->Indices.push_back(4 * 3 + 1);
		smeshbuffer->Indices.push_back(2 * 3 + 1);
		smeshbuffer->Indices.push_back(4 * 3 + 1);
		smeshbuffer->Indices.push_back(5 * 3 + 1);

		smeshbuffer->Indices.push_back(0 * 3 + 2);
		smeshbuffer->Indices.push_back(1 * 3 + 2);
		smeshbuffer->Indices.push_back(2 * 3 + 2);
		smeshbuffer->Indices.push_back(4 * 3 + 2);
		smeshbuffer->Indices.push_back(2 * 3 + 2);
		smeshbuffer->Indices.push_back(1 * 3 + 2);

		smeshbuffer->Indices.push_back(6 * 3 + 2);
		smeshbuffer->Indices.push_back(3 * 3 + 2);
		smeshbuffer->Indices.push_back(7 * 3 + 2);
		smeshbuffer->Indices.push_back(5 * 3 + 2);
		smeshbuffer->Indices.push_back(7 * 3 + 2);
		smeshbuffer->Indices.push_back(3 * 3 + 2);
	}
	smeshbuffer->Material.Lighting = false;
	SMesh* smesh = new SMesh();
	smesh->addMeshBuffer(smeshbuffer);
	//convertMesh(smesh);
	IMeshSceneNode* cube24 = mgr->addMeshSceneNode(smesh, mgr->getRootSceneNode(), -1, vector3df(5, 12, 5));
	effect->addShadowToNode(cube24, (E_FILTER_TYPE)XEffectFilterType);

	IMeshSceneNode* cube0 = mgr->addCubeSceneNode(2, mgr->getRootSceneNode(), -1, vector3df(-5, 12, -5));
	cube0->setMaterialFlag(EMF_LIGHTING, false);
	effect->addShadowToNode(cube0, (E_FILTER_TYPE)XEffectFilterType);
	/////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////
	CoordinateSystemCursor* node = new CoordinateSystemCursor(mgr->getRootSceneNode(), mgr, 777);
	/////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////
	vector3df* lookat = (vector3df*)&(node->getPosition());
	ICameraSceneNode* camera = mgr->addCameraSceneNode(0, vector3df(0, 0, -sd->getScale(ESN_CAMERA_RADIUS)), *lookat);
	f32 far = camera->getFarValue();

	ISceneNodeAnimator* cameraAnimator = new CSceneNodeAnimatorCameraTPS(lookat);
	if (cameraAnimator) {
		camera->addAnimator(cameraAnimator);
	}
	/////////////////////////////////////////////////////////////////////

	u32 count = 0, pn = 0;

	s32 lastFPS = -1;
	u32 then = tim->getTime();

	bool active = true;
	while (device->run()) {
		u32 now = tim->getTime();
		u32 time = now - then;
		f32 frameDeltaTime = (f32)time / 1000.0f;
		then = now;

		if (now > 5000) {
			count += time;
			if (count > 100) {
				count = 0;

				pieces.getPiece3d((E_PIECE_NAME)pn)->addAnimator((ISceneNodeAnimator*)new AnimatorPromotion(500));
				pn++;
				if (pn == 8) {
					pn = 0;
				}
			}
		}

		effect->getShadowLight(0).setPosition(cube->getPosition());
		//effect->getShadowLight(0).setTarget(cube->getPosition() + vector3df(0, -1, 0));
		//effect->getShadowLight(0).setTarget(vector3df(5, 12, 5));
		effect->getShadowLight(0).setTarget(vector3df(0, 5, 0));

		if (device->isWindowActive()) {
			if (!active) {
				tim->start();
				active = true;
			}

			drv->beginScene(true, true, SColor(0, 100, 100, 100));

			mgr->drawAll();
			//effect->update();

			if (queue->isGUIVisible) {
				env->drawAll();
			}
#ifdef _DEBUG
			else { debugBoard->draw(); }
#endif // _DEBUG

			drv->endScene();
		}
		else {
			if (active) {
				tim->stop();
				active = false;
			}

			device->yield();
		}

		s32 fps = drv->getFPS();
		if (lastFPS != fps) {
			lastFPS = fps;
		}

#ifdef _DEBUG
		debugBoard->appendLine(stringw("fps:") + stringw(fps));
		debugBoard->appendLine(stringw("now:") + stringw(now));
#endif // _DEBUG
	}

	device->drop();

	return 0;
}

void convertMesh(IMesh* mesh) {
	u32 meshCount = mesh->getMeshBufferCount();
	for (u32 i = 0; i < meshCount; i++) {
		IMeshBuffer* meshBuffer = mesh->getMeshBuffer(i);

		array<vector3df> vectors;
		array<array<u32>> fnGroups;
		u32 vCount = meshBuffer->getVertexCount();
		for (u32 v = 0; v < vCount; v++) {
			vector3df vec = meshBuffer->getPosition(v);
			if (vectors.linear_search(vec) == -1) {
				vectors.push_back(vec);
				fnGroups.push_back(array<u32>());
			}
		}

		array<array<u32>> indices;
		u32 fCount = meshBuffer->getIndexCount() / 3;
		for (u32 f = 0; f < fCount; f++) {
			u32 index = f * 3;
			array<u32> p;
			p.push_back(vectors.linear_search(meshBuffer->getPosition(meshBuffer->getIndices()[index])));
			p.push_back(vectors.linear_search(meshBuffer->getPosition(meshBuffer->getIndices()[index + 1])));
			p.push_back(vectors.linear_search(meshBuffer->getPosition(meshBuffer->getIndices()[index + 2])));

			bool match = false;
			u32 indexCount = indices.size();
			for (u32 j = 0; j < indexCount; j++) {
				for (u32 l = 0; l < 3; l++) {
					if (indices[j] == p) {
						match = true;
						break;
					}

					p.push_back(p[0]);
					p.erase(0);
				}
			}

			if (!match) {
				indices.push_back(p);
			}
		}
		fCount = indices.size();

		array<vector3df> fns;
		fns.set_used(fCount);
		for (u32 f = 0; f < fCount; f++) {
			vector3df v0(vectors[indices[f][1]] - vectors[indices[f][0]]);
			vector3df v1(vectors[indices[f][2]] - vectors[indices[f][0]]);
			fns[f] = v0.crossProduct(v1).normalize();
		}

		for (u32 f = 0; f < fCount; f++) {
			for (u32 j = 0; j < 3; j++) {
				u32 index = indices[f][j];
				if (fnGroups[index].linear_search(f) == -1) {
					fnGroups[index].push_back(f);
				}
			}
		}

		array<vector3df> vns;
		u32 vnCount = fnGroups.size();
		for (u32 j = 0; j < vnCount; j++) {
			vector3df vn;

			array<vector3df> ns;
			array<u32> group = fnGroups[j];
			u32 nCount = group.size();
			for (u32 l = 0; l < nCount; l++) {
				vector3df n = fns[group[l]];
				if (ns.linear_search(n) == -1) {
					ns.push_back(n);
				}
			}

			u32 fnCount = ns.size();
			for (u32 l = 0; l < fnCount; l++) {
				vn += ns[l];
			}

			vns.push_back(vn.normalize());
		}

		for (u32 v = 0; v < vCount; v++) {
			vector3df vn = vns[vectors.linear_search(meshBuffer->getPosition(v))];
			switch (meshBuffer->getVertexType())
			{
			case EVT_STANDARD:
				((S3DVertex*)meshBuffer->getVertices())[v].Normal = vn;
				break;

			case EVT_2TCOORDS:
				((S3DVertex2TCoords*)meshBuffer->getVertices())[v].Normal = vn;
				break;

			case EVT_TANGENTS:
				((S3DVertexTangents*)meshBuffer->getVertices())[v].Normal = vn;
				break;
			}
		}
	}
}