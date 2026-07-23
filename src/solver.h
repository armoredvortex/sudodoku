#include <string>
#include <array>

struct PuzzleSolution {
	bool solvable;
	std::array<std::array<int,9>,9> solution;
};

bool ValidatePuzzleString(const std::string& puzzle);

PuzzleSolution SolvePuzzle(const std::string& puzzle);