#include <fstream>
#include <string>
#include <iostream>
#include <memory>
#include "IO/cinputoutput.h"
#include "Lexer/clexer.h"
#include "Syntax/csyntax.h"

int main() {

	auto m = std::make_unique<CInput>("C:\\Users\\aaa\\source\\repos\\pascal_compiler\\pascal_compiler\\tests\\gcd.pas");
	
	/*auto l = std::move(m->NextChar());
	while (l->c != EOF) {
		std::cout << l->lineNumber << ":" << l->linePosition << " " << l->c << std::endl;
		l = std::move(m->NextChar());
	}*/
	
	auto lexer = std::make_unique<CLexer>(m.release());
	
	/*auto token = lexer->NextToken();
	while (token!=nullptr) {
		std::cout << token->getLineNumber() << ":" << token->getLinePosition() << " " << token->toString() << std::endl;
		token.reset(lexer->NextToken().release());
	}*/

	auto syntax = std::make_unique<CSyntax>(lexer.release());
	syntax->Run();

	return 0;
}