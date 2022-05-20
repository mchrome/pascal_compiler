#pragma once
#include <fstream>
#include "../Utils/cliteral.h"
#include "../Utils/cerror.h"
#include <memory>
class CInput {

private:
	int currentLine;
	int currentLinePosition;
	std::ifstream inputFileStream;

public:
	CInput(std::string inputFilePath);
	std::shared_ptr<CLiteral> NextChar();
};

class COutput {
private:
	std::ofstream outputFileStream;
public:
	COutput(std::string outFilePath);
	void WriteErrorStd(std::shared_ptr<CError> err);
	void WriteErrorFile(std::shared_ptr<CError> err);
};