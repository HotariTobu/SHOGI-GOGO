#include "ScaleDefiner.h"
#include "Converter.h"

ScaleDefiner::ScaleDefiner(IrrlichtDevice* device) {
	Scales[ESN_CAMERA_RADIUS] = 10.0f;
	Scales[ESN_PIECE_HEIGHT] = 1.0f;
	Scales[ESN_TATAMI_HEIGHT] = 30.0f;
	Scales[ESN_BOARD_VERTICAL] = 11.74193548387097f;
	Scales[ESN_BOARD_HORIZON] = 10.74193548387097f;
	Scales[ESN_BOARD_THICKNESS] = 6.5f;
	Scales[ESN_BOARD_FOOT_HEIGHT] = 3.5f;
	Scales[ESN_BOARD_FOOT_MARGIN] = 2.0f;
	Scales[ESN_BOARD_POOL_HEIGHT] = 0.5f;
	Scales[ESN_BOARD_POOL_OUT_HEIGHT] = 3.354838709677419f;
	Scales[ESN_BOARD_POOL_OUT_WIDTH] = 3.064516129032258f;
	Scales[ESN_BOARD_POOL_IN_HEIGHT] = 2.516129032258065f;
	Scales[ESN_BOARD_POOL_IN_WIDTH] = 2.290322580645161f;
	Scales[ESN_STAND_HEIGHT] = 9.5f;
	Scales[ESN_STAND_MARGIN] = 2.064516129032258f;
	Scales[ESN_RECTANGLE_HEIGHT] = 1.248387096774194f;
	Scales[ESN_RECTANGLE_WIDTH] = 1.141935483870968f;
	Scales[ESN_RECTANGLE_LINE_THICKNESS] = 5.0f;

	ILogger* logger = device->getLogger();
	IReadFile *file = device->getFileSystem()->createAndOpenFile("scale.txt");
	if (file) {
		logger->log(R"(Succeed to open "scale.txt")");

		u32 size = file->getSize();
		c8* buffer = new c8[size];
		size = file->read(buffer, size);
		stringc str(buffer);
		delete buffer;

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
				f32 value = str_to_f32(blocks[1].trim());

				logger->log(stringc("\t").append(item).c_str());
				logger->log(stringc("\t\t").append(stringc(value)).c_str());

				for (u32 id = 0; id < ESN_COUNT; id++) {
					if (item.operator==(ScaleNameStrs[id])) {
						Scales[id] = value;
					}
				}
			}
		}
		file->drop();
	}
	else {
		logger->log(R"(Failed to open "scale.txt")");
	}
}

f32 ScaleDefiner::getScale(E_SCALE_NAMES sn) {
	return Scales[sn];
}