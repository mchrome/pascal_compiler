#pragma once
#include <fstream>
#include "../Utils/cliteral.h"
#include "../Utils/cerror.h"
#include <memory>
class CInputOutput {

private:
	int currentLine;
	int currentLinePosition;
	std::ifstream inputFileStream;
	std::ofstream outputFileStream;

public:
	CInputOutput(std::string filePath, std::string outputPath);
	std::unique_ptr<CLiteral> NextChar();
	void WriteError(CError error);
};