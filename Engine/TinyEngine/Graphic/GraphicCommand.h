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
		UNSET,
		SET_LOCAL_PS_CONSTANT,
		SET_TEXTURE
	};
	GraphicCommand(CommandType type);
	GraphicCommand(const GraphicCommand& other) = delete;
	GraphicCommand(GraphicCommand&& other) = default;
	virtual ~GraphicCommand();
public:
	void apply();
protected:
	CommandType _commandType = CommandType::UNSET;
	union
	{
		SetPsConstantDetail* _setConstantDetail;
		SetTextureDetail* _setTextureDetail;
		void* _setDetail;
	};
	friend class boost::serialization::access;
	//template<class Archive>
	//void serialize(Archive & ar, const unsigned int version)
	//{
	//	ar & _commandType;
	//	switch (_commandType)
	//	{
	//	case CommandType::SET_LOCAL_PS_CONSTANT:
	//		ar & _setConstantDetail;
	//		break;
	//	case CommandType::SET_TEXTURE:
	//		ar & _setTextureDetail;
	//		break;
	//	}
	//}
	template <class Archive> void serialize(Archive &, const unsigned int) {
		TinyAssert(false);
	}

	template<class Archive>
	inline friend void save_construct_data(Archive & ar, const GraphicCommand * graphicCommand, const unsigned int file_version)
	{
		ar << graphicCommand->_commandType;
		switch (graphicCommand->_commandType)
		{
		case GraphicCommand::CommandType::SET_LOCAL_PS_CONSTANT:
			ar << graphicCommand->_setConstantDetail;
			break;
		case GraphicCommand::CommandType::SET_TEXTURE:
			ar << graphicCommand->_setTextureDetail;
			break;
		}
	}

	template<class Archive>
	inline friend void load_construct_data(Archive & ar, GraphicCommand * graphicCommand, const unsigned int file_version)
	{
		GraphicCommand::CommandType cmdType;
		ar >> cmdType;
		new (graphicCommand) GraphicCommand(cmdType);

		switch (graphicCommand->_commandType)
		{
		case GraphicCommand::CommandType::SET_LOCAL_PS_CONSTANT:
			ar >> graphicCommand->_setConstantDetail;
			break;
		case GraphicCommand::CommandType::SET_TEXTURE:
			ar >> graphicCommand->_setTextureDetail;
			break;
		}
	}
};/*
namespace boost {
	namespace serialization {

		template<class Archive>
		inline void save_construct_data(Archive & ar, const GraphicCommand * graphicCommand, const unsigned int file_version)
		{
			
		}

		template<class Archive>
		inline void load_construct_data(Archive & ar, GraphicCommand * graphicCommand, const unsigned int file_version)
		{
			
		}
	}
}*/

struct SetPsConstantDetailSaver
{
	std::string _constantName;
	std::vector<uint8_t> _data;
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & _constantName;
		ar & _data;
	}
};

struct SetTextureDetailSaver
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

struct GraphicCommandSaver
{
	GraphicCommandSaver(){}
	~GraphicCommandSaver() {}
	GraphicCommand::CommandType _commandType = GraphicCommand::CommandType::UNSET;
	union
	{
		SetPsConstantDetailSaver _setConstantDetail;
		SetTextureDetailSaver _setTextureDetail;
	};
};