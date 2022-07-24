#pragma once

#ifndef __SHADER_H_INCLUDED__
#define __SHADER_H_INCLUDED__

#include <IrrlichtDevice.h>
#include <ISceneManager.h>
#include <ICameraSceneNode.h>
#include <IMaterialRendererServices.h>
#include <IShaderConstantSetCallBack.h>
#include <IGPUProgrammingServices.h>

namespace irr {
	namespace video {
		class CShaderConstantSetCallBack : public IShaderConstantSetCallBack {
		public:
			CShaderConstantSetCallBack(IrrlichtDevice* device) : device(device) {}

			virtual void OnSetConstants(IMaterialRendererServices* services, s32 userData) override {
				IVideoDriver* drv = services->getVideoDriver();

				/*services->setVertexShaderConstant("projectionMat", drv->getTransform(ETS_PROJECTION).pointer(), 16);
				services->setVertexShaderConstant("viewMat", drv->getTransform(ETS_VIEW).pointer(), 16);
				services->setVertexShaderConstant("worldMat", drv->getTransform(ETS_WORLD).pointer(), 16);*/
				s32 timeMs = device->getTimer()->getTime();
				bool b = services->setVertexShaderConstant("timeMs", &timeMs, 1);
				matrix4 pvw = drv->getTransform(ETS_PROJECTION);
				pvw *= drv->getTransform(ETS_VIEW);
				pvw *= drv->getTransform(ETS_WORLD);
				services->setVertexShaderConstant("pvw", pvw.pointer(), 16);

				ICameraSceneNode* camera = device->getSceneManager()->getActiveCamera();
				if (camera) {
					services->setVertexShaderConstant("cameraPos", reinterpret_cast<f32*>(&(camera->getAbsolutePosition())), 3);
				}
				
				s32 textureLayer = 0;
				services->setPixelShaderConstant("texture", &textureLayer, 1);
			}

		private:
			IrrlichtDevice* device;
		};

		bool hardwareCheck(IVideoDriver* drv, ILogger* logger) {
			if (!drv || !logger) {
				return false;
			}

			bool result = true;

			if (!drv->queryFeature(video::EVDF_PIXEL_SHADER_1_1) && !drv->queryFeature(video::EVDF_ARB_FRAGMENT_PROGRAM_1)) {
				logger->log("WARNING: Pixel shaders disabled because of missing driver/hardware support.");
				result = false;
			}

			if (!drv->queryFeature(video::EVDF_VERTEX_SHADER_1_1) && !drv->queryFeature(video::EVDF_ARB_VERTEX_PROGRAM_1)) {
				logger->log("WARNING: Vertex shaders disabled because of missing driver/hardware support.");
				result = false;
			}

			return result;
		}

		s32 addNewMaterialType(IrrlichtDevice* device, const c8* vs, const c8* ps, E_MATERIAL_TYPE base = EMT_SOLID) {
			IGPUProgrammingServices* gpu = device->getVideoDriver()->getGPUProgrammingServices();
			s32 newMaterialType = 0;
			
			if (gpu) {
				CShaderConstantSetCallBack* callback = new CShaderConstantSetCallBack(device);
				newMaterialType = gpu->addHighLevelShaderMaterial(vs, "vertexMain", EVST_VS_1_1, ps, "pixelMain", EPST_PS_1_1, callback, base, 0, EGSL_DEFAULT);
				callback->drop();
			}
			else {
				newMaterialType = -1;
			}

			return newMaterialType;
		}

		const c8* VS_Basic = R"(
uniform mat4 pvw
uniform vec3 cameraPos;

void main() {
	gl_Position = pvw * gl_Vertex;
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
}
)";

		const c8* PS_Basic = R"(
uniform sampler2D texture;

void main () {
	gl_FragColor = texture2D(myTexture, vec2(gl_TexCoord[0]));
}
)";

		const c8* PS_Grayscale = R"(
uniform sampler2D texture;

void main () {
	gl_FragColor = vec4(vec3(dot(texture2D(texture, vec2(gl_TexCoord[0])).rgb, vec3(0.298912, 0.586611, 0.114478))), 1.0);
}
)";
	}
}

#endif // !__SHADER_H_INCLUDED__
