#pragma once
#include "ctoken.h"
#include <vector>
class CParameters {
public:
	CParameters();
	std::vector<std::shared_ptr<CToken>> names;
	std::vector<std::shared_ptr<CToken>> types;
	void AddParameters(std::shared_ptr<CParameters> params);
};