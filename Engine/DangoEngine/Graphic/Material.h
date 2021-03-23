#pragma once
#include "GraphicCommand.h"

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include "Graphic/ShaderInclude/ShaderInfo.h"
#include "Ash/RefCountPointer/RefCountPtr.h"
#include "Ash/RefCountPointer/RefCountObj.h"
#include "Ash/FileSystem/fs_include.h"

ASH_DEFINE_PTR(Material);

class Material : public RefCountObj
{
public:
	static MaterialPtr create(const std::string& fileName);
#if defined(TINY_TOOL_MODE)
	static MaterialPtr create(const std::string& shaderName, std::vector<std::unique_ptr<GraphicCommand> > _command);
	void saveOrigin(Path outputFile);
	void saveCompiled(Path outputFile);
#endif
protected:
	Material();
	friend class boost::serialization::access;
	template<class Archive>
	void serializeEngine(Archive & ar, const unsigned int version)
	{
		ar & _shader;
		ar & _command;
	}
#if defined(TINY_TOOL_MODE)
	template<class Archive>
	void serializeTool(Archive & ar, const unsigned int version)
	{
		ar & _shaderName;
		if (Archive::is_loading::value)
			_shader = getPixelShaderFromStr(_shaderName);
		ar & _command;
	}
#endif
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
#if defined(TINY_TOOL_MODE)
		serializeTool(ar, version);
#else
		serializeEngine(ar, version);
#endif
	}
#if defined(TINY_TOOL_MODE)
	std::string _shaderName;
#endif
	PixelShader _shader;
	std::vector<std::unique_ptr<GraphicCommand> > _command; //We input some info here, for example textures and constants
};
