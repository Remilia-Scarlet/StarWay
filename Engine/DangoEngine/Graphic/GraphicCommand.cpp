#include "DangoEngine/precomp.h"
#include "GraphicCommand.h"

SetTextureCmd::SetTextureCmd(int slot, const std::string& texName)
	:_slot(slot)
	, _textureName(texName)
{
}

BOOST_CLASS_EXPORT_IMPLEMENT(SetPsLocalConstantCmd)
BOOST_CLASS_EXPORT_IMPLEMENT(SetTextureCmd)