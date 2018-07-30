#include "TinyEngine\precomp.h"
#include "GraphicCommand.h"

GraphicCommand::GraphicCommand(CommandType type)
	:_commandType(type)
{
	switch (_commandType)
	{
	case CommandType::SET_LOCAL_PS_CONSTANT:
		new (&_setConstantDetail) SetPsConstantDetail;
		break;
	case CommandType::SET_TEXTURE:
		new (&_setTextureDetail) SetTextureDetail;
		break;
	}
}

GraphicCommand::~GraphicCommand()
{
	switch (_commandType)
	{
	case CommandType::SET_LOCAL_PS_CONSTANT:
		_setConstantDetail.~SetPsConstantDetail();
		break;
	case CommandType::SET_TEXTURE:
		_setTextureDetail.~SetTextureDetail();
		break;
	}
}
