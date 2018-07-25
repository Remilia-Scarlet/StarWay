#pragma once

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

enum class GraphicCommandType
{
	SET_CONSTANT,
	SET_TEXTURE
};
struct GraphicCmdSetConstantDetail
{
	enum SetConstantType
	{
		FLOAT,
		VECTER2,
		VECTER3,
		VECTER4,
		MATRIX4x4
	};
	SetConstantType _constantType;

};
class GraphicCommand
{
protected:
	GraphicCommand(GraphicCommandType type);
	virtual ~GraphicCommand();
public:
	void apply();
protected:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		//ar & degrees;
		//ar & minutes;
		//ar & seconds;
	}

};
