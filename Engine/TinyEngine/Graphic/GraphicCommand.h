#pragma once

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include "Graphic/ShaderInclude/ShaderInfo.h"

struct SetPsConstantDetail
{
	int _constantId;
	std::vector<uint8_t> _data;
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & _constantId;
		ar & _data;
	}
};

struct SetTextureDetail
{
	int _slot;
	std::string _textureName;
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & _slot;
		ar & _textureName;
	}
};

class GraphicCommand
{
public:
	enum class CommandType
	{
		SET_LOCAL_PS_CONSTANT,
		SET_TEXTURE
	};

	GraphicCommand(CommandType type);
	virtual ~GraphicCommand();
public:
	void apply();
protected:
	CommandType _commandType;
	union
	{
		SetPsConstantDetail _setConstantDetail;
		SetTextureDetail _setTextureDetail;
	};
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & _commandType;
		switch (_commandType)
		{
		case CommandType::SET_LOCAL_PS_CONSTANT:
			ar & _setConstantDetail;
			break;
		case CommandType::SET_TEXTURE:
			ar & _setTextureDetail;
			break;
		}
	}
};
