#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "ChessBoard.h"

class BoardWalker {
public:
	glm::ivec2 pos, dir;
};

class MoveBuilder {
public:
	static void TestPoints(
		std::vector<glm::ivec2>& points,
		glm::ivec2 pos,
		ChessBoard& board,
		std::vector<glm::ivec2>& result);

	static void WalkBoard(
		std::vector<glm::ivec2>& dirs,
		glm::ivec2 pos,
		ChessBoard& board,
		std::vector<glm::ivec2>& result);
};