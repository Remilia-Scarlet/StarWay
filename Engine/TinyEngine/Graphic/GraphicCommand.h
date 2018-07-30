#pragma once

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/export.hpp>
#include "Graphic/ShaderInclude/ShaderInfo.h"


class GraphicCommand
{
public:
	GraphicCommand() = default;
	GraphicCommand(const GraphicCommand& other) = delete;
	GraphicCommand(GraphicCommand&& other) = default;
	virtual ~GraphicCommand() = default;
public:
	virtual void apply() = 0;
protected:
	friend class boost::serialization::access;
	template <class Archive> void serialize(Archive &, const unsigned int)
	{
		//nothing to do
	}
};

class SetPsLocalConstantCmd : GraphicCommand
{
public:
	template<class ValueType>
	SetPsLocalConstantCmd(int constantId, const ValueType& data);
	SetPsLocalConstantCmd(SetPsLocalConstantCmd&&) = default;
protected:
	SetPsLocalConstantCmd() = default;//for boost serialization only 
	int _constantId = -1;
	std::vector<uint8_t> _data;
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<GraphicCommand>(*this);
		ar & _constantId;
		ar & _data;
	}
};
BOOST_CLASS_EXPORT_KEY(SetPsLocalConstantCmd)

struct SetTextureCmd : public GraphicCommand
{
public:
	SetTextureCmd(int slot, const std::string& texName);
	SetTextureCmd(SetTextureCmd&&) = default;
protected:
	SetTextureCmd() = default;//for boost serialization only 
	int _slot = -1;
	std::string _textureName;
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<GraphicCommand>(*this);
		ar & _slot;
		ar & _textureName;
	}
};
BOOST_CLASS_EXPORT_KEY(SetTextureCmd)

////////////////////////////////////////////////////////////////////////////////////////////////////
template <class ValueType>
SetPsLocalConstantCmd::SetPsLocalConstantCmd(int constantId, const ValueType& data)
	:_constantId(constantId)
{
	LocalConstantInfo info = getPsLocalParamInfo(constantId);
	TinyAssert(info._size == (int)sizeof(ValueType));
	_data.resize(sizeof(ValueType));
	memcpy(_data.data(), &data, sizeof(ValueType));
}
