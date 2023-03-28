#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "ChessBoard.h"

class BoardWalkerCell {
public:
	glm::ivec2 pos, dir;
};

class BoardWalker {
public:
	static void GetPossibleMoves(
		glm::ivec2 pos,
		std::vector<glm::ivec2>& dirs,
		ChessBoard& board,
		std::vector<glm::ivec2>& result);
};