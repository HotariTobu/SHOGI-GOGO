#include "Pieces.h"
#include "Converter.h"

Pieces::Pieces(IrrlichtDevice* device, ISceneNode* parent, ScaleDefiner* sd) {
	IVideoDriver* drv = device->getVideoDriver();
	ISceneManager* mgr = device->getSceneManager();
	IFileSystem* fsy = device->getFileSystem();
	ILogger* logger = device->getLogger();

	IReadFile *file = fsy->createAndOpenFile("piece.txt");
	if (file) {
		logger->log(R"(Succeed to open "piece.txt")");
		logger->log("(index, rate, scale)");

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
			s32 blockCount = lines[i].split(blocks, ":");
			if (blockCount > 2) {
				u32 index = str_to_u32(blocks[0].trim());
				f32 rate = str_to_f32(blocks[1].trim());
				f32 scale = str_to_f32(blocks[2].trim());

				logger->log((stringc("(") + stringc(index) + ", " + stringc(rate) + ", " + stringc(scale) + ")").c_str());

				if (index < 8 && index >= 0) {
					Piece* piece = new Piece(parent, mgr, index, sd, rate);
					piece->setScale(vector3df(scale, scale, scale));
					ITexture* texture = drv->getTexture((stringc("resources/images/piece_") + stringc(index) + ".png").c_str());
					if (texture) {
						piece->setMaterialTexture(0, texture);
					}

					pieces3d[index] = piece;
				}
			}
		}
		file->drop();
	}
	else {
		logger->log(R"(Failed to open "piece.txt")");
	}
}

ITexture* Pieces::getPiece2d(E_PIECE_NAME pn) {
	return pieces2d[pn];
}

Piece* Pieces::getPiece3d(E_PIECE_NAME pn) {
	return pieces3d[pn];
}