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
		glm::ivec2 pos,
		std::vector<glm::ivec2>& points,
		ChessBoard& board,
		std::vector<glm::ivec2>& result);

	static void WalkBoard(
		glm::ivec2 pos,
		std::vector<glm::ivec2>& dirs,
		ChessBoard& board,
		std::vector<glm::ivec2>& result);
};