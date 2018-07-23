#pragma once

class DataStream;

class GraphicCommand
{
public:
	bool serialize(DataStream* dataStream);
	bool unserialize(DataStream* dataStream);

protected:

};
