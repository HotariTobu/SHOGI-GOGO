#pragma once

#ifndef __XEFFECT_INITRAIZER_H_INCLUDED__
#define __XEFFECT_INITRAIZER_H_INCLUDED__

#include "MyHead.h"
#include "XEffects.h"
#include "Converter.h"

u32 shadowMapDimension;
u32 XEffectFilterType;

inline EffectHandler* getEffectHandler(IrrlichtDevice* device) {
	shadowMapDimension = 512;
	u32 shadowMapDimensionFactor = 2;
	XEffectFilterType = 2;
	bool useVSMShadows = false;
	bool useRoundSpotLights = true;
	bool use32BitDepthBuffers = false;

	ILogger* logger = device->getLogger();
	IReadFile* file = device->getFileSystem()->createAndOpenFile("quality.txt");
	if (file) {
		logger->log(R"(Succeed to open "quality.txt")");

		u32 size = file->getSize();
		c8* buffer = new c8[size];
		size = file->read(buffer, size);
		stringc str(buffer);
		delete[] buffer;

		if (str.lastChar() != '\n') {
			str.append('\n');
		}

		array<stringc> lines;
		s32 lineCount = str.split(lines, "\n");
		for (s32 i = 0; i < lineCount; i++) {
			if (lines[i].empty() || lines[i].findFirst('#') != -1) {
				continue;
			}

			array<stringc> blocks;
			s32 blockCount = lines[i].split(blocks, "=");
			if (blockCount > 1) {
				stringc item = blocks[0].trim();
				stringc content = blocks[1].trim();

				logger->log(stringc("\t").append(item).c_str());
				stringc logText("\t\t");

				if (item.operator==("shadowMapDimensionFactor")) {
					shadowMapDimensionFactor = str_to_u32(content);

					logText += stringc(shadowMapDimensionFactor);
				}
				if (item.operator==("XEffectFilterType")) {
					XEffectFilterType = str_to_u32(content);

					logText += stringc(XEffectFilterType);
				}
				else if (item.operator==("useVSMShadows")) {
					useVSMShadows = content.operator==("true");

					logText += stringc(useVSMShadows ? "true" : "false");
				}
				else if (item.operator==("useRoundSpotLights")) {
					useRoundSpotLights = content.operator==("true");

					logText += stringc(useRoundSpotLights ? "true" : "false");
				}
				else if (item.operator==("use32BitDepthBuffers")) {
					use32BitDepthBuffers = content.operator==("true");

					logText += stringc(use32BitDepthBuffers ? "true" : "false");
				}

				logger->log(logText.c_str());
			}
		}
		file->drop();
	}
	else {
		logger->log(R"(Failed to open "quality.txt")");
	}

	for (u32 i = 0; i < shadowMapDimensionFactor; i++) {
		shadowMapDimension *= 2;
	}

	EffectHandler* effect = new EffectHandler(device, device->getVideoDriver()->getScreenSize(), useVSMShadows, useRoundSpotLights, use32BitDepthBuffers);

	effect->setAmbientColor(SColor(255, 64, 64, 64));
	effect->setClearColour(SColor(255, 250, 255, 255));
	effect->addPostProcessingEffectFromFile("resources/shaders/BlurHP.glsl");
	effect->addPostProcessingEffectFromFile("resources/shaders/BlurVP.glsl");
	effect->addPostProcessingEffectFromFile("resources/shaders/BloomP.glsl");
	effect->addPostProcessingEffectFromFile("resources/shaders/BrightPass.glsl");

	return effect;
}

#endif // !__XEFFECT_INITRAIZER_H_INCLUDED__