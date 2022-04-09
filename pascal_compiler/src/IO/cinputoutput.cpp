#include "cinputoutput.h"

CInputOutput::CInputOutput(std::string filePath, std::string outputPath) {
	this->inputFileStream = std::ifstream(filePath);
	this->outputFileStream = std::ofstream(outputPath);
	this->currentLine = 0;
	this->currentLinePosition = 0;
}

std::unique_ptr<CLiteral> CInputOutput::NextChar() {
	char c = this->inputFileStream.get();
	
	std::unique_ptr<CLiteral> l = std::make_unique<CLiteral>(c, this->currentLine, this->currentLinePosition);

	this->currentLinePosition++;
	if (c == '\n') {
		this->currentLine++;
		this->currentLinePosition = 0;
	}

	return l;
}
