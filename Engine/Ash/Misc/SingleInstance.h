#pragma once

namespace Ash
{
    template<typename T>
    class SingleInstance
    {
    public:
	    static T* instance()
	    {
		    static T ins{};
		    return &ins;
	    }
    };
}
