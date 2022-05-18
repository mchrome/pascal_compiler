#include "cscope.h"

CScope::CScope(std::shared_ptr<CScope> parent)
{
	this->parent = parent;
}



std::shared_ptr<CScope> CScope::GetParentScope()
{
	return this->parent;
}
