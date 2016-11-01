#include "Graphic/precomp.h"
#include "InputLayoutDefine.h"
#include <vector>
#include "CommonVertex.h"
#include "DefaultVertex.h"

std::vector<std::vector<InputLayoutStruct>> GetInputLayoutDescs()
{
	std::vector<std::vector<InputLayoutStruct>> descs;
	descs.resize(size_t(InputLayoutType::TYPE_NUMBER));

	descs[size_t(InputLayoutType::DEFAULT)] = DefaultVertex::getInputLayoutStruct();
	descs[size_t(InputLayoutType::COMMON)] = CommonVertex::getInputLayoutStruct();

	return descs;
}
