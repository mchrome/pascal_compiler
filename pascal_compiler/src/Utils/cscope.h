#pragma once
#include <memory>
#include <map>
#include <string>
#include "ctype.h"
#include <vector>
#include "ctoken.h"

class CScope : public std::enable_shared_from_this<CScope> {
private:
	std::shared_ptr<CScope> parent;
	

public:
	std::map<std::string, std::string> identifiers;
	std::map<std::string, CBaseType> types;
	std::map<std::string, std::vector<CBaseType>> functionParameters;
	std::shared_ptr<CScope> GetParentScope();
	CScope(std::shared_ptr<CScope> parent);
};