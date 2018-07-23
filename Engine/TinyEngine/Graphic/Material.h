#pragma once
#include "GraphicCommand.h"

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

TINY_DEFINE_PTR(Material);

class Material : public RefCountObj
{
public:

protected:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & _shaderName;
		ar & _command;
	}
	std::string _shaderName;
	std::vector<GraphicCommand> _command; //We input some info here, for example textures and constants
};
