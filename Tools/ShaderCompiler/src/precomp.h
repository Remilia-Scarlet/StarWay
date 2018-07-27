#pragma once
#define NOMINMAX
#include "Ash/Settings.h"
#include <stdint.h>
#include "Ash/FileSystem/fs_include.h"
#include <boost/serialization/split_free.hpp>

uint64_t getTimeStamp(const Path& file);

namespace boost {
	namespace serialization {

		template<class Archive>
		inline void save(Archive & ar,const Path &path, const unsigned int file_version)
		{
			ar << path.getRelativePath();
		}

		template<class Archive>
		inline void load(Archive & ar, Path &path,const unsigned int file_version)
		{
			std::string pathStr;
			ar >> pathStr;
			path = pathStr;
		}

		// split non-intrusive serialization function member into separate
		// non intrusive save/load member functions
		template<class Archive>
		inline void serialize(Archive & ar, Path &t,const unsigned int file_version) 
		{
			boost::serialization::split_free(ar, t, file_version);
		}
	} // namespace serialization
} // namespace boost