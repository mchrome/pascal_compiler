#include "cinputoutput.h"

CInput::CInput(std::string filePath) {
	this->inputFileStream = std::ifstream(filePath);
	this->currentLine = 0;
	this->currentLinePosition = 0;
}

std::shared_ptr<CLiteral> CInput::NextChar() {
	char c = this->inputFileStream.get();
	
	std::shared_ptr<CLiteral> l = std::make_shared<CLiteral>(c, this->currentLine, this->currentLinePosition);

	this->currentLinePosition++;
	if (c == '\n') {
		this->currentLine++;
		this->currentLinePosition = 0;
	}

	return l;
}

COutput::COutput(std::string outFilePath)
{
	this->outputFileStream = std::ofstream(outFilePath);
}

void COutput::WriteErrorStd(std::shared_ptr<CError> err)
{
	std::cout << err->toString() << "\n";
}

void COutput::WriteErrorFile(std::shared_ptr<CError> err)
{
	this->outputFileStream << err->toString() << "\n";
}
