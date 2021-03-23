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

class SetPsLocalConstantCmd : public GraphicCommand
{
public:
	template<class ValueType>
	SetPsLocalConstantCmd(int constantId, const ValueType& data);
#if defined(TINY_TOOL_MODE)
	template<class ValueType>
	SetPsLocalConstantCmd(const std::string& shaderName, const std::string& constantName, const ValueType& data);
#endif
	SetPsLocalConstantCmd(SetPsLocalConstantCmd&&) = default;
public:
	virtual void apply()override {};
protected:
	SetPsLocalConstantCmd() = default;//for boost serialization only 
protected:
	int _constantId = -1;
	std::vector<uint8_t> _data;
#if defined(TINY_TOOL_MODE)
	std::string _shaderName;
	std::string _constName;
#endif
	friend class boost::serialization::access;
	template<class Archive>
	void serializeEngine(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<GraphicCommand>(*this);
		ar & _constantId;
		ar & _data;
	}
	template<class Archive>
	void serializeToolMode(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<GraphicCommand>(*this);
		ar & _shaderName;
		ar & _constName;
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
#if defined(TINY_TOOL_MODE)
template <class ValueType>
SetPsLocalConstantCmd::SetPsLocalConstantCmd(const std::string& shaderName, const std::string& constantName,
	const ValueType& data)
{
	int constantId = PsLocalConstant::getConstantIdFromName(shaderName, constantName);
	LocalConstantInfo info = getPsLocalParamInfo(constantId);
	TinyAssert(info._size == (int)sizeof(ValueType));
	_data.resize(sizeof(ValueType));
	memcpy(_data.data(), &data, sizeof(ValueType));
}
#endif