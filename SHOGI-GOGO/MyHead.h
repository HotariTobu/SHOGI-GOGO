#pragma once

#ifndef __MY_HEAD_H_INCLUDED__
#define __MY_HEAD_H_INCLUDED__

#include "Head.h"

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#include "Converter.h"
using namespace utils;

//extern s32 grayscaleMaterialType;
extern u32 shadowMapDimension;
extern u32 XEffectFilterType;

extern void convertMesh(IMesh* mesh);

#endif // !__MY_HEAD_H_INCLUDED__