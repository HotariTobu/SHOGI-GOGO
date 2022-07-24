#include "Filter.h"

namespace irr {
	namespace utils {
		IMeshBuffer* getQuad(ITexture* texture) {
			SMeshBuffer* meshBuffer = new SMeshBuffer();

			meshBuffer->Material.Lighting = false;
			meshBuffer->Material.setTexture(0, texture);

			SColor color = SColor(255, 255, 255, 255);

			meshBuffer->Vertices.set_used(4);
			meshBuffer->Vertices[0] = S3DVertex(vector3df(-1.0f, 1.0f, 0), vector3df(), color, vector2df(0, 0));
			meshBuffer->Vertices[1] = S3DVertex(vector3df(1.0f, 1.0f, 0), vector3df(), color, vector2df(1, 0));
			meshBuffer->Vertices[2] = S3DVertex(vector3df(1.0f, -1.0f, 0), vector3df(), color, vector2df(1, 1));
			meshBuffer->Vertices[3] = S3DVertex(vector3df(-1.0f, -1.0f, 0), vector3df(), color, vector2df(0, 1));

			meshBuffer->Indices.set_used(6);
			u16 indices[] = { 0,1,2, 0,2,3 };
			for (s32 i = 0; i < 6; i++) {
				meshBuffer->Indices[i] = indices[i];
			}

			return meshBuffer;
		}

		ITexture* filter(IVideoDriver* drv, ITexture* texture, s32 materialType) {
			IMeshBuffer* meshBuffer = getQuad(texture);
			meshBuffer->getMaterial().MaterialType = (E_MATERIAL_TYPE)materialType;

			ITexture* result = drv->addRenderTargetTexture(texture->getOriginalSize());
			if (result) {
				drv->setRenderTarget(result);

				drv->setMaterial(meshBuffer->getMaterial());

				matrix4 oldProjMat = drv->getTransform(ETS_PROJECTION);
				matrix4 oldViewMat = drv->getTransform(ETS_VIEW);
				drv->setTransform(ETS_PROJECTION, matrix4());
				drv->setTransform(ETS_VIEW, matrix4());

				drv->drawMeshBuffer(meshBuffer);

				drv->setRenderTarget(0);

				drv->setTransform(ETS_PROJECTION, oldProjMat);
				drv->setTransform(ETS_VIEW, oldViewMat);

				return result;
			}
			else {
				return texture;
			}
		}

		ITexture* scaling(IVideoDriver* drv, ITexture* texture, dimension2du size) {
			IMeshBuffer* meshBuffer = getQuad(texture);
			meshBuffer->getMaterial().MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;

			ITexture* result = drv->addRenderTargetTexture(size);
			if (result) {
				drv->setRenderTarget(result);

				drv->setMaterial(meshBuffer->getMaterial());

				matrix4 oldProjMat = drv->getTransform(ETS_PROJECTION);
				matrix4 oldViewMat = drv->getTransform(ETS_VIEW);
				drv->setTransform(ETS_PROJECTION, matrix4());
				drv->setTransform(ETS_VIEW, matrix4());

				drv->drawMeshBuffer(meshBuffer);

				drv->setRenderTarget(0);

				drv->setTransform(ETS_PROJECTION, oldProjMat);
				drv->setTransform(ETS_VIEW, oldViewMat);

				return result;
			}
			else {
				return texture;
			}
		}

		ITexture* blend(IVideoDriver* drv, ITexture* foundation, ITexture* ingredient, vector2di pos) {
			ITexture* result = drv->addRenderTargetTexture(foundation->getOriginalSize());
			if (result) {
				drv->setRenderTarget(result);

				drv->draw2DImage(foundation, vector2di(), recti(vector2di(0, 0), foundation->getOriginalSize()), 0, SColor(255, 255, 255, 255), true);
				drv->draw2DImage(ingredient, pos, recti(vector2di(0, 0), ingredient->getOriginalSize()), 0, SColor(255, 255, 255, 255), true);
				drv->endScene();

				drv->setRenderTarget(0);

				return result;
			}
			else {
				return foundation;
			}
		}

		ITexture* singleColor(IVideoDriver* drv, SColor color, dimension2du size) {
			ITexture* result = drv->addRenderTargetTexture(size);
			if (result) {
				drv->setRenderTarget(result, true, true, color);
				drv->setRenderTarget(0);

				return result;
			}
			else {
				return 0;
			}
		}
	}
}