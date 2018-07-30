#pragma once
#include "GraphicCommand.h"

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include "Graphic/ShaderInclude/ShaderInfo.h"
#include "Ash/RefCountPointer/RefCountPtr.h"
#include "Ash/RefCountPointer/RefCountObj.h"

TINY_DEFINE_PTR(Material);

class Material : public RefCountObj
{
public:
	static MaterialPtr create(const std::string& fileName);
protected:
	Material();
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & _shader;
		ar & _command;
	}
	PixelShaders _shader;
	std::vector<std::unique_ptr<GraphicCommand> > _command; //We input some info here, for example textures and constants
};

// Origin material property. 
struct MaterialSaver
{
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & _shader;
		ar & _command;
	}
	std::string _shader;
	std::vector<GraphicCommand> _command; //We input some info here, for example textures and constants
};
