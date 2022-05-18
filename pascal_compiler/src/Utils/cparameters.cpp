#include "cparameters.h"

CParameters::CParameters()
{

}

void CParameters::AddParameters(std::shared_ptr<CParameters> params)
{
	this->names.reserve(this->names.size() + std::distance(params->names.begin(), params->names.end()));
	this->names.insert(this->names.end(), params->names.begin(), params->names.end());
	this->types.reserve(this->types.size() + std::distance(params->types.begin(), params->types.end()));
	this->types.insert(this->types.end(), params->types.begin(), params->types.end());
}
